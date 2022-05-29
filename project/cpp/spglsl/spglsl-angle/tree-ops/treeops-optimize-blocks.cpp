#include "../lib/spglsl-angle-ast-hasher.h"
#include "../lib/spglsl-angle-node-utils.h"
#include "../spglsl-angle-compiler.h"
#include "tree-ops.h"

#include <iostream>

typedef std::vector<sh::TIntermNode> OptimizeBlockNodeResult;

class SpglslOptimizeBlocksTraverser : public sh::TIntermTraverser {
 public:
  AngleAstHasher & astHasher;
  SpglslSymbols & symbols;

  explicit SpglslOptimizeBlocksTraverser(SpglslSymbols & symbols,
      AngleAstHasher & astHasher,
      sh::TDiagnostics * diagnostics) :
      sh::TIntermTraverser(true, false, false, symbols.symbolTable),
      symbols(symbols),
      astHasher(astHasher),
      _diagnostics(diagnostics) {
  }

  bool hasChanges = false;

  bool visitBlock(sh::Visit visit, sh::TIntermBlock * block) override {
    if (this->getCurrentBlockDepth() == 0) {
      return true;
    }

    auto count = block->getChildCount();
    sh::TIntermSequence newSequence;
    std::queue<sh::TIntermNode *> queue;

    size_t i = 0;
    for (;;) {
      sh::TIntermNode * node;
      if (queue.empty()) {
        if (i >= count) {
          break;
        }
        node = block->getChildNode(i++);
      } else {
        node = queue.front();
        queue.pop();
      }

      while (node) {
        sh::TIntermNode * current = node;
        node = this->_optimizeBlockChild(node, queue);
        if (current == node) {
          break;
        }
      }

      if (node != nullptr) {
        newSequence.push_back(node);
      }
    }

    bool changed = newSequence.size() != count;
    if (!changed) {
      for (size_t i = 0; i < count; ++i) {
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
  sh::TDiagnostics * _diagnostics;

  sh::TIntermNode * _optimizeBlockChild(sh::TIntermNode * node, std::queue<sh::TIntermNode *> & queue) {
    if (node->getAsBinaryNode() || node->getAsUnaryNode() || node->getAsSymbolNode() || node->getAsSwizzleNode() ||
        node->getAsConstantUnion()) {
      if (!nodeHasSideEffects(node)) {
        return nullptr;  // Node has no effect at all
      }
    }

    auto * nodeAsTernary = node->getAsTernaryNode();
    if (nodeAsTernary) {
      // Replace ternary operator with an if then else.
      return new sh::TIntermIfElse(nodeAsTernary->getCondition(), sh::EnsureBlock(nodeAsTernary->getTrueExpression()),
          sh::EnsureBlock(nodeAsTernary->getFalseExpression()));
    }

    auto * nodeAsIf = node->getAsIfElseNode();
    if (nodeAsIf) {
      auto * originalCondition = nodeAsIf->getCondition();

      auto * condition = originalCondition->fold(this->_diagnostics);

      int conditionConst = nodeConstantBooleanValue(condition);
      if (conditionConst != -1) {
        // Remove constant if, "if (true){a}else{b}" {a}; "if(false){a}else{b}" {b};
        return conditionConst == 0 ? nodeAsIf->getFalseBlock() : nodeAsIf->getTrueBlock();
      }

      auto * trueBlock = nodeAsIf->getTrueBlock();
      auto * falseBlock = nodeAsIf->getFalseBlock();
      if (trueBlock && falseBlock && trueBlock->getChildCount() == 1 && falseBlock->getChildCount() == 1) {
        auto * trueBin = trueBlock->getChildNode(0)->getAsBinaryNode();
        auto * falseBin = falseBlock->getChildNode(0)->getAsBinaryNode();
        if (trueBin && falseBin && trueBin->getOp() == falseBin->getOp() && sh::IsAssignment(trueBin->getOp()) &&
            this->astHasher.nodesAreTheSame(trueBin->getLeft(), falseBin->getLeft())) {
          // Replace "if (condition) {x=t} else {x=f}" to "x=condition?t:f";
          return new sh::TIntermBinary(trueBin->getOp(), trueBin->getLeft(),
              new sh::TIntermTernary(condition, trueBin->getRight(), falseBin->getRight()));
        }
      }

      if (originalCondition != condition) {
        return new sh::TIntermIfElse(condition, trueBlock, falseBlock);
      }
    }

    auto * nodeAsLoop = node->getAsLoopNode();
    if (nodeAsLoop) {
      sh::TLoopType loopType = nodeAsLoop->getType();

      int constantCondition = nodeAsLoop->getCondition() ? nodeConstantBooleanValue(nodeAsLoop->getCondition()) : 1;

      if (loopType == sh::ELoopWhile) {
        if (constantCondition == 0) {
          return nullptr;  // we can remove "while (false) { ... }"
        }
        if (constantCondition == 1) {
          // This is an infinite while loop, use "for (;;) { ... }"
          return new sh::TIntermLoop(sh::ELoopFor, nullptr, nullptr, nullptr, nodeAsLoop->getBody());
        }
        // Replace the while loop with a for loop.
        return new sh::TIntermLoop(sh::ELoopFor, nullptr, nodeAsLoop->getCondition(), nullptr, nodeAsLoop->getBody());
      }

      if (loopType == sh::ELoopDoWhile) {
        if (constantCondition == 1) {
          // "do {...} while (true)" => for (;;) {...}
          return new sh::TIntermLoop(sh::ELoopFor, nullptr, nullptr, nullptr, nodeAsLoop->getBody());
        }
        if (constantCondition == 0) {
          return nodeAsLoop->getBody();  // "do { ... } while (false)" becomes just "{ ... }".
        }
      }

      if (loopType == sh::ELoopFor) {
        if (constantCondition == 0) {
          if (nodeAsLoop->getInit()) {
            // for (init;false,expression) => {init}
            return new sh::TIntermBlock({nodeAsLoop->getInit()});
          }
          // for (;false,expression) can be removed
          return nullptr;
        }
        auto * newCondition = constantCondition == 1 ? nullptr : nodeAsLoop->getCondition();
        auto * newExpression = nodeAsLoop->getExpression();
        if (newExpression && !nodeHasSideEffects(newExpression)) {
          newExpression = nullptr;
        }
        if (nodeAsLoop->getExpression() != newExpression || nodeAsLoop->getCondition() != newCondition) {
          // We can remove expression or/and condition
          return new sh::TIntermLoop(
              sh::ELoopFor, nodeAsLoop->getInit(), newCondition, newExpression, nodeAsLoop->getBody());
        }
      }
      return nodeAsLoop;
    }

    auto * nodeAsDecl = node->getAsDeclarationNode();
    if (nodeAsDecl != nullptr && nodeAsDecl->getChildCount() == 0) {
      return nullptr;  // Empty declarations
    }

    auto * nodeAsBlock = node->getAsBlock();
    if (nodeAsBlock) {
      for (auto * nodeAsBlockChild : *nodeAsBlock->getSequence()) {
        if (nodeAsBlockChild->getAsGlobalQualifierDeclarationNode() || nodeAsBlockChild->getAsFunctionDefinition() ||
            nodeAsBlockChild->getAsFunctionPrototypeNode()) {
          return nodeAsBlock;
        }
      }

      // Remove unnecessary block, merge it with the current block.
      for (auto * nodeAsBlockChild : *nodeAsBlock->getSequence()) {
        auto * decl = nodeAsBlockChild->getAsDeclarationNode();
        if (decl) {
          if (decl->getChildCount() == 0) {
            continue;
          }
          for (auto * declChild : *decl->getSequence()) {
            auto * declChildAsSymbol = declChild->getAsSymbolNode();
            if (declChildAsSymbol) {
              this->symbols.renameUnique(declChildAsSymbol);
            } else {
              auto * declBinary = declChild->getAsBinaryNode();
              if (declBinary) {
                auto * left = declBinary->getLeft();
                if (left) {
                  this->symbols.renameUnique(left->getAsSymbolNode());
                }
              }
            }
          }
        }

        queue.push(nodeAsBlockChild);
      }
      return nullptr;
    }

    auto * nodeBinary = node->getAsBinaryNode();
    if (nodeBinary && nodeBinary->getOp() == sh::EOpComma) {
      // Remove comma operators
      queue.push(nodeBinary->getLeft());
      queue.push(nodeBinary->getRight());
      return nullptr;
    }

    return node;
  }
};

void spglsl_treeops_OptimizeBlocks(SpglslAngleCompiler & compiler, sh::TIntermNode * root) {
  AngleAstHasher hasher;
  SpglslOptimizeBlocksTraverser traverser(compiler.symbols, hasher, &compiler.diagnostics);
  for (;;) {
    root->traverse(&traverser);
    if (!traverser.hasChanges) {
      break;
    }
    traverser.hasChanges = false;
  }
}
