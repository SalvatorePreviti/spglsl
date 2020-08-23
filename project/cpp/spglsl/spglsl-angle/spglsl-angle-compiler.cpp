#include "spglsl-angle-compiler.h"

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

#include "spglsl-angle-compiler-handle.h"
#include "spglsl-angle-webgl-output.h"
#include "tree-ops/tree-ops.h"

SpglslAngleCompiler::SpglslAngleCompiler(sh::GLenum shaderType, const SpglslCompileOptions & compilerOptions) :
    SpglslTCompilerHolder(shaderType, compilerOptions.outputShaderVersion),
    compilerOptions(compilerOptions),
    body(nullptr) {
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

  if (!SeparateDeclarations(&this->tCompiler, root)) {
    return false;
  }

  if (!SplitSequenceOperator(
          &this->tCompiler, root, sh::IntermNodePatternMatcher::kArrayLengthMethod, &this->symbolTable)) {
    return false;
  }

  if (!RemoveArrayLengthMethod(&this->tCompiler, root)) {
    return false;
  }

  if (!SpglslOptimizeAngleAst(*this, root)) {
    return false;
  }

  if (!RecordConstantPrecision(&this->tCompiler, root, &this->symbolTable)) {
    return false;
  }

  SpglslAngleReservedWordsTraverser reservedWordsTraverser(this->reservedWords, &this->symbolTable);
  root->traverse(&reservedWordsTraverser);

  SpglslAngleManglerTraverser renamerTraverser(this->reservedWords, &this->symbolTable);
  root->traverse(&renamerTraverser);

  return true;
}

std::string SpglslAngleCompiler::decompileOutput() {
  return this->decompileOutput(this->compilerOptions.minify);
}

std::string SpglslAngleCompiler::decompileOutput(bool minify) {
  if (!this->body) {
    return "";
  }
  std::ostringstream out;

  SpglslAngleWebglOutput outputTraverser(out, &this->symbolTable, !minify, &this->reservedWords);

  if (this->compilerOptions.language == EShLangVertex) {
    outputTraverser.defaultIntPrecision = sh::TPrecision::EbpHigh;
    outputTraverser.defaultFloatPrecision = sh::TPrecision::EbpHigh;
  } else {
    outputTraverser.defaultIntPrecision = sh::TPrecision::EbpMedium;
  }

  outputTraverser.floatPrecision = (sh::TPrecision)this->compilerOptions.floatPrecision;
  outputTraverser.intPrecision = (sh::TPrecision)this->compilerOptions.intPrecision;

  if (outputTraverser.floatPrecision == sh::TPrecision::EbpUndefined ||
      outputTraverser.intPrecision == sh::TPrecision::EbpUndefined) {
    SpglslGetPrecisionsTraverser precisionsTraverser;
    precisionsTraverser.traverseNode(this->body);
    precisionsTraverser.count();
    if (outputTraverser.floatPrecision == sh::TPrecision::EbpUndefined) {
      outputTraverser.floatPrecision = precisionsTraverser.floatPrecision;
    }
    if (outputTraverser.intPrecision == sh::TPrecision::EbpUndefined) {
      outputTraverser.intPrecision = precisionsTraverser.intPrecision;
    }
  }

  outputTraverser.writeHeader(this->metadata.shaderVersion, this->metadata.pragma, this->extensionBehavior);
  this->body->traverse(&outputTraverser);

  return out.str();
}
