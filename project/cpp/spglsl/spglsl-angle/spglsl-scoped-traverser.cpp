#include "spglsl-scoped-traverser.h"
#include "lib/spglsl-angle-node-utils.h"

SpglslScopedTraverser::SpglslScopedTraverser(SpglslSymbols & symbols) :
    sh::TIntermTraverser(true, true, true, symbols.symbolTable), symbols(symbols) {
  this->scopesStack.reserve(24);
}

void SpglslScopedTraverser::onScopeBegin() {
}

void SpglslScopedTraverser::onSymbolDeclaration(const sh::TSymbol * symbol,
    sh::TIntermNode * node,
    SpglslSymbolDeclarationKind kind) {
}

void SpglslScopedTraverser::onScopeEnd() {
}

void SpglslScopedTraverser::beforeVisitFunctionDefinition(sh::TIntermFunctionDefinition * node) {
}

void SpglslScopedTraverser::beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * proto,
    sh::TIntermFunctionDefinition * definition) {
}

void SpglslScopedTraverser::afterVisitFunctionPrototype(sh::TIntermFunctionPrototype * proto,
    sh::TIntermFunctionDefinition * definition) {
}

void SpglslScopedTraverser::onVisitBlock(sh::TIntermBlock * node) {
  for (sh::TIntermNode * child : *node->getSequence()) {
    if (!nodeBlockIsEmpty(child)) {
      this->traverseNode(child);
    }
  }
}

void SpglslScopedTraverser::onVisitForLoop(sh::TIntermLoop * node, bool infinite) {
  this->traverseNode(node->getInit());
  if (!infinite) {
    this->traverseNode(node->getCondition());
  }
  if (nodeHasSideEffects(node->getExpression())) {
    this->traverseNode(node->getExpression());
  }
  this->traverseNode(node->getBody());
}

void SpglslScopedTraverser::onVisitWhileLoop(sh::TIntermLoop * node) {
  this->traverseNode(node->getCondition());
  this->traverseNode(node->getBody());
}

void SpglslScopedTraverser::onVisitDoWhileLoop(sh::TIntermLoop * node) {
  this->traverseNode(node->getBody());
  this->traverseNode(node->getCondition());
}

bool SpglslScopedTraverser::visitVariableDeclaration(sh::TIntermNode * node, sh::TIntermDeclaration * declarationNode) {
  return true;
}

void SpglslScopedTraverser::traverseNode(sh::TIntermNode * node) {
  if (node) {
    node->traverse(this);
  }
}

/** Called when a new variable scope begins */
void SpglslScopedTraverser::pushScope(sh::TIntermNode * node) {
  this->scopesStack.push_back(node);
  this->onScopeBegin();
}

/** Called when a new variable scope ends */
void SpglslScopedTraverser::popScope() {
  if (!this->scopesStack.empty()) {
    this->onScopeEnd();
    this->scopesStack.pop_back();
  }
}

bool SpglslScopedTraverser::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  auto * proto = node->getFunctionPrototype();
  auto * body = node->getBody();
  auto * parentFunc = this->_currentFunc;
  this->_currentFunc = node;

  this->beforeVisitFunctionDefinition(node);
  if (!body) {
    this->_currentFuncDef = nullptr;
    this->traverseNode(proto);
    return false;
  }

  if (!proto) {
    bool mustPop = this->tryPushScope(body);
    this->traverseNode(proto);
    if (mustPop) {
      this->popScope();
    }
    return false;
  }

  this->_currentFuncDef = node;
  this->traverseNode(proto);
  this->_currentFuncDef = nullptr;

  this->tryPushScope(body);

  this->traverseNode(body);

  this->popScope();

  this->_currentFunc = parentFunc;

  return false;
}

void SpglslScopedTraverser::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  auto * currentFuncDef = this->_currentFuncDef;
  if (currentFuncDef && this->_currentFuncDef->getFunctionPrototype() == node) {
    this->_currentFuncDef = nullptr;
  } else {
    currentFuncDef = nullptr;
  }

  auto * body = currentFuncDef ? currentFuncDef->getBody() : nullptr;

  const auto * func = node->getFunction();

  this->onSymbolDeclaration(func, node, SpglslSymbolDeclarationKind::FunctionDefinition);
  this->beforeVisitFunctionPrototype(node, currentFuncDef);

  if (body) {
    this->tryPushScope(body);

    for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
      const auto * var = func->getParam(i);
      this->onSymbolDeclaration(var, node, SpglslSymbolDeclarationKind::FunctionParameter);
    }
  }

  this->afterVisitFunctionPrototype(node, currentFuncDef);
}

bool SpglslScopedTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  if (visit == sh::PreVisit) {
    size_t childCount = node->getChildCount();
    for (size_t i = 0; i < childCount; ++i) {
      auto * child = node->getChildNode(i);
      if (child) {
        sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
        if (symbolNode) {
          this->onSymbolDeclaration(&symbolNode->variable(), symbolNode, SpglslSymbolDeclarationKind::Variable);
        } else {
          sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
          if (binaryNode) {
            sh::TIntermTyped * left = binaryNode->getLeft();
            if (left) {
              sh::TIntermSymbol * leftAsSym = left->getAsSymbolNode();
              if (leftAsSym) {
                this->onSymbolDeclaration(&leftAsSym->variable(), leftAsSym, SpglslSymbolDeclarationKind::Variable);
              }
            }
          }
        }
        if (!child->getAsSymbolNode() || this->visitVariableDeclaration(child, node)) {
          this->traverseNode(child);
        }
      }
    }
  }
  return false;
}

bool SpglslScopedTraverser::visitBlock(sh::Visit visit, sh::TIntermBlock * node) {
  bool shouldPop = this->tryPushScope(node);
  this->onVisitBlock(node);
  if (shouldPop) {
    this->popScope();
  }
  return false;
}

bool SpglslScopedTraverser::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  sh::TIntermBlock * body = node->getBody();
  sh::TLoopType loopType = node->getType();
  int boolValue = node->getCondition() ? nodeConstantBooleanValue(node->getCondition()) : 1;
  if (boolValue == 1) {
    loopType = sh::ELoopFor;  // A while loop or do-while loop with always a true condition is just a for (;;)
  }

  switch (loopType) {
    case sh::ELoopFor: {
      bool shouldPop = body ? this->tryPushScope(body) : this->tryPushScope(node);
      this->onVisitForLoop(node, boolValue == 1);
      if (shouldPop) {
        this->popScope();
      }
      return false;
    }

    case sh::ELoopWhile: {
      this->onVisitWhileLoop(node);
      return false;
    }

    case sh::ELoopDoWhile: {
      this->onVisitDoWhileLoop(node);
      return false;
    }

    default: return true;
  }
}
