#include "spglsl-angle-ast-hasher.h"
#include "spglsl-angle-node-utils.h"
#include "spglsl-angle-operator-precedence.h"

#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/Types.h>
#include "spglsl-glsl-writer.h"

using namespace sh;

enum WriteHeaders {
  SWIZZLE,
  DIRECTIVE,
  MEMORYQUALIFIER,
  LAYOUT,
  CONSTANT_UNION_STRUCT,
  CONSTANT_UNION_VALUE,
  VARIABLE_TYPE,
  FUNCTION_PROTOTYPE,
  SQUAREDBRACKET,
  DOTOPERATOR,
  BINARYOPERATOR,
  TERNARYOPERATOR,
  IFELSEBLOCK,
  AGGREGATE,
  CODEBLOCK,
  SWITCH,
  BLOCK,
  GLOBALQUALIFIERPRECISE,
  GLOBALQUALIFIERINVARIANT,
  DECLARATION,
  CODEBLOCK_ALLOWIF_TRUE,
  CODEBLOCK_ALLOWIF_FALSE,
  LOOPFOR,
  LOOPWHILE,
  LOOPDO,
  BRANCH,
  PARENS,
  NOPARENS,
  UNARYOPERATOR_BULTIN,
  UNARYOPERATOR,
  SYMBOLREF,
  TYPEREF_BUILTIN,
  TYPEREF_STRUCT,
  TYPEREF_IFACE
};

AngleAstHasher::AngleAstHasher(TSymbolTable * symbolTable) : sh::TIntermTraverser(true, true, true, symbolTable) {
}

SpglslHashValue AngleAstHasher::computeNodeHash(sh::TIntermNode * root) {
  this->resetHash();
  this->traverseNode(root);
  return this->digest();
}

SpglslHashValue & AngleAstHasher::computeNodeHash(sh::TIntermNode * root, SpglslHashValue & output) {
  this->resetHash();
  this->traverseNode(root);
  return this->digest(output);
}

bool AngleAstHasher::computeNodeHashChanged(sh::TIntermNode * root, SpglslHashValue & hashValue) {
  this->resetHash();
  this->traverseNode(root);
  return this->digestChanged(hashValue);
}

bool AngleAstHasher::nodesAreTheSame(sh::TIntermNode * a, sh::TIntermNode * b) {
  if (a == b) {
    return true;
  }
  if (!a || !b) {
    return false;
  }
  if (a->getChildCount() != b->getChildCount()) {
    return false;
  }
  return this->computeNodeHash(a) == this->computeNodeHash(b);
}

AngleAstHasher & AngleAstHasher::writeDirective(sh::PreprocessorDirective directive, const char * command) {
  this->begin(DIRECTIVE).write(command).end();
  return *this;
}

AngleAstHasher & AngleAstHasher::writeTMemoryQualifier(const sh::TMemoryQualifier & q) {
  this->begin(MEMORYQUALIFIER).writeStruct(q).end();
  return *this;
}

AngleAstHasher & AngleAstHasher::writeTTypeLayoutQualifier(const sh::TType & type) {
  this->begin(LAYOUT);
  if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    auto blockStorage = type.getInterfaceBlock()->blockStorage();
    bool hasBlockStorage = blockStorage != sh::EbsUnspecified && blockStorage != sh::EbsStd140;
    this->begin(LAYOUT);
    this->write(hasBlockStorage ? blockStorage : 0);
    this->write(type.getInterfaceBlock()->blockBinding());
  } else {
    SpglslAngleLayoutNeeds needs(type);
    if (needs.needsToWriteLayout) {
      this->write(needs.needsToWriteLocation ? (int)needs.layoutQualifier.location : -1);
      this->write(needs.needsToWriteBinding ? (int)needs.layoutQualifier.binding : -1);
      this->write(needs.needsToWriteIndex ? (int)needs.layoutQualifier.index : -1);
      this->write(needs.needsToWriteYuv ? '1' : '0');
      this->write(needs.needsToWriteImage ? (int)needs.layoutQualifier.imageInternalFormat : -1);
      this->write(needs.needsToWriteOffset ? (int)needs.layoutQualifier.offset : -1);
    }
  }
  this->end();
  return *this;
}

const sh::TConstantUnion * AngleAstHasher::writeConstantUnion(const sh::TType * type,
    const sh::TConstantUnion * pConstUnion) {
  if (!type) {
    return nullptr;
  }
  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    this->begin(CONSTANT_UNION_STRUCT);
    this->writeSymbolRef(*structure);
    const sh::TFieldList & fields = structure->fields();
    this->write(fields.size());
    for (size_t i = 0, size = fields.size(); i < size; ++i) {
      pConstUnion = writeConstantUnion(fields[i]->type(), pConstUnion);
    }
    this->end();
    return pConstUnion;
  }
  size_t size = type->getObjectSize();
  bool writeType = size > 1;
  this->begin(CONSTANT_UNION_VALUE);
  this->writeTypeRef(*type);
  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    switch (pConstUnion->getType()) {
      case sh::EbtFloat: this->write(pConstUnion->getFConst()); break;
      case sh::EbtInt: this->write(pConstUnion->getIConst()); break;
      case sh::EbtUInt: this->write(pConstUnion->getUConst()); break;
      case sh::EbtBool: this->write(pConstUnion->getBConst()); break;
      case sh::EbtYuvCscStandardEXT: this->write(pConstUnion->getYuvCscStandardEXTConst()); break;
      default: break;
    }
  }
  this->end();
  return pConstUnion;
}

AngleAstHasher & AngleAstHasher::writeVariableType(const sh::TType & type, bool isFunctionArgument) {
  this->begin(VARIABLE_TYPE);
  this->write(type.isInvariant()).write(type.isPrecise());
  auto qualifier = type.getQualifier();
  bool hasQualifier = qualifier != sh::EvqTemporary && qualifier != sh::EvqGlobal;
  this->write(hasQualifier ? qualifier : -1);
  if (hasQualifier || isFunctionArgument) {
    this->writeTMemoryQualifier(type.getMemoryQualifier());
  }
  if (type.getBasicType() == sh::EbtStruct && type.getStruct()) {
    this->write('0').write(type.getStruct()->uniqueId().get());
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    this->write('1').write(type.getInterfaceBlock()->uniqueId().get());
  } else {
    this->write('2').write(type.getInterfaceBlock()->uniqueId().get());
    this->write(type.getPrecision());
    this->writeTypeRef(type);
  }
  this->end();
  return *this;
}

AngleAstHasher & AngleAstHasher::writeVariableDeclarationSymbol(sh::TIntermNode & child) {
  sh::TIntermSymbol * childSym = child.getAsSymbolNode();
  if (!childSym) {
    this->traverseNode(&child);
    return *this;
  }
  const sh::TVariable & variable = childSym->variable();
  this->writeTTypeLayoutQualifier(childSym->getType());
  this->writeVariableType(variable.getType(), false);
  this->writeSymbolRef(variable);
  this->writeArraySizes(variable.getType());
  return *this;
}

AngleAstHasher & AngleAstHasher::writeArraySizes(const TType & type) {
  if (!type.isArray()) {
    this->write(-1);
  } else {
    const TSpan<const unsigned int> & arraySizes = type.getArraySizes();
    this->write(arraySizes.size());
    for (auto arraySizeIter = arraySizes.rbegin(); arraySizeIter != arraySizes.rend(); ++arraySizeIter) {
      this->write(*arraySizeIter);
    }
  }
  return *this;
}

void AngleAstHasher::visitSymbol(sh::TIntermSymbol * node) {
  this->writeSymbolRef(node->variable());
}

void AngleAstHasher::visitConstantUnion(sh::TIntermConstantUnion * node) {
  sh::TIntermBinary * parentBinary = nodeGetAsBinaryNode(this->getParentNode());
  this->writeConstantUnion(&node->getType(), node->getConstantValue());
}

void AngleAstHasher::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  this->begin(FUNCTION_PROTOTYPE);
  const sh::TType & type = node->getType();
  auto proto = node->getFunction();
  this->writeVariableType(type, false).writeArraySizes(type).writeSymbolRef(*proto);
  size_t paramCount = proto->getParamCount();
  for (size_t i = 0; i < paramCount; ++i) {
    const sh::TVariable * param = proto->getParam(i);
    const sh::TType & paramType = param->getType();
    this->writeVariableType(paramType, true);
    this->writeSymbolRef(*param);
    this->writeArraySizes(paramType);
  }
  this->end();
}

void AngleAstHasher::visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) {
  this->writeDirective(node->getDirective(), node->getCommand().data());
}

bool AngleAstHasher::visitSwizzle(sh::Visit visit, sh::TIntermSwizzle * node) {
  const auto & offsets = node->getSwizzleOffsets();
  this->begin(SWIZZLE).writePtr(&offsets[0], offsets.size()).end();
  return true;
}

bool AngleAstHasher::visitBinary(sh::Visit visit, sh::TIntermBinary * node) {
  switch (node->getOp()) {
    case EOpIndexDirect:
    case EOpIndexIndirect:
      this->begin(SQUAREDBRACKET);
      this->traverseWithParentheses(node, 0);
      this->traverseNode(node->getRight());
      this->end();
      return false;

    case EOpIndexDirectStruct: {
      if (visit == sh::PreVisit) {
        this->begin(DOTOPERATOR);
        const sh::TStructure * structure = node->getLeft() ? node->getLeft()->getType().getStruct() : nullptr;
        if (structure) {
          this->traverseWithParentheses(node, 0);
          sh::TIntermConstantUnion * indexNode = nodeGetAsConstantUnion(node->getRight());
          const int fieldIndex = indexNode ? indexNode->getIConst(0) : -1;
          if (fieldIndex >= 0 && fieldIndex < structure->fields().size()) {
            this->write(fieldIndex);
          } else {
            this->traverseNode(node->getRight());
          }
          this->end();
          return false;
        }
      }
      return true;
    }

    case EOpIndexDirectInterfaceBlock: {
      if (visit == sh::PreVisit) {
        this->begin(DOTOPERATOR);
        const sh::TInterfaceBlock * iface = node->getLeft() ? node->getLeft()->getType().getInterfaceBlock() : nullptr;
        if (iface) {
          this->traverseWithParentheses(node, 0);
          sh::TIntermConstantUnion * indexNode = nodeGetAsConstantUnion(node->getRight());
          const int fieldIndex = indexNode ? indexNode->getIConst(0) : -1;
          if (fieldIndex >= 0 && fieldIndex < iface->fields().size()) {
            this->write(fieldIndex);
          } else {
            this->traverseNode(node->getRight());
          }
          this->end();
          return false;
        }
      }
      return true;
    }
    default: break;
  }

  this->begin(BINARYOPERATOR);
  this->write(node->getOp());

  if (node->getOp() == EOpInitialize) {
    this->writeVariableDeclarationSymbol(*node->getLeft());
  } else {
    this->traverseWithParentheses(node, 0);
  }
  this->traverseWithParentheses(node, 1);
  this->end();
  return false;
}

bool AngleAstHasher::visitUnary(sh::Visit visit, sh::TIntermUnary * node) {
  if (!opIsBuiltinUnaryFunction(node->getOp())) {
    this->begin(UNARYOPERATOR_BULTIN);
    this->write(node->getOp());
    this->traverseWithParentheses(node, 0);
    this->end();
    return false;
  }
  if (visit == sh::PreVisit) {
    this->begin(UNARYOPERATOR);
    this->write(node->getOp());
  } else if (visit == sh::PostVisit) {
    this->end();
  }
  return true;
}

bool AngleAstHasher::visitTernary(sh::Visit visit, sh::TIntermTernary * node) {
  this->begin(TERNARYOPERATOR);
  this->traverseWithParentheses(node, 0);
  this->traverseWithParentheses(node, 1);
  this->traverseWithParentheses(node, 2);
  this->end();
  return false;
}

bool AngleAstHasher::visitIfElse(sh::Visit visit, sh::TIntermIfElse * node) {
  this->begin(IFELSEBLOCK);
  this->traverseNode(node->getCondition());
  this->traverseCodeBlock(node->getTrueBlock(), false);
  if (!nodeBlockIsEmpty(node->getFalseBlock())) {
    this->traverseCodeBlock(node->getFalseBlock(), true);
  }
  this->end();
  return false;
}

bool AngleAstHasher::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  this->traverseNode(node->getFunctionPrototype());
  this->traverseNode(node->getBody());
  return false;
}

bool AngleAstHasher::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (visit == sh::PreVisit) {
    this->begin(AGGREGATE);
    switch (node->getOp()) {
      case EOpCallInternalRawFunction:
      case EOpCallBuiltInFunction:
      case EOpCallFunctionInAST:
        this->write('.');
        this->writeSymbolRef(*node->getFunction());
        break;

      case EOpConstruct: {
        const auto & type = node->getType();
        this->write('@');
        this->writeTypeRef(type);
        this->writeArraySizes(type);
        break;
      }

      default: this->write('#').write(node->getOp()); break;
    }
  } else if (visit == sh::PostVisit) {
    this->end();
  }
  return true;
}

void AngleAstHasher::traverseCodeBlock(sh::TIntermBlock * node) {
  this->begin(CODEBLOCK);
  this->traverseNode(node);
  this->end();
}

bool AngleAstHasher::visitSwitch(sh::Visit visit, sh::TIntermSwitch * node) {
  if (visit == sh::PreVisit) {
    this->begin(SWITCH);
  } else if (visit == sh::InVisit) {
    this->write('@');
  } else {
    this->end();
  }
  return true;
}

bool AngleAstHasher::visitCase(sh::Visit visit, sh::TIntermCase * node) {
  if (visit == sh::PreVisit) {
    if (!node->getCondition()) {
      this->write('D');
      return false;
    }
    this->write('C');
  } else if (visit == sh::PostVisit) {
    this->write(':');
  }
  return true;
}

bool AngleAstHasher::visitBlock(sh::Visit visit, sh::TIntermBlock * node) {
  if (visit == sh::PreVisit) {
    this->begin(BLOCK);
  } else if (visit == sh::InVisit) {
    this->write(';');
  } else {
    this->end();
  }
  return true;
}

bool AngleAstHasher::visitGlobalQualifierDeclaration(sh::Visit visit, sh::TIntermGlobalQualifierDeclaration * node) {
  this->begin(node->isPrecise() ? GLOBALQUALIFIERPRECISE : GLOBALQUALIFIERINVARIANT);
  this->writeSymbolRef(node->getSymbol()->variable());
  this->end();
  return false;
}

bool AngleAstHasher::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  this->begin(DECLARATION);
  size_t childCount = node->getChildCount();
  this->write(childCount);
  for (size_t i = 0; i < childCount; ++i) {
    auto child = node->getChildNode(i);
    if (child) {
      this->write('#');
      if (child->getAsSymbolNode()) {
        this->writeVariableDeclarationSymbol(*child);
      } else {
        this->traverseNode(child);
      }
    }
  }
  this->end();
  return false;
}

void AngleAstHasher::traverseCodeBlock(sh::TIntermBlock * body, bool allowIf) {
  this->begin(allowIf ? CODEBLOCK_ALLOWIF_TRUE : CODEBLOCK_ALLOWIF_FALSE);
  this->traverseNode(body);
  this->end();
}

bool AngleAstHasher::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  sh::TIntermBlock * body = node->getBody();
  sh::TLoopType loopType = node->getType();
  switch (loopType) {
    case sh::ELoopFor:
      this->begin(LOOPFOR);
      this->traverseNode(node->getInit());
      this->write(-555);
      this->traverseNode(node->getCondition());
      this->write(-655);
      this->traverseNode(node->getExpression());
      this->write(-755);
      this->traverseCodeBlock(body, true);
      this->end();
      return false;

    case sh::ELoopWhile: {
      this->begin(LOOPWHILE);
      this->traverseNode(node->getCondition());
      this->write(-855);
      this->traverseCodeBlock(body, true);
      this->end();
      return false;
    }

    case sh::ELoopDoWhile: {
      this->begin(LOOPDO);
      this->traverseCodeBlock(body, true);
      this->write(-955);
      this->traverseNode(node->getCondition());
      this->end();
      return false;
    }

    default: return true;
  }
}

bool AngleAstHasher::visitBranch(sh::Visit visit, sh::TIntermBranch * node) {
  if (visit == sh::PreVisit) {
    this->begin(BRANCH);
    this->write(node->getFlowOp());
  } else if (visit == sh::PostVisit) {
    this->end();
  }
  return true;
}

AngleAstHasher & AngleAstHasher::traverseNode(sh::TIntermNode * node) {
  if (node) {
    node->traverse(this);
  }
  return *this;
}

void AngleAstHasher::traverseWithParentheses(sh::TIntermNode * node, int operandIndex) {
  if (node) {
    sh::TIntermNode * child = node->getChildNode(operandIndex);
    this->traverseNode(child);
  }
}

AngleAstHasher & AngleAstHasher::writeSymbolRef(const sh::TSymbol & symbol) {
  this->begin(SYMBOLREF);
  this->write(symbol.uniqueId().get());
  this->end();
  return *this;
}

AngleAstHasher & AngleAstHasher::writeTypeRef(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtStruct && type.getStruct()) {
    this->begin(TYPEREF_STRUCT);
    this->writeSymbolRef(*type.getStruct());
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    this->begin(TYPEREF_IFACE);
    this->writeSymbolRef(*type.getInterfaceBlock());
  } else {
    this->begin(TYPEREF_BUILTIN);
    this->write(type.getBuiltInTypeNameString());
  }
  this->end();
  return *this;
}