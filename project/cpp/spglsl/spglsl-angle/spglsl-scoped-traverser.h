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

  /** The function currently being defined */
  inline sh::TIntermFunctionDefinition * getCurrentFunctionDefinition() {
    return this->_fnDefinitionStack.empty() ? nullptr : this->_fnDefinitionStack.top();
  }

  inline sh::TIntermNode * getCurrentScope() const {
    return this->_scopesStack.empty() ? nullptr : *this->_scopesStack.begin();
  }

  virtual void onScopeBegin(sh::TIntermNode * node);
  virtual void onScopeEnd(sh::TIntermNode * node);

  void pushScope(sh::TIntermNode * node);
  void popScope();

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

 private:
  std::stack<sh::TIntermFunctionDefinition *> _fnDefinitionStack;
  std::vector<sh::TIntermNode *> _scopesStack;
};
#endif
