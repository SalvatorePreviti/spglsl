#include "../lib/spglsl-angle-node-utils.h"
#include "tree-ops.h"

class SpglslRemoveUnnecessaryBlocks : public sh::TIntermTraverser {
 public:
  SpglslRemoveUnnecessaryBlocks() : sh::TIntermTraverser(true, false, false) {
  }

  bool hasChanges = false;

  bool visitBlock(sh::Visit visit, sh::TIntermBlock * block) override {
    if (this->getCurrentBlockDepth() == 0) {
      return true;
    }

    bool changed = false;

    auto count = block->getChildCount();
    sh::TIntermSequence newSequence;
    std::queue<sh::TIntermNode *> queue;

    size_t i = 0;
    while (i < count || !queue.empty()) {
      sh::TIntermNode * node;

      if (queue.empty()) {
        node = block->getChildNode(i++);
      } else {
        node = queue.front();
        queue.pop();
      }

      auto * nodeAsBlock = node->getAsBlock();
      if (nodeAsBlock && !nodeBlockContainsSomeSortOfDeclaration(nodeAsBlock)) {
        // Remove unnecessary block
        for (auto * nodeAsBlockChild : *nodeAsBlock->getSequence()) {
          queue.push(nodeAsBlockChild);
        }
        changed = true;
        continue;
      }

      auto * nodeBinary = node->getAsBinaryNode();
      if (nodeBinary && nodeBinary->getOp() == sh::EOpComma) {
        // Remove unnecessary comma operator
        queue.push(nodeBinary->getLeft());
        queue.push(nodeBinary->getRight());
        changed = true;
        continue;
      }

      newSequence.push_back(node);
      ++i;
    }

    if (changed) {
      block->replaceAllChildren(newSequence);
      this->hasChanges = true;
    }
    return true;
  }
};

void spglsl_treeops_removeUnnecessaryBlocks(SpglslAngleCompiler & compiler, sh::TIntermNode * root) {
  for (SpglslRemoveUnnecessaryBlocks traverser;;) {
    root->traverse(&traverser);
    if (!traverser.hasChanges) {
      break;
    }
    traverser.hasChanges = false;
  }
}
