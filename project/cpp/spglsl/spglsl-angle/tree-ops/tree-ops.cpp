#include "tree-ops.h"

#include <angle/src/compiler/translator/tree_ops/FoldExpressions.h>
#include <angle/src/compiler/translator/tree_ops/PruneEmptyCases.h>
#include <angle/src/compiler/translator/tree_ops/PruneNoOps.h>
#include <angle/src/compiler/translator/tree_ops/RemoveArrayLengthMethod.h>
#include <angle/src/compiler/translator/tree_ops/RemoveUnreferencedVariables.h>
#include <angle/src/compiler/translator/tree_ops/SeparateDeclarations.h>
#include <angle/src/compiler/translator/tree_ops/SplitSequenceOperator.h>
#include <angle/src/compiler/translator/tree_util/IntermNodePatternMatcher.h>

#include "../lib/spglsl-angle-ast-hasher.h"
#include "../spglsl-angle-compiler.h"
#include "../spglsl-angle-webgl-output.h"

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
    if (!SplitSequenceOperator(
            &compiler.tCompiler, root, sh::IntermNodePatternMatcher::kArrayLengthMethod, &compiler.symbolTable)) {
      return false;
    }
    if (!RemoveArrayLengthMethod(&compiler.tCompiler, root)) {
      return false;
    }

    compiler.callDag.tagUsedFunctions();
    compiler.callDag.pruneUnusedFunctions(root);

    spglsl_treeops_OptimizeBlocks(compiler, root);

  } while (repeat < 40 && astHasher.computeNodeHashChanged(root, oldAstHash));

  return true;
}
