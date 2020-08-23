#include "spglsl-angle-operator-precedence.h"

struct AngleOperatorPrecedenceMap {
  AngleOperatorPrecedence ByOp[256];

  AngleOperatorPrecedence Ternary;

  AngleOperatorPrecedenceMap();
};

AngleOperatorPrecedenceMap::AngleOperatorPrecedenceMap() {
  auto & byOp = this->ByOp;

  this->Ternary = AngleOperatorPrecedence(140, AngleOperatorOrder::RTL, false);

  byOp[EOpIndexDirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[EOpIndexIndirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[EOpIndexDirectStruct] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[EOpIndexDirectInterfaceBlock] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);

  byOp[EOpPostIncrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::LTR, false);
  byOp[EOpPostDecrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::LTR, false);

  byOp[EOpNegative] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpPositive] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpLogicalNot] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpBitwiseNot] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpPreIncrement] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpPreDecrement] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);
  byOp[EOpArrayLength] = AngleOperatorPrecedence(20, AngleOperatorOrder::RTL, false);

  byOp[EOpMul] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[EOpDiv] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);
  byOp[EOpIMod] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);

  byOp[EOpVectorTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[EOpVectorTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[EOpMatrixTimesVector] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[EOpMatrixTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[EOpMatrixTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);

  byOp[EOpAdd] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, true);
  byOp[EOpSub] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, false);
  byOp[EOpBitShiftLeft] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);
  byOp[EOpBitShiftRight] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);
  byOp[EOpLessThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[EOpGreaterThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[EOpLessThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[EOpGreaterThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[EOpEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);
  byOp[EOpNotEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);
  byOp[EOpBitwiseAnd] = AngleOperatorPrecedence(80, AngleOperatorOrder::LTR, true);
  byOp[EOpBitwiseXor] = AngleOperatorPrecedence(90, AngleOperatorOrder::LTR, true);
  byOp[EOpBitwiseOr] = AngleOperatorPrecedence(100, AngleOperatorOrder::LTR, true);
  byOp[EOpLogicalAnd] = AngleOperatorPrecedence(110, AngleOperatorOrder::LTR, true);
  byOp[EOpLogicalXor] = AngleOperatorPrecedence(120, AngleOperatorOrder::LTR, true);
  byOp[EOpLogicalOr] = AngleOperatorPrecedence(130, AngleOperatorOrder::LTR, true);
  byOp[EOpInitialize] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpAddAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpSubAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpDivAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpIModAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpMulAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);

  byOp[EOpVectorTimesMatrixAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpVectorTimesScalarAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpMatrixTimesScalarAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpMatrixTimesMatrixAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);

  byOp[EOpBitShiftLeftAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpBitShiftRightAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpBitwiseAndAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpBitwiseXorAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpBitwiseOrAssign] = AngleOperatorPrecedence(150, AngleOperatorOrder::LTR, false);
  byOp[EOpComma] = AngleOperatorPrecedence(200, AngleOperatorOrder::LTR, false);
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
  auto & nodePrec = AngleOperatorPrecedence::get(node);
  if (!nodePrec.exists) {
    return false;
  }

  auto & childPrec = AngleOperatorPrecedence::get(child);

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
