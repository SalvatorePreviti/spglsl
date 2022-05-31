#include "spglsl-angle-operator-precedence.h"
#include "spglsl-angle-node-utils.h"

struct AngleOperatorPrecedenceMap {
  AngleOperatorPrecedence ByOp[256];

  AngleOperatorPrecedence Ternary;
  AngleOperatorPrecedence Aggregate;
  AngleOperatorPrecedence Swizzle;

  AngleOperatorPrecedenceMap();
};

AngleOperatorPrecedenceMap::AngleOperatorPrecedenceMap() {
  auto & byOp = this->ByOp;

  // array subscript
  // function call and constructor structure
  // field or method selector, swizzler
  // post fix increment and decrement

  this->Aggregate = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  this->Swizzle = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexDirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexIndirect] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexDirectStruct] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpIndexDirectInterfaceBlock] = AngleOperatorPrecedence(0, AngleOperatorOrder::NA, false, true);
  byOp[sh::EOpPostIncrement] = AngleOperatorPrecedence(0, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpPostDecrement] = AngleOperatorPrecedence(0, AngleOperatorOrder::LTR, false);

  // prefix increment and decrement
  // unary
  byOp[sh::EOpPreIncrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpPreDecrement] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpNegative] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpPositive] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpLogicalNot] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpBitwiseNot] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);
  byOp[sh::EOpArrayLength] = AngleOperatorPrecedence(10, AngleOperatorOrder::RTL, false);

  // multiplicative

  byOp[sh::EOpMul] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpDiv] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpIMod] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpVectorTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpVectorTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesVector] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesScalar] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpMatrixTimesMatrix] = AngleOperatorPrecedence(30, AngleOperatorOrder::LTR, true);

  // additive

  byOp[sh::EOpAdd] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, true);
  byOp[sh::EOpSub] = AngleOperatorPrecedence(40, AngleOperatorOrder::LTR, false);

  // bit-wise shift

  byOp[sh::EOpBitShiftLeft] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpBitShiftRight] = AngleOperatorPrecedence(50, AngleOperatorOrder::LTR, false);

  // relational

  byOp[sh::EOpLessThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpGreaterThan] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpLessThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpGreaterThanEqual] = AngleOperatorPrecedence(60, AngleOperatorOrder::LTR, false);

  // equality

  byOp[sh::EOpEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);
  byOp[sh::EOpNotEqual] = AngleOperatorPrecedence(70, AngleOperatorOrder::LTR, false);

  // bit-wise and

  byOp[sh::EOpBitwiseAnd] = AngleOperatorPrecedence(80, AngleOperatorOrder::LTR, true);

  // bit-wise exclusive or

  byOp[sh::EOpBitwiseXor] = AngleOperatorPrecedence(90, AngleOperatorOrder::LTR, true);

  // bit-wise inclusive or

  byOp[sh::EOpBitwiseOr] = AngleOperatorPrecedence(100, AngleOperatorOrder::LTR, true);

  // logical and

  byOp[sh::EOpLogicalAnd] = AngleOperatorPrecedence(110, AngleOperatorOrder::LTR, true);

  // logical exclusive or

  byOp[sh::EOpLogicalXor] = AngleOperatorPrecedence(120, AngleOperatorOrder::LTR, true);

  // logical inclusive or

  byOp[sh::EOpLogicalOr] = AngleOperatorPrecedence(130, AngleOperatorOrder::LTR, true);

  // selection

  this->Ternary = AngleOperatorPrecedence(140, AngleOperatorOrder::RTL, false);

  // Assignment
  // arithmetic assignments

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

  // sequence

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
  sh::TIntermAggregate * aggregate = node.getAsAggregate();
  if (aggregate) {
    return _precedenceMap.Aggregate;
  }
  sh::TIntermSwizzle * swizzle = node.getAsSwizzleNode();
  if (swizzle) {
    return _precedenceMap.Swizzle;
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

  if (a.getAsAggregate() && b.getAsAggregate()) {
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

  bool haveSamePriority = nodesAreSameOpPriority(node, child);

  int precedenceDiff = haveSamePriority ? 0 : childPrec.precedence - nodePrec.precedence;
  if (precedenceDiff == 0) {
    haveSamePriority = true;
  }

  if (precedenceDiff < 0) {
    return false;
  }

  if (nodePrec.indexing && (operandIndex == 1 || child.getAsSymbolNode() || childPrec.indexing)) {
    return false;
  }

  if (precedenceDiff > 0) {
    if (node.getAsTernaryNode()) {
      if (operandIndex == 1) {
        return !((child.getAsBinaryNode() || child.getAsUnaryNode() || child.getAsSwizzleNode() ||
            child.getAsConstantUnion() || child.getAsSwizzleNode() || child.getAsAggregate() ||
            child.getAsSymbolNode() || child.getAsTernaryNode()));
      }
      if (operandIndex == 2) {
        auto * asBin = child.getAsBinaryNode();
        if (asBin) {
          switch (asBin->getOp()) {
            case sh::EOpComma: return true;
            default: return false;
          }
        }
        return !((child.getAsSwizzleNode() || child.getAsConstantUnion() || child.getAsSwizzleNode() ||
            child.getAsAggregate() || child.getAsSymbolNode() || child.getAsTernaryNode()));
      }
    }
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
    if (haveSamePriority) {
      return false;
    }
  }

  if (haveSamePriority && childPrec.order == AngleOperatorOrder::LTR && nodePrec.order == AngleOperatorOrder::LTR) {
    return false;
  }

  return true;
}
