#ifndef _SPGLSL_ANGLE_TREE_OPS_H_
#define _SPGLSL_ANGLE_TREE_OPS_H_

#include "spglsl-get-precisions-traverser.h"

class SpglslAngleCompiler;

namespace sh {
  class TIntermBlock;
};

/** A whole set of optimizations, including many declared in this file */
bool spglsl_treeops_optimize(SpglslAngleCompiler & compiler, sh::TIntermBlock * root);

/** Removes unnecessary or empty blocks, replace comma operators with statements */
void spglsl_treeops_removeUnnecessaryBlocks(SpglslAngleCompiler & compiler, sh::TIntermNode * root);

/** Minification - replace statements with comma operator where possible */
void spglsl_treeops_putCommaOperators(SpglslAngleCompiler & compiler, sh::TIntermNode * root);

#endif
