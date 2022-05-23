#include "../lib/spglsl-angle-node-utils.h"
#include "tree-ops.h"

/** Returns an TIntermTyped* if the given node can be used as argument in a Comma operator */
sh::TIntermTyped * _asCommaOpArg(sh::TIntermNode * node) {
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

  SpglslPutCommaOperatorTraverser() : sh::TIntermTraverser(true, false, false) {
  }

  void reset() {
    this->hasChanges = false;
  }

  bool visitBlock(sh::Visit visit, sh::TIntermBlock * block) override {
    bool changed = false;
    sh::TIntermTyped * prev = nullptr;
    sh::TIntermSequence newSequence;
    auto count = block->getChildCount();
    size_t i = 0;
    while (i < count) {
      auto * node = block->getChildNode(i++);

      auto * current = _asCommaOpArg(node);

      if (current) {
        if (prev) {
          changed = true;
          prev = new sh::TIntermBinary(sh::EOpComma, prev, current);
        } else {
          prev = current;
        }
        continue;
      }

      if (prev) {
        newSequence.push_back(prev);
        prev = nullptr;
      }
      newSequence.push_back(node);
    }

    if (prev) {
      newSequence.push_back(prev);
    }

    if (changed) {
      block->replaceAllChildren(newSequence);
      this->hasChanges = true;
    }

    return true;
  }
};

void spglsl_treeops_putCommaOperators(SpglslAngleCompiler & compiler, sh::TIntermNode * root) {
  for (SpglslPutCommaOperatorTraverser traverser;;) {
    root->traverse(&traverser);
    if (!traverser.hasChanges) {
      break;
    }
    traverser.hasChanges = false;
  }
}
