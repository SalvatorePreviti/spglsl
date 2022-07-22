#include "spglsl-angle-compiler.h"

#include <ostream>

#include <angle/src/compiler/translator/Initialize.h>
#include <angle/src/compiler/translator/ValidateOutputs.h>
#include <angle/src/compiler/translator/ValidateVaryingLocations.h>
#include <angle/src/compiler/translator/tree_ops/FoldExpressions.h>
#include <angle/src/compiler/translator/tree_ops/PruneNoOps.h>
#include <angle/src/compiler/translator/tree_ops/RecordConstantPrecision.h>
#include <angle/src/compiler/translator/tree_ops/RemoveArrayLengthMethod.h>
#include <angle/src/compiler/translator/tree_ops/SeparateDeclarations.h>
#include <angle/src/compiler/translator/tree_ops/SplitSequenceOperator.h>
#include <angle/src/compiler/translator/tree_util/IntermNodePatternMatcher.h>
#include <angle/src/compiler/translator/util.h>

#include "spglsl-angle-compiler-handle.h"
#include "spglsl-angle-webgl-output.h"
#include "symbols/spglsl-symbol-usage.h"
#include "tree-ops/tree-ops.h"

SpglslAngleCompiler::SpglslAngleCompiler(sh::GLenum shaderType, SpglslCompileOptions & compilerOptions) :
    SpglslTCompilerHolder(shaderType, compilerOptions.outputShaderVersion),
    compilerOptions(compilerOptions),
    body(nullptr),
    symbols(&this->symbolTable, compilerOptions) {
  this->metadata.shaderSpec = this->tCompiler.getShaderSpec();
  this->metadata.shaderType = shaderType;
  this->metadata.shaderVersion = compilerOptions.outputShaderVersion;
}

bool SpglslAngleCompiler::compile(const char * sourceCode) {
  SetGlobalPoolAllocator(&this->getAllocator());

  this->symbolTable.initializeBuiltIns(
      this->metadata.shaderType, this->tCompiler.getShaderSpec(), this->compilerOptions.angle);
  sh::InitExtensionBehavior(this->compilerOptions.angle, this->extensionBehavior);

  sh::TParseContext parseContext(this->symbolTable, this->extensionBehavior, (GLenum)this->metadata.shaderType,
      this->tCompiler.getShaderSpec(), SH_OBJECT_CODE | SH_FLATTEN_PRAGMA_STDGL_INVARIANT_ALL, true, &this->diagnostics,
      this->compilerOptions.angle, SH_ESSL_OUTPUT);

  parseContext.setFragmentPrecisionHighOnESSL1(this->compilerOptions.angle.FragmentPrecisionHigh == 1);

  TScopedSymbolTableLevel globalLevel(&this->symbolTable);

  const char * sourceCodes[1] = {sourceCode};
  bool parsed = PaParseStrings(1, &sourceCodes[0], nullptr, &parseContext) == 0;

  this->symbolTable.setGlobalInvariant(
      this->metadata.pragma.stdgl.invariantAll || parseContext.pragma().stdgl.invariantAll);

  this->metadata.setFromParseContext(parseContext);
  this->metadata.highPrecisionSupported = this->metadata.shaderVersion > 100 ||
      this->metadata.shaderType != GL_FRAGMENT_SHADER || this->compilerOptions.angle.FragmentPrecisionHigh == 1;

  sh::TIntermBlock * root = parseContext.getTreeRoot();
  this->body = root;

  bool valid = false;
  if (parsed && root) {
    if (this->_checkAndSimplifyAST(root, parseContext)) {
      valid = this->diagnostics.numErrors() == 0;
    }

    this->_collectVariables(root);
  }

  return valid;
}

bool SpglslAngleCompiler::_checkAndSimplifyAST(sh::TIntermBlock * root, const sh::TParseContext & parseContext) {
  if (!FoldExpressions(&this->tCompiler, root, &this->diagnostics)) {
    return false;
  }

  if (!PruneNoOps(&this->tCompiler, root, &this->symbolTable)) {
    return false;
  }

  if (!this->callDag.init(root, &this->diagnostics)) {
    return false;
  }
  this->callDag.pruneUnusedFunctions(root);

  if (!ValidateVaryingLocations(root, &this->diagnostics, this->metadata.shaderType)) {
    return false;
  }

  if (this->metadata.shaderVersion >= 300 && this->metadata.shaderType == GL_FRAGMENT_SHADER &&
      !ValidateOutputs(root, this->extensionBehavior, this->compilerOptions.angle.MaxDrawBuffers, &this->diagnostics)) {
    return false;
  }

  if (this->compilerOptions.recordConstantPrecision) {
    if (!sh::RecordConstantPrecision(&this->tCompiler, root, &this->symbolTable)) {
      return false;
    }
  }

  if (this->compilerOptions.compileMode == SpglslCompileMode::Optimize) {
    if (!SeparateDeclarations(&this->tCompiler, root, &this->symbolTable)) {
      return false;
    }

    if (!SplitSequenceOperator(
            &this->tCompiler, root, sh::IntermNodePatternMatcher::kArrayLengthMethod, &this->symbolTable)) {
      return false;
    }

    if (!RemoveArrayLengthMethod(&this->tCompiler, root)) {
      return false;
    }

    if (!spglsl_treeops_optimize(*this, root)) {
      return false;
    }
  }

  this->loadPrecisions(true);

  if (this->compilerOptions.compileMode == SpglslCompileMode::Optimize) {
    if (this->compilerOptions.minify) {
      spglsl_treeops_minify(*this, root);
    }

    if (this->compilerOptions.mangle) {
      this->_mangle(root);
    }
  }

  return true;
}

void SpglslAngleCompiler::_mangle(sh::TIntermBlock * root) {
  SpglslSymbolUsage usage(this->symbols);
  SpglslSymbolGenerator symgen(usage);

  usage.load(root, this->precisions, &symgen);

  for (const auto & entry : usage.sorted) {
    if (entry->mangleId > 0) {
      entry->entry->renamed = symgen.getOrCreateMangledName(entry->mangleId);
      entry->entry->mustBeRenamedUnique = false;
    }
  }

  if (!this->symbols.compileOptions.mangle_global_map.isUndefined()) {
    for (auto & kv : this->symbols._map) {
      auto & sym = kv.second;
      if (sym.symbol && sym.renamed.empty() && !sym.symbolName.empty()) {
        auto * s = sym.symbolName.c_str();
        auto globalRename = this->symbols.compileOptions.mangle_global_map[s];
        if (globalRename.isString()) {
          sym.renamed = globalRename.as<std::string>();
          sym.mustBeRenamedUnique = false;
        }
      }
    }
  }
}

void SpglslAngleCompiler::loadPrecisions(bool reload) {
  if (reload) {
    this->precisions = SpglslGlslPrecisions();
  }
  if (this->compilerOptions.language == EShLangVertex) {
    this->precisions.defaultIntPrecision = sh::TPrecision::EbpHigh;
    this->precisions.defaultFloatPrecision = sh::TPrecision::EbpHigh;
  } else {
    this->precisions.defaultIntPrecision = sh::TPrecision::EbpMedium;
  }

  this->precisions.floatPrecision = (sh::TPrecision)this->compilerOptions.floatPrecision;
  this->precisions.intPrecision = (sh::TPrecision)this->compilerOptions.intPrecision;

  if (this->precisions.floatPrecision == sh::TPrecision::EbpUndefined ||
      this->precisions.intPrecision == sh::TPrecision::EbpUndefined) {
    SpglslGetPrecisionsTraverser precisionsTraverser;
    precisionsTraverser.traverseNode(this->body);
    precisionsTraverser.count();
    if (this->precisions.floatPrecision == sh::TPrecision::EbpUndefined) {
      this->precisions.floatPrecision = precisionsTraverser.floatPrecision;
    }
    if (this->precisions.intPrecision == sh::TPrecision::EbpUndefined) {
      this->precisions.intPrecision = precisionsTraverser.intPrecision;
    }
  }
}

std::string SpglslAngleCompiler::decompileOutput() {
  if (!this->body) {
    return "";
  }
  std::ostringstream out;

  SpglslAngleWebglOutput outputTraverser(out, this->symbols, this->precisions, this->compilerOptions.beautify);

  outputTraverser.writeHeader(this->metadata.shaderVersion, this->metadata.pragma, this->extensionBehavior);
  this->body->traverse(&outputTraverser);

  return out.str();
}

class CollectVariablesTraverser : public sh::TIntermTraverser {
 public:
  SpglslAngleCompiler & compiler;

  explicit CollectVariablesTraverser(SpglslAngleCompiler & compiler) :
      sh::TIntermTraverser(true, false, false, &compiler.symbolTable), compiler(compiler) {
  }

  bool visitDeclaration(sh::Visit, sh::TIntermDeclaration * node) override {
    const sh::TIntermSequence & sequence = *(node->getSequence());
    if (sequence.empty()) {
      return false;
    }

    const sh::TIntermTyped & typedNode = *(sequence.front()->getAsTyped());
    sh::TQualifier qualifier = typedNode.getQualifier();

    bool isShaderVariable = qualifier == sh::EvqAttribute || qualifier == sh::EvqVertexIn ||
        qualifier == sh::EvqFragmentOut || qualifier == sh::EvqFragmentInOut || qualifier == sh::EvqUniform ||
        sh::IsVarying(qualifier);

    if (typedNode.getBasicType() != sh::EbtInterfaceBlock && !isShaderVariable) {
      return false;
    }

    for (sh::TIntermNode * variableNode : sequence) {
      const sh::TIntermSymbol & variable = *variableNode->getAsSymbolNode();
      if (variable.variable().symbolType() == sh::SymbolType::AngleInternal) {
        continue;  // Internal variables are not collected.
      }

      switch (qualifier) {
        case sh::EvqAttribute:
        case sh::EvqVertexIn:
        case sh::EvqBuffer:
        case sh::EvqFragmentIn:
        case sh::EvqFragmentOut:
        case sh::EvqFragmentInOut: {
          auto & v = this->compiler.symbols.get(&variable.variable());
          const auto & name = this->compiler.symbols.getName(&variable.variable(), false);
          const auto & renamed = this->compiler.symbols.getName(&variable.variable(), true);
          this->compiler.globalsMap.emplace(name, renamed);
          break;
        }

        case sh::EvqUniform: {
          auto & v = this->compiler.symbols.get(&variable.variable());
          const auto & name = this->compiler.symbols.getName(&variable.variable(), false);
          const auto & renamed = this->compiler.symbols.getName(&variable.variable(), true);
          this->compiler.uniformsMap.emplace(name, renamed);
          break;
        }

        default: break;
      }
    }

    return false;
  }
};

void SpglslAngleCompiler::_collectVariables(sh::TIntermBlock * root) {
  CollectVariablesTraverser collectVariablesTraverser(*this);
  root->traverse(&collectVariablesTraverser);
}
