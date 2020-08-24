#include "tree-ops.h"

#include <angle/src/compiler/translator/tree_ops/FoldExpressions.h>
#include <angle/src/compiler/translator/tree_ops/PruneEmptyCases.h>
#include <angle/src/compiler/translator/tree_ops/PruneNoOps.h>
#include <angle/src/compiler/translator/tree_ops/RecordConstantPrecision.h>
#include <angle/src/compiler/translator/tree_ops/RemoveArrayLengthMethod.h>
#include <angle/src/compiler/translator/tree_ops/RemoveUnreferencedVariables.h>
#include <angle/src/compiler/translator/tree_util/IntermNodePatternMatcher.h>

#include "../lib/spglsl-angle-ast-hasher.h"
#include "../spglsl-angle-compiler.h"
#include "../spglsl-angle-webgl-output.h"
#include "spglsl-angle-update-traverser.h"

bool SpglslOptimizeAngleAst(SpglslAngleCompiler & compiler, sh::TIntermBlock * root) {
  SpglslHashValue oldAstHash;
  AngleAstHasher astHasher(&compiler.symbolTable);
  int repeat = -1;
  do {
    ++repeat;

    /*if (!sh::RecordConstantPrecision(&compiler.tCompiler, root, &compiler.symbolTable)) {
      return false;
    }*/

    if (!sh::RemoveUnreferencedVariables(&compiler.tCompiler, root, &compiler.symbolTable)) {
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

    compiler.callDag.tagUsedFunctions();
    compiler.callDag.pruneUnusedFunctions(root);
  } while (repeat < 32 && astHasher.computeNodeHashChanged(root, oldAstHash));

  return true;
}
