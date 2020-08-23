#include "spglsl-angle-node-utils.h"
#include "../../core/math-utils.h"

#include <angle/src/compiler/translator/Symbol.h>

bool opIsBuiltinUnaryFunction(TOperator op) {
  switch (op) {
    case EOpNegative:
    case EOpPositive:
    case EOpLogicalNot:
    case EOpBitwiseNot:
    case EOpPostIncrement:
    case EOpPostDecrement:
    case EOpPreIncrement:
    case EOpPreDecrement:
    case EOpArrayLength: return false;
    default: return true;
  }
}

bool basicTypeNeedsPrecision(sh::TBasicType basicType) {
  switch (basicType) {
    case sh::EbtStruct:
    case sh::EbtInterfaceBlock:
    case sh::EbtVoid:
    case sh::EbtAtomicCounter:
    case sh::EbtBool: return false;
    default: break;
  }

  return true;
}

int nodeConstantBooleanValue(sh::TIntermNode * node) {
  sh::TIntermTyped * typed = node->getAsTyped();
  if (!typed) {
    return -1;
  }

  switch (typed->getType().getBasicType()) {
    case sh::EbtFloat:
    case sh::EbtDouble:
    case sh::EbtInt:
    case sh::EbtUInt:
    case sh::EbtBool: break;
    default: return -1;
  }

  const sh::TConstantUnion * value = typed->getConstantValue();
  if (!value) {
    return -1;
  }

  int size = typed->getNominalSize() * typed->getSecondarySize();

  bool first = value[0].isZero();
  for (int i = 1; i < size; ++i) {
    if (first != value[i].isZero()) {
      return -1;
    }
  }
  return first ? 0 : 1;
}

bool nodeBlockIsEmpty(sh::TIntermNode * node) {
  if (!node) {
    return true;
  }
  sh::TIntermBlock * block = node->getAsBlock();
  if (!block) {
    return false;
  }
  const sh::TIntermSequence * sequence = block->getSequence();
  if (!sequence || sequence->size() == 0) {
    return true;
  }
  return false;
}

sh::TIntermNode * nodeGetBlockSingleNode(sh::TIntermBlock * block) {
  sh::TIntermNode * result = nullptr;
  if (block) {
    const sh::TIntermSequence * sequence = block->getSequence();
    if (sequence) {
      for (sh::TIntermNode * child : *sequence) {
        if (!nodeBlockIsEmpty(child)) {
          if (result) {
            return nullptr;
          }
          result = child;
        }
      }
    }
  }
  return result;
}

sh::TIntermNode * nodeGetBlockLastNode(sh::TIntermBlock * block) {
  if (block) {
    const sh::TIntermSequence * sequence = block->getSequence();
    if (sequence) {
      size_t size = sequence->size();
      if (size > 0) {
        return (*sequence)[size - 1];
      }
    }
  }
  return nullptr;
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
  if (binaryNode && binaryNode->getOp() == EOpInitialize) {
    return true;  // TODO: maybe this is not needed.
  }

  return false;
}

bool nodeBlockContainsSomeSortOfDeclaration(sh::TIntermNode * node) {
  if (node) {
    sh::TIntermBlock * block = node->getAsBlock();
    if (block) {
      const sh::TIntermSequence * sequence = block->getSequence();
      if (sequence) {
        for (sh::TIntermNode * child : *sequence) {
          if (!nodeIsSomeSortOfDeclaration(child)) {
            return false;
          }
        }
      }
    } else if (nodeIsSomeSortOfDeclaration(node)) {
      return true;
    }
  }
  return false;
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

#include <iostream>

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
      case sh::EbtBool: isOne = pConstUnion->getBConst() == true; break;
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
          case sh::EbtBool: pConstUnion->setIConst(constant.getBConst() ? 0xffffffff : 0); break;
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
  sh::TConstantUnion * result = new sh::TConstantUnion[size];
  _writeConstantUnionFromScalar(&type, result, constant);
  return result;
}

sh::TIntermConstantUnion * nodeCreateConstantUnionFillFromScalar(const sh::TType & type,
    const sh::TConstantUnion & constant) {
  return new sh::TIntermConstantUnion(CreateConstantUnionFillFromScalar(type, constant), type);
}

bool nodeConstantUnionIsAllZero(sh::TIntermNode * node) {
  auto constantUnion = node ? node->getAsConstantUnion() : nullptr;
  if (!constantUnion) {
    return false;
  }
  bool result = true;
  _constantUnionIsAllZero(&constantUnion->getType(), constantUnion->getConstantValue(), result);
  return result;
}

bool nodeConstantUnionIsAllOne(sh::TIntermNode * node) {
  auto constantUnion = node ? node->getAsConstantUnion() : nullptr;
  if (!constantUnion || !constantUnion->getConstantValue()) {
    return false;
  }
  bool result = true;
  _constantUnionIsAllOne(&constantUnion->getType(), constantUnion->getConstantValue(), result);
  return result;
}

class NodeCollectAllNodesVisitor final : public sh::TIntermTraverser {
 public:
  std::vector<sh::TIntermNode *> * out;

  static NodeCollectAllNodesVisitor instance;

  NodeCollectAllNodesVisitor() : sh::TIntermTraverser(true, false, false, nullptr), out(nullptr) {
  }

  void visitSymbol(sh::TIntermSymbol * node) override {
    this->out->push_back(node);
  }

  void visitConstantUnion(sh::TIntermConstantUnion * node) override {
    this->out->push_back(node);
  }

  bool visitSwizzle(sh::Visit, sh::TIntermSwizzle * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitBinary(sh::Visit, sh::TIntermBinary * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitUnary(sh::Visit, sh::TIntermUnary * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitTernary(sh::Visit, sh::TIntermTernary * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitIfElse(sh::Visit, sh::TIntermIfElse * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitSwitch(sh::Visit, sh::TIntermSwitch * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitCase(sh::Visit, sh::TIntermCase * node) override {
    this->out->push_back(node);
    return true;
  }

  void visitFunctionPrototype(sh::TIntermFunctionPrototype * node) override {
    this->out->push_back(node);
  }

  bool visitFunctionDefinition(sh::Visit, sh::TIntermFunctionDefinition * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitAggregate(sh::Visit, sh::TIntermAggregate * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitBlock(sh::Visit, sh::TIntermBlock * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitGlobalQualifierDeclaration(sh::Visit, sh::TIntermGlobalQualifierDeclaration * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitDeclaration(sh::Visit, sh::TIntermDeclaration * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitLoop(sh::Visit, sh::TIntermLoop * node) override {
    this->out->push_back(node);
    return true;
  }

  bool visitBranch(sh::Visit, sh::TIntermBranch * node) override {
    this->out->push_back(node);
    return true;
  }

  void visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) override {
    this->out->push_back(node);
  }
};

NodeCollectAllNodesVisitor NodeCollectAllNodesVisitor::instance;

void nodeCollectAllNodes(sh::TIntermNode * root, std::vector<sh::TIntermNode *> & out) {
  if (root) {
    NodeCollectAllNodesVisitor::instance.out = &out;
    root->traverse(&NodeCollectAllNodesVisitor::instance);
  }
}

class NodeKindGetterVisitor final : public sh::TIntermTraverser {
 public:
  AngleNodeKind kind;

  static NodeKindGetterVisitor instance;

  NodeKindGetterVisitor() : sh::TIntermTraverser(false, false, false, nullptr), kind(AngleNodeKind::TNull) {
  }

  void visitSymbol(sh::TIntermSymbol *) override {
    this->kind = AngleNodeKind::TIntermSymbol;
  }

  void visitConstantUnion(sh::TIntermConstantUnion *) override {
    this->kind = AngleNodeKind::TIntermConstantUnion;
  }

  bool visitSwizzle(sh::Visit, sh::TIntermSwizzle *) override {
    this->kind = AngleNodeKind::TIntermSwizzle;
    return false;
  }

  bool visitBinary(sh::Visit, sh::TIntermBinary *) override {
    this->kind = AngleNodeKind::TIntermBinary;
    return false;
  }

  bool visitUnary(sh::Visit, sh::TIntermUnary *) override {
    this->kind = AngleNodeKind::TIntermUnary;
    return false;
  }

  bool visitTernary(sh::Visit, sh::TIntermTernary *) override {
    this->kind = AngleNodeKind::TIntermTernary;
    return false;
  }

  bool visitIfElse(sh::Visit, sh::TIntermIfElse *) override {
    this->kind = AngleNodeKind::TIntermIfElse;
    return false;
  }

  bool visitSwitch(sh::Visit, sh::TIntermSwitch *) override {
    this->kind = AngleNodeKind::TIntermSwitch;
    return false;
  }

  bool visitCase(sh::Visit, sh::TIntermCase *) override {
    this->kind = AngleNodeKind::TIntermCase;
    return false;
  }

  void visitFunctionPrototype(sh::TIntermFunctionPrototype *) override {
    this->kind = AngleNodeKind::TIntermFunctionPrototype;
  }

  bool visitFunctionDefinition(sh::Visit, sh::TIntermFunctionDefinition *) override {
    this->kind = AngleNodeKind::TIntermFunctionDefinition;
    return false;
  }

  bool visitAggregate(sh::Visit, sh::TIntermAggregate *) override {
    this->kind = AngleNodeKind::TIntermAggregate;
    return false;
  }

  bool visitBlock(sh::Visit, sh::TIntermBlock *) override {
    this->kind = AngleNodeKind::TIntermBlock;
    return false;
  }

  bool visitGlobalQualifierDeclaration(sh::Visit, sh::TIntermGlobalQualifierDeclaration *) override {
    this->kind = AngleNodeKind::TIntermGlobalQualifierDeclaration;
    return false;
  }

  bool visitDeclaration(sh::Visit, sh::TIntermDeclaration *) override {
    this->kind = AngleNodeKind::TIntermDeclaration;
    return false;
  }

  bool visitLoop(sh::Visit, sh::TIntermLoop *) override {
    this->kind = AngleNodeKind::TIntermLoop;
    return false;
  }

  bool visitBranch(sh::Visit, sh::TIntermBranch *) override {
    this->kind = AngleNodeKind::TIntermBranch;
    return false;
  }

  void visitPreprocessorDirective(sh::TIntermPreprocessorDirective *) override {
    this->kind = AngleNodeKind::TIntermPreprocessorDirective;
  }
};

NodeKindGetterVisitor NodeKindGetterVisitor::instance;

AngleNodeKind nodeGetKind(sh::TIntermNode * node) {
  // Not thread safe, but it does not matter in wasm
  NodeKindGetterVisitor::instance.kind = AngleNodeKind::TNull;
  if (node) {
    node->visit(sh::PreVisit, &NodeKindGetterVisitor::instance);
  }
  return NodeKindGetterVisitor::instance.kind;
}
