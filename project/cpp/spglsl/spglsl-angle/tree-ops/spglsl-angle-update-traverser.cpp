#include "spglsl-angle-update-traverser.h"
#include <string.h>
#include <xlocale.h>

#include "../spglsl-angle-compiler.h"

SpglslAngleUpdateTraverser::SpglslAngleUpdateTraverser(SpglslAngleCompiler & compiler,
    bool preVisitIn,
    bool inVisitIn,
    bool postVisitIn) :
    sh::TIntermTraverser(preVisitIn, inVisitIn, postVisitIn, &compiler.symbolTable), compiler(compiler) {
}

bool SpglslAngleUpdateTraverser::Apply() {
  return this->Apply(this->compiler.body);
}

bool SpglslAngleUpdateTraverser::Apply(sh::TIntermNode * node) {
  if (!node) {
    return true;
  }

  this->traverseNode(node);

  return this->updateTree(&this->compiler.tCompiler, node);
}

void SpglslAngleUpdateTraverser::traverseNode(sh::TIntermNode * node) {
  if (node) {
    node->traverse(this);
  }
}

void SpglslAngleUpdateTraverser::queueAggregateRemove(sh::TIntermAggregateBase * parent, sh::TIntermNode * node) {
  if (parent && node) {
    this->mMultiReplacements.emplace_back(NodeReplaceWithMultipleEntry(parent, node, sh::TIntermSequence()));
  }
}

void SpglslAngleUpdateTraverser::queueAggregateReplace(sh::TIntermAggregateBase * parent,
    sh::TIntermNode * node,
    sh::TIntermSequence * replacement) {
  if (!replacement) {
    this->queueAggregateRemove(parent, node);
  } else if (parent && node) {
    this->mMultiReplacements.emplace_back(NodeReplaceWithMultipleEntry(parent, node, std::move(*replacement)));
  }
}

void SpglslAngleUpdateTraverser::queueAggregateReplace(sh::TIntermAggregateBase * parent,
    sh::TIntermNode * node,
    sh::TIntermSequence && replacement) {
  this->mMultiReplacements.emplace_back(NodeReplaceWithMultipleEntry(parent, node, std::move(replacement)));
}
