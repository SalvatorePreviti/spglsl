#ifndef _SPGLSL_ANGLE_OPERATOR_PRECEDENCE_H_
#define _SPGLSL_ANGLE_OPERATOR_PRECEDENCE_H_

#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

enum class AngleOperatorOrder { NA = 0, LTR = 1, RTL = 2 };

struct AngleOperatorPrecedence {
  int precedence;
  AngleOperatorOrder order;

  bool associative : 1;
  bool indexing : 1;
  bool exists : 1;

  static constexpr AngleOperatorOrder NA = AngleOperatorOrder::NA;
  static constexpr AngleOperatorOrder LTR = AngleOperatorOrder::LTR;
  static constexpr AngleOperatorOrder RTL = AngleOperatorOrder::RTL;

  static const AngleOperatorPrecedence & get(sh::TIntermNode & node);

  explicit AngleOperatorPrecedence();

  explicit AngleOperatorPrecedence(int precedence,
      AngleOperatorOrder order,
      bool associative,
      bool indexing = false,
      bool exists = true);

  inline bool isSameLevel(const AngleOperatorPrecedence & other) const {
    return this->precedence == other.precedence && this->order == other.order && this->indexing == other.indexing &&
        this->exists == other.exists;
  }
};

bool childNodeNeedsParentheses(sh::TIntermNode & node, sh::TIntermNode & child, size_t operandIndex);

#endif