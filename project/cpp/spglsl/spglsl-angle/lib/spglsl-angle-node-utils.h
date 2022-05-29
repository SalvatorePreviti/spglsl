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

AngleNodeKind nodeGetKind(const sh::TIntermNode * node);

const char * AngleNodeKind_name(AngleNodeKind kind);

inline const char * AngleNodeKind_name(const sh::TIntermNode * node) {
  return AngleNodeKind_name(nodeGetKind(node));
}

inline const char * AngleNodeKind_name(const sh::TIntermNode & node) {
  return AngleNodeKind_name(nodeGetKind(&node));
}

bool opIsBuiltinUnaryFunction(sh::TOperator op);

int nodeConstantBooleanValue(sh::TIntermNode * condition);

bool nodeBlockIsEmpty(sh::TIntermNode * node);

bool nodeHasSideEffects(sh::TIntermNode * node);

sh::TIntermNode * nodeGetStatementSingleNode(sh::TIntermNode * node);

sh::TIntermNode * nodeGetBlockSingleNode(sh::TIntermNode * node);

bool nodeIsSomeSortOfDeclaration(sh::TIntermNode * node);

bool nodeBlockContainsSomeSortOfDeclaration(sh::TIntermNode * node);

const sh::TFunction * nodeGetAsFunction(sh::TIntermNode * node);

bool isIntermNodeSingleStatement(sh::TIntermNode * node);

inline sh::TIntermConstantUnion * nodeGetAsConstantUnion(sh::TIntermNode * node) {
  return node ? node->getAsConstantUnion() : nullptr;
}

inline sh::TIntermBinary * nodeGetAsBinaryNode(sh::TIntermNode * node) {
  return node ? node->getAsBinaryNode() : nullptr;
}

inline sh::TIntermSymbol * nodeGetAsSymbolNode(sh::TIntermNode * node) {
  return node ? node->getAsSymbolNode() : nullptr;
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

inline sh::TOperator nodeGetOperator(sh::TIntermNode * node) {
  if (!node) {
    return sh::EOpNull;
  }
  auto * unary = node->getAsUnaryNode();
  if (unary) {
    return unary->getOp();
  }
  auto * binary = node->getAsBinaryNode();
  if (binary) {
    return binary->getOp();
  }
  auto * aggr = node->getAsAggregate();
  if (aggr) {
    return aggr->getOp();
  }
  return sh::EOpNull;
}

inline sh::TIntermTyped * nodeGetUnaryOperand(sh::TIntermTyped * node) {
  if (!node) {
    return nullptr;
  }
  auto * unary = node->getAsUnaryNode();
  if (unary) {
    return unary->getOperand();
  }
  auto * swizzle = node->getAsSwizzleNode();
  if (swizzle) {
    return swizzle->getOperand();
  }
  return node;
}

#endif
