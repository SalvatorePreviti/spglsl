#include "spglsl-angle-node-utils.h"

#include <angle/src/compiler/translator/Symbol.h>

#include "../../core/math-utils.h"

bool opIsBuiltinUnaryFunction(sh::TOperator op) {
  switch (op) {
    case sh::EOpNegative:
    case sh::EOpPositive:
    case sh::EOpLogicalNot:
    case sh::EOpBitwiseNot:
    case sh::EOpPostIncrement:
    case sh::EOpPostDecrement:
    case sh::EOpPreIncrement:
    case sh::EOpPreDecrement:
    case sh::EOpArrayLength: return false;
    default: return true;
  }
}

bool nodeIsConstantZero(sh::TIntermNode * node) {
  sh::TIntermTyped * typed = node->getAsTyped();
  if (!typed) {
    return false;
  }

  switch (typed->getType().getBasicType()) {
    case sh::EbtFloat:
    case sh::EbtDouble:
    case sh::EbtInt:
    case sh::EbtUInt:
    case sh::EbtBool: break;
    default: return false;
  }

  const sh::TConstantUnion * value = typed->getConstantValue();
  if (!value) {
    return false;
  }
  int size = typed->getNominalSize() * typed->getSecondarySize();
  for (int i = 0; i < size; ++i) {
    if (!value[i].isZero()) {
      return false;
    }
  }
  return true;
}

int nodeConstantBooleanValue(sh::TIntermNode * node) {
  if (!node) {
    return -1;
  }
  auto * typed = node->getAsTyped();
  if (!typed) {
    return -1;
  }

  if (typed->getType().getBasicType() != sh::EbtBool) {
    return -1;  // Not a boolean.
  }

  const sh::TConstantUnion * asConstanUnion = typed->getConstantValue();
  if (asConstanUnion) {
    int size = typed->getNominalSize() * typed->getSecondarySize();
    if (size != 1) {
      return -1;
    }
    return asConstanUnion[0].isZero() ? 0 : 1;
  }

  auto * asBinary = typed->getAsBinaryNode();
  if (asBinary) {
    switch (asBinary->getOp()) {
      case sh::EOpLogicalAnd: {
        auto va = nodeConstantBooleanValue(asBinary->getLeft());
        auto vb = nodeConstantBooleanValue(asBinary->getRight());
        if (va >= 0 && vb >= 0) {
          return va == 1 && vb == 1;  // two constants
        }
        if (va == 0) {
          return 0;  // false && something => false
        }
        if (!asBinary->getLeft()->hasSideEffects() && vb == 0) {
          return 0;  // something that has no side effects && false => false
        }
        return -1;
      }

      case sh::EOpLogicalOr: {
        auto va = nodeConstantBooleanValue(asBinary->getLeft());
        auto vb = nodeConstantBooleanValue(asBinary->getRight());
        if (va >= 0 && vb >= 0) {
          return va == 1 || vb == 1;  // two constants
        }
        if (va == 1 && !asBinary->getRight()->hasSideEffects()) {
          return 1;  // true || something that has no side effect => true
        }
        if (!asBinary->getLeft()->hasSideEffects() && vb == 1) {
          return 1;  // something that has no side effects && true => true
        }
        return -1;
      }

      case sh::EOpLogicalXor: {
        auto va = nodeConstantBooleanValue(asBinary->getLeft());
        auto vb = nodeConstantBooleanValue(asBinary->getRight());
        if (va >= 0 && vb >= 0)
          return va != vb ? 1 : 0;  // two constants
        return -1;
      }

      default: return -1;
    }
  } else {
    auto * asUnary = typed->getAsUnaryNode();
    if (asUnary) {
      switch (asUnary->getOp()) {
        case sh::EOpLogicalNot: {
          auto v = nodeConstantBooleanValue(asUnary->getOperand());
          if (v >= 0)
            return v ? 0 : 1;  // boolean not of a constant
          return -1;
        }

        default: return -1;
      }
    }
  }

  return -1;
}

bool nodeBlockIsEmpty(sh::TIntermNode * node) {
  if (!node) {
    return true;
  }
  sh::TIntermBlock * block = node->getAsBlock();
  if (!block) {
    return false;
  }
  const sh::TIntermSequence & sequence = *block->getSequence();
  if (sequence.empty()) {
    return true;
  }
  for (size_t i = 0; i < sequence.size(); ++i) {
    if (!nodeBlockIsEmpty(sequence[i])) {
      return false;
    }
  }
  return true;
}

sh::TIntermNode * nodeGetBlockSingleNode(sh::TIntermNode * node) {
  if (!node) {
    return nullptr;
  }
  sh::TIntermBlock * block = node->getAsBlock();
  if (!block) {
    return node;
  }

  const sh::TIntermSequence & sequence = *block->getSequence();

  sh::TIntermNode * result = nullptr;
  for (size_t i = 0; i < sequence.size(); ++i) {
    if (!nodeBlockIsEmpty(sequence[i])) {
      if (result) {
        return block;
      }
      result = nodeGetBlockSingleNode(sequence[i]);
      if (nodeBlockContainsSomeSortOfDeclaration(result)) {
        return block;
      }
    }
  }
  return result;
}

bool nodeHasSideEffects(sh::TIntermNode * node) {
  if (!node) {
    return false;
  }
  sh::TIntermTyped * asTyped = node->getAsTyped();
  if (asTyped && !asTyped->getAsFunctionPrototypeNode()) {
    return asTyped->hasSideEffects();
  }
  return true;  // Let's be conservative.
}

bool nodeIsSomeSortOfDeclaration(sh::TIntermNode * node) {
  if (!node) {
    return false;
  }

  if (node->getAsDeclarationNode() || node->getAsGlobalQualifierDeclarationNode() || node->getAsFunctionDefinition() ||
      node->getAsFunctionPrototypeNode() || node->getAsPreprocessorDirective()) {
    return true;
  }

  sh::TIntermBinary * binaryNode = node->getAsBinaryNode();
  return binaryNode && binaryNode->getOp() == sh::EOpInitialize;
}

bool nodeBlockContainsSomeSortOfDeclaration(sh::TIntermNode * node) {
  if (!node) {
    return false;
  }

  sh::TIntermBlock * block = node->getAsBlock();
  if (block) {
    const sh::TIntermSequence * sequence = block->getSequence();
    for (sh::TIntermNode * child : *sequence) {
      if (nodeIsSomeSortOfDeclaration(child)) {
        return true;
      }
    }
    return false;
  }

  return nodeIsSomeSortOfDeclaration(node);
}

const sh::TFunction * nodeGetAsFunction(sh::TIntermNode * node) {
  if (node) {
    const sh::TIntermFunctionDefinition * asFunctionDefinition = node->getAsFunctionDefinition();
    if (asFunctionDefinition) {
      return asFunctionDefinition->getFunction();
    }
    const sh::TIntermFunctionPrototype * asFunctionPrototype = node->getAsFunctionPrototypeNode();
    if (asFunctionPrototype) {
      return asFunctionPrototype->getFunction();
    }
  }
  return nullptr;
}

const sh::TConstantUnion * _constantUnionIsAllZero(const sh::TType * type,
    const sh::TConstantUnion * pConstUnion,
    bool & result) {
  if (!type || !result || !pConstUnion) {
    return nullptr;
  }
  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    const sh::TFieldList & fields = structure->fields();
    for (size_t i = 0, size = fields.size(); pConstUnion && i < size; ++i) {
      pConstUnion = _constantUnionIsAllZero(fields[i]->type(), pConstUnion, result);
    }
    return pConstUnion;
  }
  size_t size = type->getObjectSize();
  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    if (!pConstUnion->isZero()) {
      result = false;
      return nullptr;
    }
  }
  return pConstUnion;
}

const sh::TConstantUnion * _constantUnionIsAllOne(const sh::TType * type,
    const sh::TConstantUnion * pConstUnion,
    bool & result) {
  if (!type || !result || !pConstUnion) {
    return nullptr;
  }
  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    const sh::TFieldList & fields = structure->fields();
    for (size_t i = 0, size = fields.size(); pConstUnion && i < size; ++i) {
      pConstUnion = _constantUnionIsAllOne(fields[i]->type(), pConstUnion, result);
    }
    return pConstUnion;
  }
  size_t size = type->getObjectSize();
  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    bool isOne;
    switch (type->getBasicType()) {
      case sh::EbtInt: isOne = pConstUnion->getIConst() == 1; break;
      case sh::EbtUInt: isOne = pConstUnion->getUConst() == 1; break;
      case sh::EbtFloat: isOne = pConstUnion->getFConst() == 1.0f; break;
      case sh::EbtBool: isOne = pConstUnion->getBConst(); break;
      default: isOne = false;
    }

    if (!isOne) {
      result = false;
      return nullptr;
    }
  }
  return pConstUnion;
}

sh::TConstantUnion * _writeConstantUnionFromScalar(const sh::TType * type,
    sh::TConstantUnion * pConstUnion,
    const sh::TConstantUnion & constant) {
  if (!type) {
    return nullptr;
  }
  const sh::TStructure * structure = type->getBasicType() == sh::EbtStruct ? type->getStruct() : nullptr;
  if (structure) {
    const sh::TFieldList & fields = structure->fields();
    for (size_t i = 0, size = fields.size(); i < size; ++i) {
      pConstUnion = _writeConstantUnionFromScalar(fields[i]->type(), pConstUnion, constant);
    }
    return pConstUnion;
  }
  size_t size = type->getObjectSize();
  for (size_t i = 0; i < size; ++i, ++pConstUnion) {
    switch (type->getBasicType()) {
      case sh::EbtFloat:
        switch (constant.getType()) {
          case sh::EbtVoid: pConstUnion->setFConst(0.0f); break;
          case sh::EbtFloat:
          case sh::EbtInt:
          case sh::EbtUInt: pConstUnion->setFConst(constant.getFConst()); break;
          case sh::EbtBool: pConstUnion->setFConst(constant.getBConst() ? 1.0f : 0.0f); break;
          case sh::EbtYuvCscStandardEXT:
            pConstUnion->setFConst((float)(int)constant.getYuvCscStandardEXTConst());
            break;
          default: break;
        }
        break;

      case sh::EbtInt:
        switch (constant.getType()) {
          case sh::EbtVoid: pConstUnion->setIConst(0); break;
          case sh::EbtInt: pConstUnion->setIConst(constant.getIConst()); break;
          case sh::EbtFloat:
            pConstUnion->setIConst(floatIsNaN(constant.getFConst()) ? 0 : (int)constant.getFConst());
            break;
          case sh::EbtUInt: pConstUnion->setIConst((int)constant.getUConst()); break;
          case sh::EbtBool: pConstUnion->setIConst(constant.getBConst() ? -1 : 0); break;
          case sh::EbtYuvCscStandardEXT: pConstUnion->setIConst((int)constant.getYuvCscStandardEXTConst()); break;
          default: break;
        }
        break;

      case sh::EbtUInt:
        switch (constant.getType()) {
          case sh::EbtVoid: pConstUnion->setUConst(0u); break;
          case sh::EbtInt: pConstUnion->setUConst((unsigned int)constant.getIConst()); break;
          case sh::EbtFloat:
            pConstUnion->setUConst(floatIsNaN(constant.getFConst()) ? 0u : (unsigned int)constant.getFConst());
            break;
          case sh::EbtUInt: pConstUnion->setUConst(constant.getUConst()); break;
          case sh::EbtBool: pConstUnion->setUConst(constant.getBConst() ? 0xffffffffu : 0u); break;
          case sh::EbtYuvCscStandardEXT:
            pConstUnion->setUConst((unsigned int)constant.getYuvCscStandardEXTConst());
            break;
          default: break;
        }
        break;

      case sh::EbtBool:
        switch (constant.getType()) {
          case sh::EbtVoid: pConstUnion->setBConst(false); break;
          case sh::EbtInt: pConstUnion->setBConst(constant.getIConst() != 0); break;
          case sh::EbtFloat:
            pConstUnion->setBConst(floatIsNaN(constant.getFConst()) ? false : constant.getFConst() != 0.0f);
            break;
          case sh::EbtUInt: pConstUnion->setBConst(constant.getUConst() != 0); break;
          case sh::EbtBool: pConstUnion->setBConst(constant.getBConst()); break;
          case sh::EbtYuvCscStandardEXT:
            pConstUnion->setBConst((unsigned int)constant.getYuvCscStandardEXTConst());
            break;
          default: break;
        }
        break;

      default: pConstUnion->cast(type->getBasicType(), constant); break;
    }
  }
  return pConstUnion;
}

sh::TConstantUnion * CreateConstantUnionFillFromScalar(const sh::TType & type, const sh::TConstantUnion & constant) {
  size_t size = type.getObjectSize();
  auto * result = new sh::TConstantUnion[size];
  _writeConstantUnionFromScalar(&type, result, constant);
  return result;
}

sh::TIntermConstantUnion * nodeCreateConstantUnionFillFromScalar(const sh::TType & type,
    const sh::TConstantUnion & constant) {
  return new sh::TIntermConstantUnion(CreateConstantUnionFillFromScalar(type, constant), type);
}

bool nodeConstantUnionIsAllZero(sh::TIntermNode * node) {
  auto * constantUnion = node ? node->getAsConstantUnion() : nullptr;
  if (!constantUnion) {
    return false;
  }
  bool result = true;
  _constantUnionIsAllZero(&constantUnion->getType(), constantUnion->getConstantValue(), result);
  return result;
}

bool nodeConstantUnionIsAllOne(sh::TIntermNode * node) {
  auto * constantUnion = node ? node->getAsConstantUnion() : nullptr;
  if (!constantUnion || !constantUnion->getConstantValue()) {
    return false;
  }
  bool result = true;
  _constantUnionIsAllOne(&constantUnion->getType(), constantUnion->getConstantValue(), result);
  return result;
}

bool isIntermNodeSingleStatement(sh::TIntermNode * node) {
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

AngleNodeKind nodeGetKind(const sh::TIntermNode * node) {
  if (node == nullptr) {
    return AngleNodeKind::TNull;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsAggregate()) {
    return AngleNodeKind::TIntermAggregate;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsBlock()) {
    return AngleNodeKind::TIntermBlock;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsGlobalQualifierDeclarationNode()) {
    return AngleNodeKind::TIntermGlobalQualifierDeclaration;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsDeclarationNode()) {
    return AngleNodeKind::TIntermDeclaration;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsFunctionPrototypeNode()) {
    return AngleNodeKind::TIntermDeclaration;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsFunctionDefinition()) {
    return AngleNodeKind::TIntermFunctionDefinition;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsSwizzleNode()) {
    return AngleNodeKind::TIntermSwizzle;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsBinaryNode()) {
    return AngleNodeKind::TIntermBinary;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsUnaryNode()) {
    return AngleNodeKind::TIntermUnary;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsConstantUnion()) {
    return AngleNodeKind::TIntermConstantUnion;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsTernaryNode()) {
    return AngleNodeKind::TIntermTernary;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsIfElseNode()) {
    return AngleNodeKind::TIntermIfElse;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsSwitchNode()) {
    return AngleNodeKind::TIntermSwitch;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsCaseNode()) {
    return AngleNodeKind::TIntermCase;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsSymbolNode()) {
    return AngleNodeKind::TIntermSymbol;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsLoopNode()) {
    return AngleNodeKind::TIntermLoop;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsBranchNode()) {
    return AngleNodeKind::TIntermBranch;
  }

  if (const_cast<sh::TIntermNode *>(node)->getAsPreprocessorDirective()) {
    return AngleNodeKind::TIntermPreprocessorDirective;
  }

  return AngleNodeKind::TNull;
}

const char * AngleNodeKind_name(AngleNodeKind kind) {
  switch (kind) {
    case AngleNodeKind::TNull: return "TNull";
    case AngleNodeKind::TIntermAggregate: return "TIntermAggregate";
    case AngleNodeKind::TIntermBlock: return "TIntermBlock";
    case AngleNodeKind::TIntermGlobalQualifierDeclaration: return "TIntermGlobalQualifierDeclaration";
    case AngleNodeKind::TIntermDeclaration: return "TIntermDeclaration";
    case AngleNodeKind::TIntermFunctionPrototype: return "TIntermFunctionPrototype";
    case AngleNodeKind::TIntermFunctionDefinition: return "TIntermFunctionDefinition";
    case AngleNodeKind::TIntermSwizzle: return "TIntermSwizzle";
    case AngleNodeKind::TIntermBinary: return "TIntermBinary";
    case AngleNodeKind::TIntermUnary: return "TIntermUnary";
    case AngleNodeKind::TIntermConstantUnion: return "TIntermConstantUnion";
    case AngleNodeKind::TIntermTernary: return "TIntermTernary";
    case AngleNodeKind::TIntermIfElse: return "TIntermIfElse";
    case AngleNodeKind::TIntermSwitch: return "TIntermSwitch";
    case AngleNodeKind::TIntermCase: return "TIntermCase";
    case AngleNodeKind::TIntermSymbol: return "TIntermSymbol";
    case AngleNodeKind::TIntermLoop: return "TIntermLoop";
    case AngleNodeKind::TIntermBranch: return "TIntermBranch";
    case AngleNodeKind::TIntermPreprocessorDirective: return "TIntermPreprocessorDirective";

    default: return "TIntermNode_Unknown";
  }
}
