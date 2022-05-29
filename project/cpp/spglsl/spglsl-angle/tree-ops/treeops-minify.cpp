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
      !asTyped->getAsUnaryNode() && !asTyped->getAsTernaryNode() && !asTyped->getAsSymbolNode() &&
      !asTyped->getAsConstantUnion()) {
    // Not supported in sequence operator.
    return nullptr;
  }

  if (asTyped->isArray() || asTyped->getBasicType() == sh::EbtVoid ||
      asTyped->getType().isStructureContainingArrays()) {
    // sequence operator is not allowed for void, arrays, or structs containing arrays.
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
    sh::TIntermSequence newSequence;
    auto count = block->getChildCount();
    size_t i = 0;

    std::vector<sh::TIntermTyped *> commaPending;

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
            }
          } else if (a0->getType().isScalar() && a0->getType().getBasicType() == sh::EbtBool) {
            // Replace "if(condition){a0}" with "condition&&a0";
            node = new sh::TIntermBinary(sh::EOpLogicalAnd, condition, a0);
          }
        }
      }

      auto * commaRight = _asCommaOpArg(node);
      if (commaRight) {
        commaPending.push_back(commaRight);
        continue;
      }

      auto * flushedCommas = _flushCommas(commaPending);
      if (flushedCommas) {
        auto * branchNode = node->getAsBranchNode();
        if (branchNode && branchNode->getFlowOp() == sh::EOpReturn && branchNode->getExpression()) {
          if (_asCommaOpArg(branchNode->getExpression())) {
            node = new sh::TIntermBranch(
                sh::EOpReturn, new sh::TIntermBinary(sh::EOpComma, flushedCommas, branchNode->getExpression()));
          } else {
            newSequence.push_back(flushedCommas);
          }
        } else {
          auto * ifElseNode = node->getAsIfElseNode();
          if (ifElseNode) {
            if (_asCommaOpArg(ifElseNode->getCondition())) {
              node =
                  new sh::TIntermIfElse(new sh::TIntermBinary(sh::EOpComma, flushedCommas, ifElseNode->getCondition()),
                      ifElseNode->getTrueBlock(), ifElseNode->getFalseBlock());
            } else {
              newSequence.push_back(flushedCommas);
            }
          } else {
            newSequence.push_back(flushedCommas);
          }
        }
      }

      newSequence.push_back(node);
    }

    auto * lastCommas = _flushCommas(commaPending);
    if (lastCommas) {
      newSequence.push_back(lastCommas);
    }

    bool changed = false;
    if (newSequence.size() != block->getChildCount()) {
      changed = true;
    } else {
      for (size_t i = 0; i < newSequence.size(); ++i) {
        if (newSequence[i] != block->getChildNode(i)) {
          changed = true;
          break;
        }
      }
    }

    if (changed) {
      block->replaceAllChildren(newSequence);
      this->hasChanges = true;
    }

    return true;
  }

 private:
  static sh::TIntermTyped * _flushCommas(std::vector<sh::TIntermTyped *> & commaPending) {
    switch (commaPending.size()) {
      case 0: return nullptr;
      case 1: {
        auto * result = commaPending[0];
        commaPending.clear();
        return result;
      }
      default: break;
    }

    sh::TIntermTyped * commaLeft = commaPending[0];
    for (size_t i = 1; i < commaPending.size(); ++i) {
      commaLeft = new sh::TIntermBinary(sh::EOpComma, commaLeft, commaPending[i]);
    }
    commaPending.clear();
    return commaLeft;
  }
};

void spglsl_treeops_minify(SpglslAngleCompiler & compiler, sh::TIntermNode * root) {
  for (SpglslPutCommaOperatorTraverser traverser;;) {
    root->traverse(&traverser);
    if (!traverser.hasChanges) {
      break;
    }
    traverser.hasChanges = false;
  }
}
