#include "spglsl-angle-operator-precedence.h"

struct AngleOperatorPrecedenceMap {
  AngleOperatorPrecedence ByOp[256];

  AngleOperatorPrecedence Ternary;

  AngleOperatorPrecedenceMap();
};

AngleOperatorPrecedenceMap::AngleOperatorPrecedenceMap() {
  auto & byOp = this->ByOp;

  this->Ternary = AngleOperatorPrecedence(140, AngleOperatorOrder::RTL, false);

  byOp[sh::EOpIndexDirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexIndirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexDirectStruct] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexDirectInterfaceBlock] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);

  byOp[sh::EOpPostIncrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpPostDecrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::LTR, false);

  byOp[sh::EOpNegative] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpPositive] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpLogicalNot] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpBitwiseNot] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpPreIncrement] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpPreDecrement] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpArrayLength] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);

  byOp[sh::EOpMul] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpDiv] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpIMod] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);

  byOp[sh::EOpVectorTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpVectorTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesVector] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);

  byOp[sh::EOpAdd] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpSub] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitShiftLeft] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitShiftRight] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpLessThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpGreaterThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpLessThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpGreaterThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpNotEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitwiseAnd] = AngleOperatorPrecedence(80, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpBitwiseXor] = AngleOperatorPrecedence(90, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpBitwiseOr] = AngleOperatorPrecedence(100, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpLogicalAnd] = AngleOperatorPrecedence(110, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpLogicalXor] = AngleOperatorPrecedence(120, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpLogicalOr] = AngleOperatorPrecedence(130, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpInitialize] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpAddAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpSubAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpDivAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpIModAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpMulAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);

  byOp[sh::EOpVectorTimesMatrixAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpVectorTimesScalarAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpMatrixTimesScalarAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpMatrixTimesMatrixAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);

  byOp[sh::EOpBitShiftLeftAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitShiftRightAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitwiseAndAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitwiseXorAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitwiseOrAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpComma] = AngleOperatorPrecedence(200, AngleOperatorOrder::LTR, false);
}

AngleOperatorPrecedence::AngleOperatorPrecedence() :
    precedence(0), order(AngleOperatorOrder::NA), associative(false), exists(false) {
}

AngleOperatorPrecedence::AngleOperatorPrecedence(int precedence,
    AngleOperatorOrder order,
    bool associative,
    bool indexing,
    bool exists) :
    precedence(precedence), order(order), associative(associative), indexing(indexing), exists(exists) {
}

static AngleOperatorPrecedenceMap _precedenceMap;

const AngleOperatorPrecedence & AngleOperatorPrecedence::get(sh::TIntermNode & node) {
  sh::TIntermBinary * binary = node.getAsBinaryNode();
  if (binary) {
    return _precedenceMap.ByOp[binary->getOp()];
  }
  sh::TIntermUnary * unary = node.getAsUnaryNode();
  if (unary) {
    return _precedenceMap.ByOp[unary->getOp()];
  }
  sh::TIntermTernary * ternary = node.getAsTernaryNode();
  if (ternary) {
    return _precedenceMap.Ternary;
  }
  return _precedenceMap.ByOp[0];
}

bool nodesAreSameOpPriority(sh::TIntermNode & a, sh::TIntermNode & b) {
  if (a.getAsBinaryNode() && b.getAsBinaryNode()) {
    return a.getAsBinaryNode()->getOp() == b.getAsBinaryNode()->getOp();
  }

  if (a.getAsUnaryNode() && b.getAsUnaryNode()) {
    return a.getAsUnaryNode()->getOp() == b.getAsUnaryNode()->getOp();
  }

  if (a.getAsConstantUnion() && b.getAsConstantUnion()) {
    return true;
  }

  if (a.getAsAggregate() && b.getAsAggregate()) {
    return true;
  }

  if (a.getAsSwizzleNode() && b.getAsSwizzleNode()) {
    return true;
  }

  if (a.getAsTernaryNode() && b.getAsTernaryNode()) {
    return true;
  }

  return false;
}

bool childNodeNeedsParentheses(sh::TIntermNode & node, sh::TIntermNode & child, size_t operandIndex) {
  const auto & nodePrec = AngleOperatorPrecedence::get(node);
  if (!nodePrec.exists) {
    return false;
  }

  const auto & childPrec = AngleOperatorPrecedence::get(child);

  int precedenceDiff = childPrec.precedence - nodePrec.precedence;

  if (precedenceDiff < 0) {
    return false;
  }

  if (nodePrec.indexing && (operandIndex == 1 || child.getAsSymbolNode() || childPrec.indexing)) {
    return false;
  }

  if (precedenceDiff > 0) {
    return true;
  }

  if ((nodePrec.order == AngleOperatorOrder::LTR && operandIndex == 0) ||
      (nodePrec.order == AngleOperatorOrder::RTL && operandIndex >= node.getChildCount() - 1)) {
    return false;
  }

  if (childPrec.associative) {
    if (nodePrec.associative) {
      return false;
    }
    if (nodesAreSameOpPriority(child, node)) {
      return false;
    }
  }

  return true;
}
