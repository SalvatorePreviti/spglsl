#ifndef _SPGLSL_ANGLE_FUNCTION_DAG_H_
#define _SPGLSL_ANGLE_FUNCTION_DAG_H_

#include <angle/src/compiler/translator/CallDAG.h>

#include <unordered_set>
#include <vector>

class SpglslAngleFunctionMetadata {
 public:
  bool used;
  inline SpglslAngleFunctionMetadata() : used(false) {
  }
};

class SpglslAngleCallDag : public sh::CallDAG {
 public:
  std::unordered_set<int> indexOfMainFunctions;

  std::vector<SpglslAngleFunctionMetadata> metadata;

  SpglslAngleCallDag();

  bool init(sh::TIntermNode * root, sh::TDiagnostics * diagnostics);
  void clear();

  void tagUsedFunctions();
  void pruneUnusedFunctions(sh::TIntermBlock * root);

 private:
  void _tagUsedFunction(size_t index);
};

class SpglslAngleCallDagUnusedPredicate {
 public:
  const SpglslAngleCallDag & callDag;
  explicit SpglslAngleCallDagUnusedPredicate(const SpglslAngleCallDag & callDag);
  bool operator()(sh::TIntermNode * node);
};

#endif
