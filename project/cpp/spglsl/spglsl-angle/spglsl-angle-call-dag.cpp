#include "spglsl-angle-call-dag.h"

#include <angle/src/compiler/translator/Diagnostics.h>
#include <angle/src/compiler/translator/Symbol.h>

#include "lib/spglsl-angle-node-utils.h"

///////////// SpglslAngleCallDag /////////////

SpglslAngleCallDag::SpglslAngleCallDag() {
}

void SpglslAngleCallDag::clear() {
  this->metadata.clear();
  this->indexOfMainFunctions.clear();
  sh::CallDAG::clear();
}

bool SpglslAngleCallDag::init(sh::TIntermNode * root, sh::TDiagnostics * diagnostics) {
  this->clear();

  switch (sh::CallDAG::init(root, diagnostics)) {
    case sh::CallDAG::INITDAG_RECURSION:
    case sh::CallDAG::INITDAG_UNDEFINED: return false;
    default: break;
  }

  this->metadata.clear();
  this->metadata.resize(this->size());

  for (size_t i = this->size(); i-- > 0;) {
    auto func = this->getRecordFromIndex(i).node->getFunction();
    if (func &&
        (func->isMain() || (func->symbolType() == sh::SymbolType::UserDefined && func->name().beginsWith("main")))) {
      this->indexOfMainFunctions.emplace(i);
    }
  }

  if (this->indexOfMainFunctions.size() == 0) {
    diagnostics->globalError("Missing main()");
    return false;
  }

  this->tagUsedFunctions();

  return true;
}

void SpglslAngleCallDag::tagUsedFunctions() {
  for (size_t i = this->metadata.size(); i-- > 0;) {
    this->metadata[i].used = false;
  }
  if (this->indexOfMainFunctions.size() != 0) {
    for (int idx : this->indexOfMainFunctions) {
      this->_tagUsedFunction(idx);
    }
  }
}

void SpglslAngleCallDag::_tagUsedFunction(size_t index) {
  if (!this->metadata[index].used) {
    this->metadata[index].used = true;
    for (int calleeIndex : this->getRecordFromIndex(index).callees) {
      this->_tagUsedFunction(calleeIndex);
    }
  }
}

void SpglslAngleCallDag::pruneUnusedFunctions(sh::TIntermBlock * root) {
  if (root) {
    sh::TIntermSequence * sequence = root->getSequence();
    if (sequence && !sequence->empty()) {
      SpglslAngleCallDagUnusedPredicate isUnused(*this);
      sequence->erase(std::remove_if(sequence->begin(), sequence->end(), isUnused), sequence->end());
    }
  }
}

///////////// SpglslAngleCallDagUnusedPredicate /////////////

SpglslAngleCallDagUnusedPredicate::SpglslAngleCallDagUnusedPredicate(const SpglslAngleCallDag & callDag) :
    callDag(callDag) {
}

bool SpglslAngleCallDagUnusedPredicate::operator()(sh::TIntermNode * node) {
  const sh::TFunction * func = nodeGetAsFunction(node);
  if (func) {
    size_t callDagIndex = this->callDag.findIndex(func->uniqueId());
    if (callDagIndex == sh::CallDAG::InvalidIndex) {
      return true;
    }
    return !this->callDag.metadata[callDagIndex].used;
  }
  return false;
}