#ifndef _SPGLSL_SCOPED_TRAVERSER_
#define _SPGLSL_SCOPED_TRAVERSER_

#include <angle/src/compiler/translator/ExtensionBehavior.h>
#include <angle/src/compiler/translator/Pragma.h>
#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

#include <sstream>
#include <unordered_set>

#include "../core/string-utils.h"
#include "lib/spglsl-glsl-writer.h"
#include "symbols/spglsl-symbol-info.h"

enum class SpglslSymbolDeclarationKind { FunctionPrototype, FunctionDefinition, FunctionParameter, Variable };

class SpglslScopedTraverser : public sh::TIntermTraverser {
 public:
  SpglslSymbols & symbols;

  explicit SpglslScopedTraverser(SpglslSymbols & symbols);

  bool visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) final;
  void visitFunctionPrototype(sh::TIntermFunctionPrototype * node) final;
  bool visitBlock(sh::Visit visit, sh::TIntermBlock * node) final;
  bool visitLoop(sh::Visit visit, sh::TIntermLoop * node) final;
  bool visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) final;

  virtual bool visitVariableDeclaration(sh::TIntermNode * node, sh::TIntermDeclaration * declarationNode);

  void traverseNode(sh::TIntermNode * node);

 protected:
  std::vector<sh::TIntermNode *> scopesStack;

  inline sh::TIntermNode * getCurrentScope() const {
    auto size = this->scopesStack.size();
    return size ? this->scopesStack[size - 1] : nullptr;
  }

  inline sh::TIntermFunctionDefinition * getCurrentFunctionDefinition() const {
    return this->_currentFunc;
  }

  virtual void onScopeBegin();
  virtual void onScopeEnd();

  virtual void beforeVisitFunctionDefinition(sh::TIntermFunctionDefinition * node);
  virtual void beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * proto,
      sh::TIntermFunctionDefinition * definition);
  virtual void afterVisitFunctionPrototype(sh::TIntermFunctionPrototype * proto,
      sh::TIntermFunctionDefinition * definition);
  virtual void onVisitBlock(sh::TIntermBlock * node);
  virtual void onVisitForLoop(sh::TIntermLoop * loop, bool infinite);
  virtual void onVisitWhileLoop(sh::TIntermLoop * loop);
  virtual void onVisitDoWhileLoop(sh::TIntermLoop * node);
  virtual void onSymbolDeclaration(const sh::TSymbol * symbol,
      sh::TIntermNode * node,
      SpglslSymbolDeclarationKind kind);

  void pushScope(sh::TIntermNode * node);
  void popScope();

  inline bool tryPushScope(sh::TIntermNode * node) {
    if (this->getCurrentScope() == node) {
      return false;
    }
    this->pushScope(node);
    return true;
  }

 private:
  sh::TIntermFunctionDefinition * _currentFuncDef = nullptr;
  sh::TIntermFunctionDefinition * _currentFunc = nullptr;
};
#endif
