#include "spglsl-angle-webgl-output.h"

#include <angle/src/compiler/translator/Pragma.h>
#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/util.h>

#include "../core/math-utils.h"
#include "lib/spglsl-angle-node-utils.h"
#include "lib/spglsl-angle-operator-precedence.h"

SpglslAngleWebglOutput::SpglslAngleWebglOutput(std::ostream & out, sh::TSymbolTable * symbolTable, bool beautify) :
    sh::TIntermTraverser(true, true, true, symbolTable), SpglslGlslWriter(out, beautify), _skipNextBlockBraces(true) {
}

std::string SpglslAngleWebglOutput::getSymbolName(const sh::TSymbol & symbol) {
  if (!symbol.uniqueId().get() || symbol.symbolType() == sh::SymbolType::Empty) {
    return Strings::empty;
  }
  if (symbol.isFunction() && static_cast<const sh::TFunction &>(symbol).isMain()) {
    return "main";
  }

  const sh::ImmutableString & n = symbol.name();
  size_t len = n.length();

  if (symbol.isVariable()) {
    const sh::TVariable & variable = static_cast<const sh::TVariable &>(symbol);
    if (variable.getType().isInterfaceBlock()) {
      // Interface blocks declared as "_NUM" should be unnamed, they wrongly come like this from glslang.
      bool isHiddenName = len > 1 && n[0] == '_';
      for (size_t i = 1; i < len; ++i) {
        if (!std::isdigit(n[i])) {
          isHiddenName = false;
          break;
        }
      }
      if (isHiddenName) {
        return Strings::empty;
      }
    }
  }

  return std::string(n.data(), len);
}

std::string SpglslAngleWebglOutput::getTypeName(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtStruct && type.getStruct()) {
    return this->getSymbolName(type.getStruct());
  }
  if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    return this->getSymbolName(type.getInterfaceBlock());
  }
  return type.getBuiltInTypeNameString();
}

const sh::TConstantUnion * SpglslAngleWebglOutput::writeConstantUnion(const sh::TType * type,
    const sh::TConstantUnion * pConstUnion,
    bool needsParentheses) {
  if (!type) {
    return nullptr;
  }

  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    this->write(this->getSymbolName(*structure));
    this->write('(');
    const sh::TFieldList & fields = structure->fields();
    for (size_t i = 0, size = fields.size(); i < size; ++i) {
      if (i != 0) {
        this->writeComma();
      }
      pConstUnion = writeConstantUnion(fields[i]->type(), pConstUnion, false);
    }
    this->write(')');
    return pConstUnion;
  }

  size_t size = type->getObjectSize();
  bool writeType = size > 1;
  if (writeType) {
    this->write(this->getTypeName(*type));
    this->write('(');
  }
  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    if (i != 0) {
      this->writeComma();
    }
    switch (pConstUnion->getType()) {
      case sh::EbtFloat: this->write(floatToGlsl(pConstUnion->getFConst(), needsParentheses)); break;
      case sh::EbtInt: this->write(int32ToGlsl(pConstUnion->getIConst())); break;
      case sh::EbtUInt: this->write(uint32ToGlsl(pConstUnion->getUConst())); break;
      case sh::EbtBool: this->write(pConstUnion->getBConst() ? "true" : "false"); break;
      case sh::EbtYuvCscStandardEXT: out << getYuvCscStandardEXTString(pConstUnion->getYuvCscStandardEXTConst()); break;
      default: break;
    }
  }
  if (writeType) {
    this->write(')');
  }
  return pConstUnion;
}

void SpglslAngleWebglOutput::writeVariableType(const sh::TType & type, bool isFunctionArgument) {
  if (type.isInvariant()) {
    this->write("invariant");
  }
  if (type.isPrecise()) {
    this->write("precise");
  }
  auto qualifier = type.getQualifier();
  bool hasQualifier = qualifier != sh::EvqTemporary && qualifier != sh::EvqGlobal;
  if (hasQualifier) {
    this->write(sh::getQualifierString(qualifier));
  }
  if (hasQualifier || isFunctionArgument) {
    this->writeTMemoryQualifier(type.getMemoryQualifier());
  }
  // Declare the struct if we have not done so already.
  if (type.getBasicType() == sh::EbtStruct && type.getStruct() && this->declaredStructs.count(type.getStruct()) == 0) {
    this->declareStruct(*type.getStruct());
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    this->declareInterfaceBlock(*type.getInterfaceBlock());
  } else {
    this->writeTypePrecision(type);
    this->write(this->getTypeName(type));
  }
}

void SpglslAngleWebglOutput::declareInterfaceBlock(const sh::TInterfaceBlock & interfaceBlock) {
  this->write(this->getSymbolName(interfaceBlock));
  this->write('{').indent().beautyNewLine();
  for (const auto field : interfaceBlock.fields()) {
    auto type = field->type();
    if (type) {
      if (type->isMatrix() || type->isStructureContainingMatrices()) {
        auto matrixPacking = type->getLayoutQualifier().matrixPacking;
        if (matrixPacking == sh::EmpColumnMajor) {
          this->write("layout").write('(').write("column_major").write(')').beautySpace();
        } else if (matrixPacking == sh::EmpRowMajor) {
          this->write("layout").write('(').write("row_major").write(')').beautySpace();
        }
      }
      this->writeTMemoryQualifier(type->getMemoryQualifier());
      this->writeTypePrecision(*type);
      this->write(this->getTypeName(*type));
      this->write(this->getFieldName(field));
      this->write(sh::ArrayString(*type));
      this->writeStatementSemicolon();
    }
  }
  this->deindent().write('}');
}

void SpglslAngleWebglOutput::declareStruct(const sh::TStructure & structure) {
  this->write("struct");
  this->write(this->getSymbolName(structure));
  this->write('{').indent().beautyNewLine();
  for (const auto & field : structure.fields()) {
    auto fieldType = field->type();
    if (fieldType) {
      this->writeTypePrecision(*fieldType);
      this->write(this->getTypeName(fieldType));
      this->write(this->getFieldName(field));
      this->write(sh::ArrayString(*fieldType));
      this->writeStatementSemicolon();
    }
  }
  this->deindent().write('}');

  if (this->getSymbolName(structure).length() != 0) {
    this->declaredStructs.emplace(&structure);
  }
}

std::string SpglslAngleWebglOutput::getFieldName(const sh::TField * field) {
  return field ? field->name().data() : Strings::empty;
}

void SpglslAngleWebglOutput::writeVariableDeclarationSymbol(sh::TIntermNode & child) {
  sh::TIntermSymbol * childSym = child.getAsSymbolNode();
  if (!childSym) {
    this->traverseNode(&child);
    return;
  }
  const sh::TVariable & variable = childSym->variable();
  if (variable.name() != "gl_ClipDistance") {
    this->writeTTypeLayoutQualifier(childSym->getType());
  }
  this->writeVariableType(variable.getType(), false);
  this->write(this->getSymbolName(variable)).write(sh::ArrayString(variable.getType()));
}

void SpglslAngleWebglOutput::visitSymbol(sh::TIntermSymbol * node) {
  this->write(this->getSymbolName(node->variable()));
}

void SpglslAngleWebglOutput::visitConstantUnion(sh::TIntermConstantUnion * node) {
  sh::TIntermBinary * parentBinary = nodeGetAsBinaryNode(this->getParentNode());
  bool canSkipParentheses = parentBinary != nullptr && parentBinary->getOp() == EOpAssign;
  this->writeConstantUnion(&node->getType(), node->getConstantValue(), !canSkipParentheses);
}

void SpglslAngleWebglOutput::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  const sh::TType & type = node->getType();
  this->writeVariableType(type, false);
  this->write(sh::ArrayString(type));
  auto proto = node->getFunction();
  this->write(this->getSymbolName(*proto)).write('(');
  size_t paramCount = proto->getParamCount();
  for (size_t i = 0; i < paramCount; ++i) {
    const sh::TVariable * param = proto->getParam(i);
    const sh::TType & paramType = param->getType();
    if (i != 0) {
      this->writeComma();
    }
    this->writeVariableType(paramType, true);
    this->write(this->getSymbolName(*param)).write(sh::ArrayString(paramType));
  }
  this->write(')');
}

void SpglslAngleWebglOutput::visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) {
  this->writeDirective(node->getDirective(), node->getCommand().data());
}

bool SpglslAngleWebglOutput::visitSwizzle(sh::Visit visit, sh::TIntermSwizzle * node) {
  if (visit == sh::PostVisit) {
    this->write('.').writeSwizzle(node->getSwizzleOffsets());
  }
  return true;
}

bool SpglslAngleWebglOutput::visitBinary(sh::Visit visit, sh::TIntermBinary * node) {
  switch (node->getOp()) {
    case EOpIndexDirect:
    case EOpIndexIndirect:
      this->traverseWithParentheses(node, 0);
      this->write('[');
      this->traverseNode(node->getRight());
      this->write(']');
      return false;

    case EOpIndexDirectStruct: {
      if (visit == sh::PreVisit) {
        const sh::TStructure * structure = node->getLeft() ? node->getLeft()->getType().getStruct() : nullptr;
        if (structure) {
          this->traverseWithParentheses(node, 0);
          this->write('.');
          sh::TIntermConstantUnion * indexNode = nodeGetAsConstantUnion(node->getRight());
          const int fieldIndex = indexNode ? indexNode->getIConst(0) : -1;
          if (fieldIndex >= 0 && fieldIndex < structure->fields().size()) {
            this->write(this->getFieldName(structure->fields().at(fieldIndex)));
          } else {
            this->traverseNode(node->getRight());
          }
          return false;
        }
      }
      return true;
    }

    case EOpIndexDirectInterfaceBlock: {
      if (visit == sh::PreVisit) {
        const sh::TInterfaceBlock * iface = node->getLeft() ? node->getLeft()->getType().getInterfaceBlock() : nullptr;
        if (iface) {
          sh::TIntermSymbol * symLeft = nodeGetAsSymbolNode(node->getLeft());
          if (!symLeft || !this->getSymbolName(symLeft->variable()).empty()) {
            this->traverseWithParentheses(node, 0);
            this->write('.');
          }
          sh::TIntermConstantUnion * indexNode = nodeGetAsConstantUnion(node->getRight());
          const int fieldIndex = indexNode ? indexNode->getIConst(0) : -1;
          if (fieldIndex >= 0 && fieldIndex < iface->fields().size()) {
            this->write(this->getFieldName(iface->fields().at(fieldIndex)));
          } else {
            this->traverseNode(node->getRight());
          }
          return false;
        }
      }
      return true;
    }
    default: break;
  }

  if (node->getOp() == EOpInitialize) {
    this->writeVariableDeclarationSymbol(*node->getLeft());
  } else {
    this->traverseWithParentheses(node, 0);
  }
  this->beautySpace();
  this->writeTOperator(node->getOp());
  this->beautySpace();
  this->traverseWithParentheses(node, 1);
  return false;
}

bool SpglslAngleWebglOutput::visitUnary(sh::Visit visit, sh::TIntermUnary * node) {
  auto op = node->getOp();
  if (!opIsBuiltinUnaryFunction(op)) {
    if (op == EOpPostDecrement || op == EOpPostIncrement || op == EOpArrayLength) {
      this->traverseWithParentheses(node, 0);
      this->writeTOperator(op);
    } else {
      this->writeTOperator(op);
      this->traverseWithParentheses(node, 0);
    }
    return false;
  }
  if (visit == sh::PreVisit) {
    this->writeTOperator(op).write('(');
  } else if (visit == sh::InVisit) {
    this->writeComma();
  } else {
    this->write(')');
  }
  return true;
}

bool SpglslAngleWebglOutput::visitTernary(sh::Visit visit, sh::TIntermTernary * node) {
  this->traverseWithParentheses(node, 0);
  this->beautySpace().write('?').beautySpace();
  this->traverseWithParentheses(node, 1);
  this->beautySpace().write(':').beautySpace();
  this->traverseWithParentheses(node, 2);
  return false;
}

bool SpglslAngleWebglOutput::visitIfElse(sh::Visit visit, sh::TIntermIfElse * node) {
  this->write("if").beautySpace().write('(');
  this->traverseNode(node->getCondition());
  this->write(')');

  this->traverseCodeBlock(node->getTrueBlock(), false);
  if (!nodeBlockIsEmpty(node->getFalseBlock())) {
    this->beautySpace().write("else").beautySpace();
    this->traverseCodeBlock(node->getFalseBlock(), true);
  }
  return false;
}

bool SpglslAngleWebglOutput::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  this->beautyDoubleNewLine();
  this->traverseNode(node->getFunctionPrototype());
  this->traverseNode(node->getBody());
  return false;
}

bool SpglslAngleWebglOutput::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (visit == sh::PreVisit) {
    switch (node->getOp()) {
      case EOpCallInternalRawFunction:
      case EOpCallBuiltInFunction:
      case EOpCallFunctionInAST: this->write(this->getSymbolName(*node->getFunction())); break;

      case EOpConstruct: {
        const auto & type = node->getType();
        this->write(this->getTypeName(type));
        if (type.isArray()) {
          this->write(ArrayString(type));
        }
        break;
      }

      default: this->writeTOperator(node->getOp()); break;
    }
    this->write('(');
  } else if (visit == sh::InVisit) {
    this->writeComma();
  } else {
    this->write(')');
  }
  return true;
}

void SpglslAngleWebglOutput::traverseCodeBlock(sh::TIntermBlock * node) {
  if (nodeBlockIsEmpty(node)) {
    this->beautyNewLine().indent().writeStatementSemicolon().deindent();
  } else {
    this->traverseNode(node);
  }
}

bool SpglslAngleWebglOutput::visitSwitch(sh::Visit visit, sh::TIntermSwitch * node) {
  if (visit == sh::PreVisit) {
    this->write("switch").beautySpace().write('(');
  } else if (visit == sh::InVisit) {
    this->write(')').beautySpace();
  }
  return true;
}

bool SpglslAngleWebglOutput::visitCase(sh::Visit visit, sh::TIntermCase * node) {
  if (visit == sh::PreVisit) {
    if (!node->getCondition()) {
      this->write("default").write(':');
      return false;
    }
    this->write("case");
  } else if (visit == sh::PostVisit) {
    this->write(':');
  }
  return true;
}

bool SpglslAngleWebglOutput::visitBlock(sh::Visit visit, sh::TIntermBlock * node) {
  bool skipBlockBraces = this->_skipNextBlockBraces || this->getCurrentTraversalDepth() == 0;
  sh::TIntermSequence * sequence = node->getSequence();
  int initialIndentLevel = this->getIndentLevel();
  if (!skipBlockBraces) {
    this->beautyStatementNewLine();
    this->write('{').indent().beautyNewLine();
  } else {
    this->_skipNextBlockBraces = false;
  }

  if (sequence) {
    sh::TIntermNode * prev = nullptr;
    for (sh::TIntermNode * child : *sequence) {
      if (!nodeBlockIsEmpty(child)) {
        if (((prev && prev->getAsCaseNode()) || child->getAsBlock()) &&
            !nodeBlockContainsSomeSortOfDeclaration(child)) {
          if (child->getAsBlock()) {
            this->_skipNextBlockBraces = true;
          }
          this->beautyNewLine().indent();
          this->traverseNode(child);
          if (isIntermNodeSingleStatement(child)) {
            this->writeStatementSemicolon();
          }
          this->deindent().beautyDoubleNewLine();
        } else {
          this->beautyStatementNewLine(true);
          this->traverseNode(child);
          if (isIntermNodeSingleStatement(child)) {
            this->writeStatementSemicolon();
          }
        }
      }
      prev = child;
    }
  }
  if (!skipBlockBraces) {
    this->setIndentLevel(initialIndentLevel);
    this->beautyStatementNewLine();
    this->write('}');
  }
  return false;
}

bool SpglslAngleWebglOutput::visitGlobalQualifierDeclaration(sh::Visit visit,
    sh::TIntermGlobalQualifierDeclaration * node) {
  this->beautyStatementNewLine(true);
  this->write(node->isPrecise() ? "precise" : "invariant").write(this->getSymbolName(node->getSymbol()->variable()));
  return false;
}

bool SpglslAngleWebglOutput::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  size_t childCount = node->getChildCount();
  for (size_t i = 0; i < childCount; ++i) {
    auto child = node->getChildNode(i);
    if (child) {
      if (child->getAsSymbolNode()) {
        this->writeVariableDeclarationSymbol(*child);
      } else {
        this->traverseNode(child);
      }
    }
  }
  return false;
}

void SpglslAngleWebglOutput::traverseCodeBlock(sh::TIntermBlock * body, bool allowIf) {
  if (nodeBlockIsEmpty(body)) {
    this->beautyNewLine().indent().writeStatementSemicolon().deindent();
  } else {
    sh::TIntermNode * singleNode = nodeGetBlockSingleNode(body);
    if (!this->beautify && singleNode && (allowIf || !singleNode->getAsIfElseNode()) &&
        !nodeIsSomeSortOfDeclaration(singleNode)) {
      this->traverseNode(singleNode);
      if (isIntermNodeSingleStatement(singleNode)) {
        this->write(';');
      }
    } else {
      this->traverseNode(body);
    }
  }
}

bool SpglslAngleWebglOutput::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  sh::TIntermBlock * body = node->getBody();
  sh::TLoopType loopType = node->getType();
  if (loopType == sh::ELoopFor && !node->getInit() && !nodeHasSideEffects(node->getExpression())) {
    loopType = sh::ELoopWhile;  // For without init and expression is just a while
  }
  int boolValue = node->getCondition() ? nodeConstantBooleanValue(node->getCondition()) : 1;
  if (boolValue == 1) {
    loopType = sh::ELoopFor;  // A while loop or do-while loop with always a true condition is just a for (;;)
  }
  switch (loopType) {
    case sh::ELoopFor:
      this->write("for").beautySpace().write('(');
      this->traverseNode(node->getInit());
      this->write(';');
      if (boolValue != 1) {
        this->traverseNode(node->getCondition());
      }
      this->write(';');
      if (nodeHasSideEffects(node->getExpression())) {
        this->traverseNode(node->getExpression());
      }
      this->write(')').beautySpace();
      this->traverseCodeBlock(body, true);
      return false;

    case sh::ELoopWhile: {
      this->write("while").beautySpace().write('(');
      this->traverseNode(node->getCondition());
      this->write(')').beautySpace();
      this->traverseCodeBlock(body, true);
      return false;
    }

    case sh::ELoopDoWhile: {
      this->write("do").beautySpace();
      this->traverseCodeBlock(body, true);
      this->write("while").beautySpace().write('(');
      this->traverseNode(node->getCondition());
      this->write(')').write(';');
      return false;
    }

    default: return true;
  }
}

bool SpglslAngleWebglOutput::visitBranch(sh::Visit visit, sh::TIntermBranch * node) {
  if (visit == sh::PreVisit) {
    this->writeTOperator(node->getFlowOp());
  }
  return true;
}

void SpglslAngleWebglOutput::traverseNode(sh::TIntermNode * node) {
  if (node) {
    node->traverse(this);
  }
}

void SpglslAngleWebglOutput::traverseWithParentheses(sh::TIntermNode * node, int operandIndex) {
  if (node) {
    sh::TIntermNode * child = node->getChildNode(operandIndex);
    if (child) {
      if (childNodeNeedsParentheses(*node, *child, operandIndex)) {
        this->write('(');
        this->traverseNode(child);
        this->write(')');
      } else {
        this->traverse(child);
      }
    }
  }
}

void SpglslAngleWebglOutput::writeHeader(int shaderVersion,
    const TPragma & pragma,
    const sh::TExtensionBehavior & extBehavior) {
  if (shaderVersion > 100) {
    std::string tmp = std::to_string(shaderVersion) + " es";
    this->writeDirective("#version", tmp.c_str());
    this->beautyDoubleNewLine();
  }

  for (sh::TExtensionBehavior::const_iterator iter = extBehavior.begin(); iter != extBehavior.end(); ++iter) {
    if (iter->first != sh::TExtension::ARB_texture_rectangle && iter->second != sh::EBhUndefined) {
      std::string s = GetExtensionNameString(iter->first);
      s += (this->beautify ? " : " : ":");
      s += GetBehaviorString(iter->second);
      this->writeDirective("#extension", s.c_str());
    }
  }

  this->beautyDoubleNewLine();
  if (!pragma.optimize) {
    this->writeDirective("#pragma", "optimize(off)");
  }
  if (pragma.debug) {
    this->writeDirective("#pragma", "debug(on)");
  } else if (!this->beautify) {
    this->writeDirective("#pragma", "webgl_debug_shader_precision(off)");
  }
  if (pragma.stdgl.invariantAll) {
    this->writeDirective("#pragma", "STDGL invariant(all)");
  }
  this->beautyDoubleNewLine();

  if (this->floatPrecision != sh::EbpUndefined && this->floatPrecision != this->defaultFloatPrecision) {
    this->write("precision")
        .write(sh::getPrecisionString(this->floatPrecision))
        .write("float")
        .write(';')
        .beautyNewLine();
  }

  if (this->intPrecision != sh::EbpUndefined && this->intPrecision != this->defaultIntPrecision) {
    this->write("precision").write(sh::getPrecisionString(this->intPrecision)).write("int").write(';').beautyNewLine();
  }

  this->beautyDoubleNewLine();
}

bool SpglslAngleWebglOutput::isIntermNodeSingleStatement(sh::TIntermNode * node) {
  if (node->getAsFunctionDefinition()) {
    return false;
  }
  if (node->getAsBlock()) {
    return false;
  }
  if (node->getAsIfElseNode()) {
    return false;
  }
  if (node->getAsLoopNode()) {
    return false;
  }
  if (node->getAsSwitchNode()) {
    return false;
  }
  if (node->getAsCaseNode()) {
    return false;
  }
  if (node->getAsPreprocessorDirective()) {
    return false;
  }
  return true;
}