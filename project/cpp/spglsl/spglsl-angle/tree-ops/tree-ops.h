#ifndef _SPGLSL_ANGLE_TREE_OPS_H_
#define _SPGLSL_ANGLE_TREE_OPS_H_

#include "spglsl-get-precisions-traverser.h"

class SpglslAngleCompiler;

namespace sh {
  class TIntermBlock;
};

bool SpglslOptimizeAngleAst(SpglslAngleCompiler & compiler, sh::TIntermBlock * root);

#endif