#ifndef _SPGLSL_ANGLE_NODE_UTILS_H_
#define _SPGLSL_ANGLE_NODE_UTILS_H_

#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

enum class AngleNodeKind {
  TNull,
  TIntermAggregate,
  TIntermBlock,
  TIntermGlobalQualifierDeclaration,
  TIntermDeclaration,
  TIntermFunctionPrototype,
  TIntermFunctionDefinition,
  TIntermSwizzle,
  TIntermBinary,
  TIntermUnary,
  TIntermConstantUnion,
  TIntermTernary,
  TIntermIfElse,
  TIntermSwitch,
  TIntermCase,
  TIntermSymbol,
  TIntermLoop,
  TIntermBranch,
  TIntermPreprocessorDirective
};

void nodeCollectAllNodes(sh::TIntermNode * root, std::vector<sh::TIntermNode *> & out);

AngleNodeKind nodeGetKind(sh::TIntermNode * node);

bool opIsBuiltinUnaryFunction(TOperator op);

bool basicTypeNeedsPrecision(sh::TBasicType basicType);

int nodeConstantBooleanValue(sh::TIntermNode * condition);

bool nodeBlockIsEmpty(sh::TIntermNode * node);

bool nodeHasSideEffects(sh::TIntermNode * node);

sh::TIntermNode * nodeGetBlockSingleNode(sh::TIntermBlock * block);

sh::TIntermNode * nodeGetBlockLastNode(sh::TIntermBlock * block);

bool nodeIsSomeSortOfDeclaration(sh::TIntermNode * node);

bool nodeBlockContainsSomeSortOfDeclaration(sh::TIntermNode * node);

const sh::TFunction * nodeGetAsFunction(sh::TIntermNode * node);

inline sh::TIntermTyped * nodeGetAsTyped(sh::TIntermNode * node) {
  return node ? node->getAsTyped() : nullptr;
}

inline sh::TIntermConstantUnion * nodeGetAsConstantUnion(sh::TIntermNode * node) {
  return node ? node->getAsConstantUnion() : nullptr;
}

inline sh::TIntermFunctionDefinition * nodeGetAsFunctionDefinition(sh::TIntermNode * node) {
  return node ? node->getAsFunctionDefinition() : nullptr;
}

inline sh::TIntermAggregate * nodeGetAsAggregate(sh::TIntermNode * node) {
  return node ? node->getAsAggregate() : nullptr;
}

inline sh::TIntermBlock * nodeGetAsBlock(sh::TIntermNode * node) {
  return node ? node->getAsBlock() : nullptr;
}

inline sh::TIntermFunctionPrototype * nodeGetAsFunctionPrototypeNode(sh::TIntermNode * node) {
  return node ? node->getAsFunctionPrototypeNode() : nullptr;
}

inline sh::TIntermGlobalQualifierDeclaration * nodeGetAsGlobalQualifierDeclarationNode(sh::TIntermNode * node) {
  return node ? node->getAsGlobalQualifierDeclarationNode() : nullptr;
}

inline sh::TIntermDeclaration * nodeGetAsDeclarationNode(sh::TIntermNode * node) {
  return node ? node->getAsDeclarationNode() : nullptr;
}

inline sh::TIntermSwizzle * nodeGetAsSwizzleNode(sh::TIntermNode * node) {
  return node ? node->getAsSwizzleNode() : nullptr;
}

inline sh::TIntermBinary * nodeGetAsBinaryNode(sh::TIntermNode * node) {
  return node ? node->getAsBinaryNode() : nullptr;
}

inline sh::TIntermUnary * nodeGetAsUnaryNode(sh::TIntermNode * node) {
  return node ? node->getAsUnaryNode() : nullptr;
}

inline sh::TIntermTernary * nodeGetAsTernaryNode(sh::TIntermNode * node) {
  return node ? node->getAsTernaryNode() : nullptr;
}

inline sh::TIntermIfElse * nodeGetAsIfElseNode(sh::TIntermNode * node) {
  return node ? node->getAsIfElseNode() : nullptr;
}

inline sh::TIntermSwitch * nodeGetAsSwitchNode(sh::TIntermNode * node) {
  return node ? node->getAsSwitchNode() : nullptr;
}

inline sh::TIntermCase * nodeGetAsCaseNode(sh::TIntermNode * node) {
  return node ? node->getAsCaseNode() : nullptr;
}

inline sh::TIntermSymbol * nodeGetAsSymbolNode(sh::TIntermNode * node) {
  return node ? node->getAsSymbolNode() : nullptr;
}

inline sh::TIntermLoop * nodeGetAsLoopNode(sh::TIntermNode * node) {
  return node ? node->getAsLoopNode() : nullptr;
}

inline sh::TIntermBranch * nodeGetAsBranchNode(sh::TIntermNode * node) {
  return node ? node->getAsBranchNode() : nullptr;
}

inline sh::TIntermPreprocessorDirective * nodeGetAsPreprocessorDirective(sh::TIntermNode * node) {
  return node ? node->getAsPreprocessorDirective() : nullptr;
}

inline bool nodeIsTrueConstant(sh::TIntermNode * node) {
  return nodeConstantBooleanValue(node) == 1;
}

inline bool nodeIsFalseConstant(sh::TIntermNode * node) {
  return nodeConstantBooleanValue(node) == 0;
}

sh::TConstantUnion * CreateConstantUnionFillFromScalar(const sh::TType & type,
    const sh::TConstantUnion & constant = sh::TConstantUnion());

sh::TIntermConstantUnion * nodeCreateConstantUnionFillFromScalar(const sh::TType & type,
    const sh::TConstantUnion & constant = sh::TConstantUnion());

bool nodeConstantUnionIsAllZero(sh::TIntermNode * node);
bool nodeConstantUnionIsAllOne(sh::TIntermNode * node);

inline sh::TConstantUnion ConstantUnionFromBool(bool value) {
  sh::TConstantUnion result;
  result.setBConst(value);
  return result;
}

inline sh::TConstantUnion ConstantUnionFromInt(int value) {
  sh::TConstantUnion result;
  result.setIConst(value);
  return result;
}

inline sh::TConstantUnion ConstantUnionFromUInt(unsigned int value) {
  sh::TConstantUnion result;
  result.setUConst(value);
  return result;
}

inline sh::TConstantUnion ConstantUnionFromFloat(float value) {
  sh::TConstantUnion result;
  result.setFConst(value);
  return result;
}

inline TOperator nodeGetOperator(sh::TIntermNode * node) {
  if (!node) {
    return EOpNull;
  }
  auto unary = node->getAsUnaryNode();
  if (unary) {
    return unary->getOp();
  }
  auto binary = node->getAsBinaryNode();
  if (binary) {
    return binary->getOp();
  }
  auto aggr = node->getAsAggregate();
  if (aggr) {
    return aggr->getOp();
  }
  return EOpNull;
}

inline sh::TIntermTyped * nodeGetUnaryOperand(sh::TIntermTyped * node) {
  if (!node) {
    return nullptr;
  }
  auto unary = node->getAsUnaryNode();
  if (unary) {
    return unary->getOperand();
  }
  auto swizzle = node->getAsSwizzleNode();
  if (swizzle) {
    return swizzle->getOperand();
  }
  return node;
}

#endif