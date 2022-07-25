#include "spglsl-angle-webgl-output.h"

#include <angle/src/compiler/translator/Pragma.h>
#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/util.h>

#include "../core/math-utils.h"
#include "lib/spglsl-angle-node-utils.h"
#include "lib/spglsl-angle-operator-precedence.h"
#include "symbols/spglsl-symbol-info.h"

SpglslAngleWebglOutput::SpglslAngleWebglOutput(std::ostream & out,
    SpglslSymbols & symbols,
    const SpglslGlslPrecisions & precisions,
    bool beautify) :
    SpglslScopedTraverser(symbols), SpglslGlslWriter(out, precisions, beautify) {
}

const std::string & SpglslAngleWebglOutput::getSymbolName(const sh::TSymbol * symbol) {
  return this->symbols.getName(symbol);
}

std::string SpglslAngleWebglOutput::getFunctionName(sh::TIntermAggregate * aggregateNode) {
  if (!aggregateNode) {
    return Strings::empty;
  }
  const auto op = aggregateNode->getOp();
  if (op == sh::EOpCallInternalRawFunction || op == sh::EOpCallFunctionInAST || sh::BuiltInGroup::IsBuiltIn(op)) {
    return this->getSymbolName(aggregateNode->getFunction());
  }
  return sh::GetOperatorString(op);
}

std::string SpglslAngleWebglOutput::getFieldName(const sh::TField * field) {
  return field ? field->name().data() : Strings::empty;
}

std::string SpglslAngleWebglOutput::getBuiltinTypeName(const sh::TType * type) {
  return type->getBuiltInTypeNameString();
}

std::string SpglslAngleWebglOutput::getTypeName(const sh::TType * type) {
  if (type == nullptr) {
    return Strings::empty;
  }
  if (type->getBasicType() == sh::EbtStruct && type->getStruct()) {
    return this->getSymbolName(type->getStruct());
  }
  if (type->getBasicType() == sh::EbtInterfaceBlock && type->getInterfaceBlock()) {
    return this->getSymbolName(type->getInterfaceBlock());
  }
  return this->getBuiltinTypeName(type);
}

void SpglslAngleWebglOutput::writeTOperatorNode(sh::TIntermOperator * node) {
  sh::TOperator op = node->getOp();

  switch (op) {
    case sh::EOpKill: this->write("discard"); return;
    case sh::EOpBreak: this->write("break"); return;
    case sh::EOpContinue: this->write("continue"); return;
    case sh::EOpReturn: this->write("return"); return;
    default: break;
  }

  sh::TIntermUnary * unaryNode = node->getAsUnaryNode();
  if (unaryNode) {
    const auto * fn = unaryNode->getFunction();
    if (fn) {
      this->write(this->getSymbolName(fn));
      return;
    }
  }

  sh::TIntermAggregate * aggregateNode = node->getAsAggregate();
  if (aggregateNode) {
    const auto * fn = aggregateNode->getFunction();

    switch (op) {
      case sh::EOpCallInternalRawFunction:
      case sh::EOpCallFunctionInAST: {
        if (!fn) {
          break;
        }
        this->write(this->getSymbolName(fn));
        return;
      }

      case sh::EOpConstruct: {
        const auto & type = aggregateNode->getType();
        this->write(this->getTypeName(&type));
        if (type.isArray()) {
          this->write(ArrayString(type));
        }
        return;
      }

      default: break;
    }

    std::string fname = this->getFunctionName(aggregateNode);
    if (!fname.empty()) {
      this->write(fname);
      return;
    }
  }

  this->write(GetOperatorString(node->getOp()));
}

void SpglslAngleWebglOutput::writeConstantUnionSingleValue(const sh::TConstantUnion * value,
    bool needsParentheses,
    bool needsFloat) {
  switch (value->getType()) {
    case sh::EbtInt: this->write(int32ToGlsl(value->getIConst())); break;
    case sh::EbtUInt: this->write(uint32ToGlsl(value->getUConst())); break;
    case sh::EbtBool: this->write(value->getBConst() ? "true" : "false"); break;
    case sh::EbtYuvCscStandardEXT: out << getYuvCscStandardEXTString(value->getYuvCscStandardEXTConst()); break;
    default: this->write(floatToGlsl(value->getFConst(), needsParentheses, needsFloat)); break;
  }
}

const sh::TConstantUnion * SpglslAngleWebglOutput::writeConstantUnion(const sh::TType * type,
    const sh::TConstantUnion * pConstUnion,
    bool needsParentheses) {
  if (!type) {
    return nullptr;
  }

  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    this->write(this->getSymbolName(structure));
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
  if (size == 1) {
    this->writeConstantUnionSingleValue(pConstUnion, needsParentheses, true);
    return pConstUnion + 1;
  }

  this->write(this->getTypeName(type));
  this->write('(');

  if (type->isVector()) {
    // Check if we have a vector whose input values are all the same, vec4(0,0,0,0) => vec4(0)
    bool isAllSameValue = true;
    for (size_t i = 1; i < size; ++i) {
      if (pConstUnion[0] != pConstUnion[i]) {
        isAllSameValue = false;
      }
    }
    if (isAllSameValue) {
      this->writeConstantUnionSingleValue(pConstUnion, needsParentheses, false);
      this->write(')');
      return pConstUnion + size;
    }
  }

  if (type->isMatrix() && type->getNominalSize() == type->getSecondarySize()) {
    // If the the matrix is square and is a multiple of the identity matrix we can use a single value.
    bool isIdentityMultiple = true;
    for (int i = 0, msize = type->getNominalSize(); i < msize; i++) {
      for (int j = 0; j < msize; j++) {
        const sh::TConstantUnion & cell = pConstUnion[i * msize + j];
        if ((i == j && cell != *pConstUnion) || (i != j && !cell.isZero())) {
          isIdentityMultiple = false;
          break;
        }
      }
    }
    if (isIdentityMultiple) {
      this->writeConstantUnionSingleValue(pConstUnion, needsParentheses, false);
      this->write(')');
      return pConstUnion + size;
    }
  }

  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    if (i != 0) {
      this->writeComma();
    }
    this->writeConstantUnionSingleValue(pConstUnion, needsParentheses, false);
  }

  this->write(')');
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
  if (hasQualifier &&
      (!isFunctionArgument ||
          (qualifier != sh::TQualifier::EvqVertexIn && qualifier != sh::TQualifier::EvqFragmentIn &&
              qualifier != sh::TQualifier::EvqParamIn))) {
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
    this->write(this->getTypeName(&type));
  }
}

void SpglslAngleWebglOutput::declareInterfaceBlock(const sh::TInterfaceBlock & interfaceBlock) {
  this->write(this->getSymbolName(&interfaceBlock));
  this->write('{').indent().beautyNewLine();
  for (const auto * field : interfaceBlock.fields()) {
    const auto * type = field->type();
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
      this->write(this->getTypeName(type));
      this->write(this->getFieldName(field));
      this->write(sh::ArrayString(*type));
      this->writeStatementSemicolon();
    }
  }
  this->deindent().write('}');
}

void SpglslAngleWebglOutput::declareStruct(const sh::TStructure & structure) {
  this->write("struct");
  this->write(this->getSymbolName(&structure));
  this->write('{').indent().beautyNewLine();
  for (const auto & field : structure.fields()) {
    auto * fieldType = field->type();
    if (fieldType) {
      this->writeTypePrecision(*fieldType);
      this->write(this->getTypeName(fieldType));
      this->write(this->getFieldName(field));
      this->write(sh::ArrayString(*fieldType));
      this->writeStatementSemicolon();
    }
  }
  this->deindent().write('}');
  if (!this->symbols.getName(&structure).empty()) {
    this->declaredStructs.emplace(&structure);
  }
}

bool SpglslAngleWebglOutput::needsToClearLastWrittenVarDecl() {
  switch (this->getLastOrLastLastCh()) {
    case ',':
    case '{':
    case '[':
    case '(':
    case ';': return true;
    default: return false;
  }
}

void SpglslAngleWebglOutput::clearLastWrittenVarDecl() {
  this->_canForwardVarDecl = false;
  if (this->_lastWrittenVarDecl) {
    this->_lastWrittenVarDecl = nullptr;
    switch (this->getLastOrLastLastCh()) {
      case ',':
      case '{':
      case '[':
      case '(':
      case ';': break;
      default: {
        this->writeStatementSemicolon();
        break;
      }
    }
  }
}

void SpglslAngleWebglOutput::writeVariableDeclaration(sh::TIntermNode & child) {
  sh::TIntermSymbol * childSym = child.getAsSymbolNode();
  if (!childSym) {
    this->traverseNode(&child);
    return;
  }
  const sh::TVariable & variable = childSym->variable();
  const sh::TType & type = variable.getType();

  bool needsToWriteType = true;

  if (this->_isInsideForInit) {
    needsToWriteType = this->_lastWrittenVarDecl == nullptr;
  } else {
    auto q = type.getQualifier();
    if (this->_canForwardVarDecl && this->_lastWrittenVarDecl && type == *this->_lastWrittenVarDecl &&
        q == this->_lastWrittenVarDecl->getQualifier() && !this->needsToClearLastWrittenVarDecl()) {
      needsToWriteType = false;
    }
    if (!needsToWriteType && type.getBasicType() == sh::EbtStruct && type.getStruct() &&
        this->declaredStructs.count(type.getStruct()) == 0 && this->getSymbolName(type.getStruct()).length() != 0) {
      needsToWriteType = true;
    }
  }

  if (!needsToWriteType && SpglslGlslWriter::needsToWriteTTypeLayoutQualifier(type)) {
    needsToWriteType = true;
  }

  if (needsToWriteType) {
    this->clearLastWrittenVarDecl();
    if (variable.name() != "gl_ClipDistance") {
      this->writeTTypeLayoutQualifier(childSym->getType());
    }
    this->writeVariableType(type, false);
  } else {
    this->writeComma();
  }

  const auto & name = this->getSymbolName(&variable);
  this->write(name).write(sh::ArrayString(type));

  this->_lastWrittenVarDecl = &type;
  this->_canForwardVarDecl = !name.empty();
}

void SpglslAngleWebglOutput::visitSymbol(sh::TIntermSymbol * node) {
  this->write(this->getSymbolName(&node->variable()));
}

void SpglslAngleWebglOutput::visitConstantUnion(sh::TIntermConstantUnion * node) {
  sh::TIntermBinary * parentBinary = nodeGetAsBinaryNode(this->getParentNode());
  bool canSkipParentheses = parentBinary != nullptr && parentBinary->getOp() == sh::EOpAssign;
  this->writeConstantUnion(&node->getType(), node->getConstantValue(), !canSkipParentheses);
}

void SpglslAngleWebglOutput::visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) {
  this->clearLastWrittenVarDecl();
  this->writeDirective(node->getDirective(), node->getCommand().data());
}

bool SpglslAngleWebglOutput::visitSwizzle(sh::Visit visit, sh::TIntermSwizzle * node) {
  if (visit == sh::PreVisit) {
    if (node->getOperand() && childNodeNeedsParentheses(*node, *node->getOperand(), 0)) {
      this->write("(");
    }
  }
  if (visit == sh::PostVisit) {
    if (node->getOperand() && childNodeNeedsParentheses(*node, *node->getOperand(), 0)) {
      this->write(")");
    }
    this->writeSwizzle(node);
  }
  return true;
}

bool SpglslAngleWebglOutput::visitBinary(sh::Visit visit, sh::TIntermBinary * node) {
  switch (node->getOp()) {
    case sh::EOpIndexDirect:
    case sh::EOpIndexIndirect:
      this->traverseWithParentheses(node, 0);
      this->write('[');
      this->traverseNode(node->getRight());
      this->write(']');
      return false;

    case sh::EOpIndexDirectStruct: {
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

    case sh::EOpIndexDirectInterfaceBlock: {
      if (visit == sh::PreVisit) {
        const sh::TInterfaceBlock * iface = node->getLeft() ? node->getLeft()->getType().getInterfaceBlock() : nullptr;
        if (iface) {
          sh::TIntermSymbol * symLeft = nodeGetAsSymbolNode(node->getLeft());
          if (!symLeft || !this->getSymbolName(&symLeft->variable()).empty()) {
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

  if (node->getOp() == sh::EOpInitialize) {
    this->writeVariableDeclaration(*node->getLeft());
  } else {
    this->traverseWithParentheses(node, 0);
  }
  if (node->getOp() != sh::EOpComma) {
    this->beautySpace();
  }
  this->writeTOperatorNode(node);
  this->beautySpace();
  this->traverseWithParentheses(node, 1);
  return false;
}

bool SpglslAngleWebglOutput::visitUnary(sh::Visit visit, sh::TIntermUnary * node) {
  auto op = node->getOp();
  if (!opIsBuiltinUnaryFunction(op)) {
    if (op == sh::EOpPostDecrement || op == sh::EOpPostIncrement || op == sh::EOpArrayLength) {
      this->traverseWithParentheses(node, 0);
      this->writeTOperatorNode(node);
    } else {
      this->writeTOperatorNode(node);
      this->traverseWithParentheses(node, 0);
    }
    return false;
  }
  if (visit == sh::PreVisit) {
    this->writeTOperatorNode(node);
    this->write('(');
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
  this->clearLastWrittenVarDecl();
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

void SpglslAngleWebglOutput::beforeVisitFunctionDefinition(sh::TIntermFunctionDefinition * node) {
  this->clearLastWrittenVarDecl();
  this->beautyDoubleNewLine();
}

void SpglslAngleWebglOutput::beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
    sh::TIntermFunctionDefinition * definition) {
  this->clearLastWrittenVarDecl();
  const sh::TType & type = node->getType();
  this->writeVariableType(type, false);
  this->write(sh::ArrayString(type));
  const auto * proto = node->getFunction();
  this->write(this->getSymbolName(proto)).write('(');
}

void SpglslAngleWebglOutput::afterVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
    sh::TIntermFunctionDefinition * definition) {
  const auto * fun = node->getFunction();
  size_t paramCount = fun->getParamCount();
  for (size_t i = 0; i < paramCount; ++i) {
    const sh::TVariable * param = fun->getParam(i);
    const sh::TType & paramType = param->getType();
    if (i != 0) {
      this->writeComma();
    }
    this->writeVariableType(paramType, true);
    if (definition) {
      this->write(this->getSymbolName(param));
    }
    this->write(sh::ArrayString(paramType));
  }
  this->write(')');
}

void SpglslAngleWebglOutput::onVisitBlock(sh::TIntermBlock * node) {
  this->clearLastWrittenVarDecl();
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
        bool isVarDecl = child->getAsDeclarationNode() != nullptr;
        if (!isVarDecl) {
          this->clearLastWrittenVarDecl();
        }

        if (((prev && prev->getAsCaseNode()) || child->getAsBlock()) &&
            !nodeBlockContainsSomeSortOfDeclaration(child)) {
          if (child->getAsBlock()) {
            this->_skipNextBlockBraces = true;
          }
          this->beautyNewLine().indent();
          this->traverseNode(child);
          if (isIntermNodeSingleStatement(child) && (!isVarDecl)) {
            this->writeStatementSemicolon();
          }
          this->deindent().beautyDoubleNewLine();
        } else {
          this->beautyStatementNewLine(true);
          this->traverseNode(child);
          if (isIntermNodeSingleStatement(child) && (!isVarDecl)) {
            this->writeStatementSemicolon();
          }
        }
      }
      prev = child;
    }
  }
  if (!skipBlockBraces) {
    this->clearLastWrittenVarDecl();
    this->setIndentLevel(initialIndentLevel);
    this->beautyStatementNewLine();
    this->write('}');
  }
}

void SpglslAngleWebglOutput::onVisitForLoop(sh::TIntermLoop * node, bool infinite) {
  this->clearLastWrittenVarDecl();
  this->write("for").beautySpace().write('(');
  ++this->_isInsideForInit;
  this->traverseNode(node->getInit());
  --this->_isInsideForInit;
  this->write(';');
  this->clearLastWrittenVarDecl();
  if (!infinite) {
    this->traverseNode(node->getCondition());
  }
  this->write(';');
  if (nodeHasSideEffects(node->getExpression())) {
    this->traverseNode(node->getExpression());
  }
  this->write(')').beautySpace();
  this->traverseCodeBlock(node->getBody(), true);
  this->beautyDoubleNewLine();
}

void SpglslAngleWebglOutput::onVisitWhileLoop(sh::TIntermLoop * node) {
  this->clearLastWrittenVarDecl();
  this->write("while").beautySpace().write('(');
  this->traverseNode(node->getCondition());
  this->write(')').beautySpace();
  this->traverseCodeBlock(node->getBody(), true);
  this->beautyDoubleNewLine();
}

void SpglslAngleWebglOutput::onVisitDoWhileLoop(sh::TIntermLoop * node) {
  this->clearLastWrittenVarDecl();
  this->write("do").beautySpace();
  this->traverseCodeBlock(node->getBody(), true);
  this->write("while").beautySpace().write('(');
  this->traverseNode(node->getCondition());
  this->write(')').write(';');
  this->beautyDoubleNewLine();
}

bool SpglslAngleWebglOutput::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (visit == sh::PreVisit) {
    this->writeTOperatorNode(node);
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
    this->beautyNewLine().indent();
    this->clearLastWrittenVarDecl();
    this->writeStatementSemicolon().deindent();
  } else {
    this->traverseNode(node);
  }
}

bool SpglslAngleWebglOutput::visitSwitch(sh::Visit visit, sh::TIntermSwitch * node) {
  if (visit == sh::PreVisit) {
    this->clearLastWrittenVarDecl();
    this->write("switch").beautySpace().write('(');
  } else if (visit == sh::InVisit) {
    this->write(')').beautySpace();
  }
  return true;
}

bool SpglslAngleWebglOutput::visitCase(sh::Visit visit, sh::TIntermCase * node) {
  if (visit == sh::PreVisit) {
    this->clearLastWrittenVarDecl();
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

bool SpglslAngleWebglOutput::visitGlobalQualifierDeclaration(sh::Visit visit,
    sh::TIntermGlobalQualifierDeclaration * node) {
  this->clearLastWrittenVarDecl();
  this->beautyStatementNewLine(true);
  this->write(node->isPrecise() ? "precise" : "invariant").write(this->getSymbolName(&node->getSymbol()->variable()));
  return false;
}

bool SpglslAngleWebglOutput::visitVariableDeclaration(sh::TIntermNode * node,
    sh::TIntermDeclaration * declarationNode) {
  this->writeVariableDeclaration(*node);
  return false;
}

void SpglslAngleWebglOutput::traverseCodeBlock(sh::TIntermBlock * body, bool allowIf) {
  this->clearLastWrittenVarDecl();
  if (nodeBlockIsEmpty(body)) {
    this->beautyNewLine().indent().writeStatementSemicolon().deindent();
  } else {
    sh::TIntermNode * singleNode = nodeGetBlockSingleNode(body);
    if (!this->beautify && singleNode && (allowIf || !singleNode->getAsIfElseNode()) &&
        !nodeIsSomeSortOfDeclaration(singleNode)) {
      this->traverseNode(singleNode);
      if (isIntermNodeSingleStatement(singleNode)) {
        this->writeStatementSemicolon();
      }
    } else {
      this->traverseNode(body);
    }
  }
  this->clearLastWrittenVarDecl();
}

bool SpglslAngleWebglOutput::visitBranch(sh::Visit visit, sh::TIntermBranch * node) {
  if (visit == sh::PreVisit) {
    this->clearLastWrittenVarDecl();

    const char * ops;
    switch (node->getFlowOp()) {
      case sh::EOpKill: ops = "discard"; break;
      case sh::EOpBreak: ops = "break"; break;
      case sh::EOpContinue: ops = "continue"; break;
      case sh::EOpReturn: ops = "return"; break;
      default: ops = GetOperatorString(node->getFlowOp()); break;
    }
    this->write(ops);
    if (node->getChildCount() > 0) {
      this->beautySpace();
    }
  }
  return true;
}

void SpglslAngleWebglOutput::traverseWithParentheses(sh::TIntermNode * node, int operandIndex) {
  if (node) {
    sh::TIntermNode * child = node->getChildNode(operandIndex);
    if (child) {
      if (childNodeNeedsParentheses(*node, *child, operandIndex)) {
        this->write('(');
        this->traverse(child);
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
  this->clearLastWrittenVarDecl();
  if (shaderVersion > 100) {
    std::string tmp = std::to_string(shaderVersion) + " es";
    this->writeDirective("#version", tmp.c_str());
    this->beautyDoubleNewLine();
  }

  for (auto iter = extBehavior.begin(); iter != extBehavior.end(); ++iter) {
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
  }
  if (pragma.stdgl.invariantAll) {
    this->writeDirective("#pragma", "STDGL invariant(all)");
  }
  this->beautyDoubleNewLine();

  bool hasVariables = false;
  for (const auto & iter : this->symbols._map) {
    if (!iter.second.symbolName.empty()) {
      hasVariables = true;
      break;
    }
  }

  if (hasVariables) {
    if (this->precisions.floatPrecision != sh::EbpUndefined &&
        this->precisions.floatPrecision != this->precisions.defaultFloatPrecision) {
      this->write("precision")
          .write(sh::getPrecisionString(this->precisions.floatPrecision))
          .write("float")
          .write(';')
          .beautyNewLine();
    }

    if (this->precisions.intPrecision != sh::EbpUndefined &&
        this->precisions.intPrecision != this->precisions.defaultIntPrecision) {
      this->write("precision")
          .write(sh::getPrecisionString(this->precisions.intPrecision))
          .write("int")
          .write(';')
          .beautyNewLine();
    }
  }

  this->beautyDoubleNewLine();
}
