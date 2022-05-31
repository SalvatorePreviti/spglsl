#include "tree-ops.h"

#include <angle/src/compiler/translator/tree_ops/FoldExpressions.h>
#include <angle/src/compiler/translator/tree_ops/PruneEmptyCases.h>
#include <angle/src/compiler/translator/tree_ops/PruneNoOps.h>
#include <angle/src/compiler/translator/tree_ops/RemoveArrayLengthMethod.h>
#include <angle/src/compiler/translator/tree_ops/RemoveUnreferencedVariables.h>
#include <angle/src/compiler/translator/tree_ops/SeparateDeclarations.h>
#include <angle/src/compiler/translator/tree_ops/SplitSequenceOperator.h>
#include <angle/src/compiler/translator/tree_util/IntermNodePatternMatcher.h>
#include <angle/src/compiler/translator/tree_util/IntermRebuild.h>

#include "../lib/spglsl-angle-ast-hasher.h"
#include "../lib/spglsl-angle-node-utils.h"
#include "../spglsl-angle-compiler.h"
#include "../spglsl-angle-webgl-output.h"

class XRebuilder : public sh::TIntermRebuild {
 public:
  explicit XRebuilder(SpglslAngleCompiler & compiler) : sh::TIntermRebuild(compiler.tCompiler, true, false) {
  }

  static sh::TIntermNode * _optimizeSingleNode(sh::TIntermNode * input) {
    sh::TIntermNode * current = input;
    while (current) {
      auto * next = current;
      auto * unary = current->getAsUnaryNode();
      if (unary) {
        next = _optimizeUnary(*unary);
      } else {
        auto * ternary = current->getAsTernaryNode();
        if (ternary) {
          next = _optimizeTernary(*ternary);
        }
      }
      if (current == next) {
        break;
      }
      current = next;
    }
    return current;
  }

  static sh::TIntermNode * _optimizeUnary(sh::TIntermUnary & node) {
    auto op = node.getOp();
    auto * operand = node.getOperand();
    if (op == sh::EOpPositive) {
      return operand;
    }

    if (op == sh::EOpNegative) {
      auto * doubleNegation = nodeGetAsUnaryNode(operand, sh::EOpNegative);
      if (doubleNegation) {
        return doubleNegation->getOperand();
      }
    }

    if (op == sh::EOpBitwiseNot) {
      auto * doubleNegation = nodeGetAsUnaryNode(operand, sh::EOpBitwiseNot);
      if (doubleNegation) {
        return doubleNegation->getOperand();
      }
    }

    if (op == sh::EOpLogicalNot) {
      auto * doubleNegation = nodeGetAsUnaryNode(operand, sh::EOpLogicalNot);
      if (doubleNegation) {
        return doubleNegation->getOperand();
      }

      auto * negationOfEqual = nodeGetAsBinaryNode(operand, sh::EOpEqual);
      if (negationOfEqual) {
        return new sh::TIntermBinary(sh::EOpNotEqual, negationOfEqual->getLeft(), negationOfEqual->getRight());
      }

      auto * negationOfEqualComponents = nodeGetAsBinaryNode(operand, sh::EOpEqualComponentWise);
      if (negationOfEqualComponents) {
        return new sh::TIntermBinary(
            sh::EOpNotEqualComponentWise, negationOfEqualComponents->getLeft(), negationOfEqualComponents->getRight());
      }

      auto * negationOfNotEqual = nodeGetAsBinaryNode(operand, sh::EOpNotEqual);
      if (negationOfNotEqual) {
        return new sh::TIntermBinary(sh::EOpEqual, negationOfNotEqual->getLeft(), negationOfNotEqual->getRight());
      }

      auto * negationOfNotEqualComponents = nodeGetAsBinaryNode(operand, sh::EOpNotEqualComponentWise);
      if (negationOfNotEqualComponents) {
        return new sh::TIntermBinary(sh::EOpEqualComponentWise, negationOfNotEqualComponents->getLeft(),
            negationOfNotEqualComponents->getRight());
      }

      auto * negationOfLessThan = nodeGetAsBinaryNode(operand, sh::EOpLessThan);
      if (negationOfLessThan) {
        return new sh::TIntermBinary(
            sh::EOpGreaterThanEqual, negationOfLessThan->getLeft(), negationOfLessThan->getRight());
      }

      auto * negationOfLessThanComponents = nodeGetAsBinaryNode(operand, sh::EOpLessThanComponentWise);
      if (negationOfLessThanComponents) {
        return new sh::TIntermBinary(sh::EOpGreaterThanEqualComponentWise, negationOfLessThanComponents->getLeft(),
            negationOfLessThanComponents->getRight());
      }

      auto * negationOfGreaterThan = nodeGetAsBinaryNode(operand, sh::EOpGreaterThan);
      if (negationOfGreaterThan) {
        return new sh::TIntermBinary(
            sh::EOpLessThanEqual, negationOfGreaterThan->getLeft(), negationOfGreaterThan->getRight());
      }

      auto * negationOfGreaterThanComponents = nodeGetAsBinaryNode(operand, sh::EOpGreaterThanComponentWise);
      if (negationOfGreaterThanComponents) {
        return new sh::TIntermBinary(sh::EOpLessThanEqualComponentWise, negationOfGreaterThanComponents->getLeft(),
            negationOfGreaterThanComponents->getRight());
      }

      auto * negationOfLessThanEqual = nodeGetAsBinaryNode(operand, sh::EOpLessThanEqual);
      if (negationOfLessThanEqual) {
        return new sh::TIntermBinary(
            sh::EOpGreaterThan, negationOfLessThanEqual->getLeft(), negationOfLessThanEqual->getRight());
      }

      auto * negationOfLessThanEqualComponents = nodeGetAsBinaryNode(operand, sh::EOpLessThanEqualComponentWise);
      if (negationOfLessThanEqualComponents) {
        return new sh::TIntermBinary(sh::EOpGreaterThanComponentWise, negationOfLessThanEqualComponents->getLeft(),
            negationOfLessThanEqualComponents->getRight());
      }

      auto * negationOfGreaterThanEqual = nodeGetAsBinaryNode(operand, sh::EOpGreaterThanEqual);
      if (negationOfGreaterThanEqual) {
        return new sh::TIntermBinary(
            sh::EOpLessThan, negationOfGreaterThanEqual->getLeft(), negationOfGreaterThanEqual->getRight());
      }

      auto * negationOfGreaterThanEqualComponents = nodeGetAsBinaryNode(operand, sh::EOpGreaterThanEqualComponentWise);
      if (negationOfGreaterThanEqualComponents) {
        return new sh::TIntermBinary(sh::EOpLessThanComponentWise, negationOfGreaterThanEqualComponents->getLeft(),
            negationOfGreaterThanEqualComponents->getRight());
      }
    }

    return &node;
  }

  static sh::TIntermNode * _optimizeTernary(sh::TIntermTernary & node) {
    auto * condition = node.getCondition();

    auto * conditionNegation = nodeGetAsUnaryNode(condition, sh::EOpLogicalNot);
    if (conditionNegation) {
      return new sh::TIntermTernary(
          conditionNegation->getOperand(), node.getFalseExpression(), node.getTrueExpression());
    }

    return &node;
  }

  PreResult visitUnaryPre(sh::TIntermUnary & node) override {
    return _optimizeSingleNode(&node);
  }

  PreResult visitBinaryPre(sh::TIntermBinary & node) override {
    return _optimizeSingleNode(&node);
  }

  PreResult visitTernaryPre(sh::TIntermTernary & node) override {
    return _optimizeSingleNode(&node);
  }
};

bool spglsl_treeops_optimize(SpglslAngleCompiler & compiler, sh::TIntermBlock * root) {
  SpglslHashValue oldAstHash;
  AngleAstHasher astHasher(&compiler.symbolTable);
  int repeat = -1;

  do {
    ++repeat;

    if (!sh::RemoveUnreferencedVariables(&compiler.tCompiler, root, &compiler.symbolTable)) {
      return false;
    }
    if (!sh::SeparateDeclarations(&compiler.tCompiler, root, &compiler.symbolTable)) {
      return false;
    }
    if (!sh::PruneEmptyCases(&compiler.tCompiler, root)) {
      return false;
    }
    if (!sh::PruneNoOps(&compiler.tCompiler, root, &compiler.symbolTable)) {
      return false;
    }
    if (!sh::FoldExpressions(&compiler.tCompiler, root, &compiler.diagnostics)) {
      return false;
    }
    if (!RemoveArrayLengthMethod(&compiler.tCompiler, root)) {
      return false;
    }

    compiler.callDag.tagUsedFunctions();
    compiler.callDag.pruneUnusedFunctions(root);

    spglsl_treeops_OptimizeBlocks(compiler, root);

    XRebuilder rebuilder(compiler);
    if (!rebuilder.rebuildRoot(*root)) {
      return false;
    }

  } while (repeat < 50 && astHasher.computeNodeHashChanged(root, oldAstHash));

  return true;
}
