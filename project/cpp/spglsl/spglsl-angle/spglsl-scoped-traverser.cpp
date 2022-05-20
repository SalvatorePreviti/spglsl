#include "spglsl-scoped-traverser.h"
#include "lib/spglsl-angle-node-utils.h"

SpglslScopedTraverser::SpglslScopedTraverser(SpglslSymbols & symbols) :
    sh::TIntermTraverser(true, true, true, symbols.symbolTable), symbols(symbols) {
  this->_scopesStack.reserve(32);
  this->scopesStack.reserve(24);
}

void SpglslScopedTraverser::onScopeBegin(sh::TIntermNode * node) {
}

void SpglslScopedTraverser::onSymbolDeclaration(const sh::TSymbol * symbol,
    sh::TIntermNode * node,
    SpglslSymbolDeclarationKind kind) {
}

void SpglslScopedTraverser::onScopeEnd(sh::TIntermNode * node) {
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
  sh::TIntermSequence * sequence = node->getSequence();
  if (sequence) {
    for (sh::TIntermNode * child : *sequence) {
      if (!nodeBlockIsEmpty(child)) {
        this->traverseNode(child);
      }
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
  if (this->getCurrentScope() != node) {
    this->onScopeBegin(node);
    this->scopesStack.push_back(node);
  }
  this->_scopesStack.push_back(node);
}

/** Called when a new variable scope ends */
void SpglslScopedTraverser::popScope() {
  if (!this->_scopesStack.empty()) {
    auto * node = *this->_scopesStack.end();
    this->_scopesStack.pop_back();
    if (this->getCurrentScope() != node) {
      this->scopesStack.pop_back();
      this->onScopeEnd(node);
    }
  }
}

bool SpglslScopedTraverser::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  auto * proto = node->getFunctionPrototype();
  auto * body = node->getBody();

  this->beforeVisitFunctionDefinition(node);
  this->_fnDefinitionStack.push(node);
  if (body) {
    if (proto) {
      this->traverseNode(proto);
    } else {
      this->pushScope(body);
    }
    this->traverseNode(body);
    if (this->getCurrentScope() == body) {
      this->popScope();
    }
  } else {
    this->traverseNode(proto);
    if (this->getCurrentScope() == node) {
      this->popScope();
    }
  }
  this->_fnDefinitionStack.pop();

  return false;
}

void SpglslScopedTraverser::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  auto * currentFunctionDefinition = this->getCurrentFunctionDefinition();
  bool isCurrentFunctionDefinitionPrototype =
      currentFunctionDefinition != nullptr && currentFunctionDefinition->getFunctionPrototype() == node;

  auto * def = isCurrentFunctionDefinitionPrototype ? currentFunctionDefinition : nullptr;
  this->beforeVisitFunctionPrototype(node, def);

  const auto * func = node->getFunction();

  if (isCurrentFunctionDefinitionPrototype) {
    this->onSymbolDeclaration(func, node, SpglslSymbolDeclarationKind::FunctionDefinition);

    auto * body = currentFunctionDefinition->getBody();
    if (body) {
      this->pushScope(body);
    } else {
      this->pushScope(currentFunctionDefinition);
    }

    for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
      const auto * var = func->getParam(i);
      this->onSymbolDeclaration(var, node, SpglslSymbolDeclarationKind::FunctionParameter);
    }

  } else {
    this->onSymbolDeclaration(func, node, SpglslSymbolDeclarationKind::FunctionPrototype);
  }
  this->afterVisitFunctionPrototype(node, def);
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
  this->pushScope(node);
  this->onVisitBlock(node);
  this->popScope();
  return false;
}

bool SpglslScopedTraverser::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  sh::TIntermBlock * body = node->getBody();
  sh::TLoopType loopType = node->getType();
  if (loopType == sh::ELoopFor && !node->getInit() && !nodeHasSideEffects(node->getExpression())) {
    loopType = sh::ELoopWhile;  // For without init and expression is just a while
  }
  int boolValue = node->getCondition() ? nodeConstantBooleanValue(node->getCondition()) : 1;
  if (boolValue == 1) {
    loopType = sh::ELoopFor;  // A while loop or do-while loop with always a true condition is just a for (;;)
  }

  switch (loopType) {
    case sh::ELoopFor:
      if (body) {
        this->pushScope(body);
      } else {
        this->pushScope(node);
      }
      this->onVisitForLoop(node, boolValue == 1);
      this->popScope();
      return false;

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
