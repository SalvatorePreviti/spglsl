#include "../lib/spglsl-angle-node-utils.h"
#include "../spglsl-angle-compiler.h"
#include "tree-ops.h"

#include <iostream>

/** Returns an TIntermTyped* if the given node can be used as argument in a Comma operator */
sh::TIntermTyped * _asCommaOpArg(sh::TIntermNode * node) {
  if (!node) {
    return nullptr;
  }
  auto * asTyped = node->getAsTyped();
  if (!asTyped) {
    // Not a typed node, cannot be used.
    return nullptr;
  }

  if (!asTyped->getAsAggregate() && !asTyped->getAsSwizzleNode() && !asTyped->getAsBinaryNode() &&
      !asTyped->getAsUnaryNode() && !asTyped->getAsTernaryNode() && !asTyped->getAsSymbolNode()) {
    // Declarations are not supported in comma operators.
    return nullptr;
  }

  if (asTyped->isArray() || asTyped->getBasicType() == sh::EbtVoid ||
      asTyped->getType().isStructureContainingArrays()) {
    // sequence operator is not allowed for void, arrays, or structs containing arrays not allowed in comma.
    return nullptr;
  }

  return asTyped;
}

class SpglslPutCommaOperatorTraverser : public sh::TIntermTraverser {
 public:
  bool hasChanges = false;

  SpglslPutCommaOperatorTraverser() : sh::TIntermTraverser(false, false, true) {
  }

  void reset() {
    this->hasChanges = false;
  }

  inline void traverseNode(sh::TIntermNode * node) {
    if (node) {
      node->traverse(this);
    }
  }

  bool visitBlock(sh::Visit visit, sh::TIntermBlock * block) override {
    bool changed = false;
    sh::TIntermTyped * commaLeft = nullptr;
    sh::TIntermSequence newSequence;
    auto count = block->getChildCount();
    size_t i = 0;

    while (i < count) {
      auto * node = block->getChildNode(i++);

      auto * ifNode = node->getAsIfElseNode();
      if (ifNode) {
        auto * condition = ifNode->getCondition();
        auto * trueBlock = ifNode->getTrueBlock();
        auto * falseBlock = ifNode->getFalseBlock();

        auto * a0 = _asCommaOpArg(nodeGetBlockSingleNode(trueBlock));
        if (a0) {
          if (falseBlock) {
            auto * a1 = _asCommaOpArg(nodeGetBlockSingleNode(falseBlock));
            if (a1 && a0->getType() == a1->getType()) {
              // Replace "if(condition){a0}else{a1}" with "condition?a0:a1";
              node = new sh::TIntermTernary(condition, a0, a1);
              changed = true;
            }
          } else if (a0->getType().isScalar() && a0->getType().getBasicType() == sh::EbtBool) {
            // Replace "if(condition){a0}" with "condition&&a0";
            node = new sh::TIntermBinary(sh::EOpLogicalAnd, condition, a0);
            changed = true;
          }
        }
      }

      auto * commaRight = _asCommaOpArg(node);

      if (commaRight) {
        if (commaLeft) {
          // Merge two statement with a comma operator. Previous statement can be a comma.
          commaLeft = new sh::TIntermBinary(sh::EOpComma, commaLeft, commaRight);
          changed = true;
        } else {
          commaLeft = commaRight;
        }
        continue;
      }

      if (commaLeft) {
        auto * branchNode = node->getAsBranchNode();
        if (branchNode && branchNode->getFlowOp() == sh::EOpReturn && branchNode->getExpression()) {
          node = new sh::TIntermBranch(
              sh::EOpReturn, new sh::TIntermBinary(sh::EOpComma, commaLeft, branchNode->getExpression()));
          changed = true;
        } else {
          auto * ifElseNode = node->getAsIfElseNode();
          if (ifElseNode) {
            node = new sh::TIntermIfElse(new sh::TIntermBinary(sh::EOpComma, commaLeft, ifElseNode->getCondition()),
                ifElseNode->getTrueBlock(), ifElseNode->getFalseBlock());
            changed = true;
          } else {
            newSequence.push_back(commaLeft);
          }
        }
        commaLeft = nullptr;
      }

      newSequence.push_back(node);
    }

    if (commaLeft) {
      newSequence.push_back(commaLeft);
    }

    if (changed) {
      block->replaceAllChildren(newSequence);
      this->hasChanges = true;
    }

    return true;
  }
};

#include <iostream>

void spglsl_treeops_minify(SpglslAngleCompiler & compiler, sh::TIntermNode * root) {
  for (SpglslPutCommaOperatorTraverser traverser;;) {
    root->traverse(&traverser);
    if (!traverser.hasChanges) {
      break;
    }
    traverser.hasChanges = false;
  }
}
