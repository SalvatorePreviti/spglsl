#ifndef _SPGLSL_GET_PRECISIONS_TRAVERSER_H_
#define _SPGLSL_GET_PRECISIONS_TRAVERSER_H_

#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>
#include <unordered_set>

#include "../../core/non-copyable.h"

class SpglslAngleCompiler;

class SpglslGetPrecisionsTraverser : protected sh::TIntermTraverser, NonCopyable {
 public:
  sh::TPrecision floatPrecision;
  sh::TPrecision intPrecision;

  explicit SpglslGetPrecisionsTraverser();

  void reset();

  void traverseNode(sh::TIntermNode * node);

  void add(sh::TBasicType basicType, sh::TPrecision precision);
  void add(const sh::TType & type);
  void add(const sh::TVariable * variable);

  void count();

 protected:
  bool visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) override;
  bool visitGlobalQualifierDeclaration(sh::Visit visit, sh::TIntermGlobalQualifierDeclaration * node) override;
  bool visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) override;
  bool visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) override;

 private:
  std::unordered_set<const sh::TStructure *> _declaredStructs;
  std::unordered_set<const sh::TInterfaceBlock *> _declaredInterfaces;

  int _floatPrecisionLowp;
  int _floatPrecisionMediump;
  int _floatPrecisionHighp;

  int _intPrecisionLowp;
  int _intPrecisionMediump;
  int _intPrecisionHighp;
};

#endif