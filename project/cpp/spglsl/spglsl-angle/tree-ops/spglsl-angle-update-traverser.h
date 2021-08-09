#ifndef _SPGLSL_ANGLE_UPDATE_TRAVERSER_H_
#define _SPGLSL_ANGLE_UPDATE_TRAVERSER_H_

#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

#include <unordered_set>

#include "../../core/non-copyable.h"
#include "../lib/spglsl-angle-node-utils.h"

class SpglslAngleCompiler;

class SpglslAngleUpdateTraverser : protected sh::TIntermTraverser, NonCopyable {
 public:
  SpglslAngleCompiler & compiler;

  using TIntermTraverser::NodeReplaceWithMultipleEntry;
  using TIntermTraverser::OriginalNode;
  using TIntermTraverser::ScopedNodeInTraversalPath;

  explicit SpglslAngleUpdateTraverser(SpglslAngleCompiler & compiler,
      bool preVisitIn = true,
      bool inVisitIn = true,
      bool postVisitIn = true);

  /** Run the traverser to the root */
  bool Apply();

  /** Run the traverser to the given node */
  bool Apply(sh::TIntermNode * node);

  void queueAggregateRemove(sh::TIntermAggregateBase * parent, sh::TIntermNode * node);

  void queueAggregateReplace(sh::TIntermAggregateBase * parent,
      sh::TIntermNode * node,
      sh::TIntermSequence * replacement);

  void queueAggregateReplace(sh::TIntermAggregateBase * parent,
      sh::TIntermNode * node,
      sh::TIntermSequence && replacement);

 protected:
  /** Traverse the given node */
  void traverseNode(sh::TIntermNode * node);
};

#endif