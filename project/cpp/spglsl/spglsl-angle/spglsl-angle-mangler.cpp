#include "spglsl-angle-mangler.h"

#include <ios>
#include <sstream>

/////////////// SpglslAngleReservedWords ///////////////

SpglslAngleReservedWords::SpglslAngleReservedWords() : isSecondPass(false) {
}

bool SpglslAngleReservedWords::isReserved(const std::string & name) const {
  return this->definitions.count(name) != 0 || spglslIsWordReserved(name);
}

std::string SpglslAngleReservedWords::getTypeName(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtStruct && type.getStruct()) {
    return this->getSymbolName(type.getStruct());
  }
  if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    return this->getSymbolName(type.getInterfaceBlock());
  }
  return type.getBuiltInTypeNameString();
}

std::string SpglslAngleReservedWords::getSymbolName(const sh::TSymbol & symbol) {
  if (!symbol.uniqueId().get() || symbol.symbolType() == sh::SymbolType::Empty) {
    return Strings::empty;
  }
  if (symbol.isFunction() && static_cast<const sh::TFunction &>(symbol).isMain()) {
    return "main";
  }

  const auto found = this->symRemap.find(&symbol);
  if (found != this->symRemap.end()) {
    return found->second;
  }

  const auto found1 = this->firstPassSymRemap.find(&symbol);
  if (found1 != this->firstPassSymRemap.end()) {
    return found1->second;
  }

  const sh::ImmutableString & n = symbol.name();
  return std::string(n.data(), n.length());
}

/////////////// SpglslAngleReservedWordsTraverser ///////////////

void SpglslAngleReservedWordsTraverser::exec(SpglslAngleReservedWords & reserved,
    sh::TSymbolTable * symbolTable,
    sh::TIntermNode * root) {
  SpglslAngleReservedWordsTraverser traverser(reserved, symbolTable);
  root->traverse(&traverser);
}

SpglslAngleReservedWordsTraverser::SpglslAngleReservedWordsTraverser(SpglslAngleReservedWords & target,
    sh::TSymbolTable * symbolTable) :
    sh::TIntermTraverser(true, false, false, symbolTable), target(target) {
}

bool SpglslAngleReservedWordsTraverser::add(const sh::ImmutableString & name) {
  return this->add(name.data());
}

bool SpglslAngleReservedWordsTraverser::add(const std::string & name) {
  return !spglslIsWordReserved(name) && this->target.definitions.emplace(name).second;
}

void SpglslAngleReservedWordsTraverser::add(const sh::TType & type) {
  this->add(this->target.getTypeName(type));
  if (type.getBasicType() == sh::EbtStruct && type.getStruct() &&
      this->_declaredStructs.emplace(type.getStruct()).second) {
    for (const auto & field : type.getStruct()->fields()) {
      if (field) {
        auto * fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
        this->add(field->name());
      }
    }
  } else if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock() &&
      this->_declaredInterfaces.emplace(type.getInterfaceBlock()).second) {
    for (const auto & field : type.getInterfaceBlock()->fields()) {
      if (field) {
        this->add(field->name());
        auto fieldType = field->type();
        if (fieldType) {
          this->add(*fieldType);
        }
      }
    }
  }
}

void SpglslAngleReservedWordsTraverser::add(const sh::TVariable * variable) {
  if (variable) {
    this->add(this->target.getSymbolName(variable));
    this->add(variable->getType());
  }
}

bool SpglslAngleReservedWordsTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  size_t childCount = node->getChildCount();
  for (size_t i = 0; i < childCount; ++i) {
    auto * child = node->getChildNode(i);
    if (child) {
      sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
      if (symbolNode) {
        this->add(&symbolNode->variable());
        continue;
      }
      sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
      if (binaryNode) {
        sh::TIntermTyped * left = binaryNode->getLeft();
        if (left) {
          sh::TIntermSymbol * leftAsSym = left->getAsSymbolNode();
          if (leftAsSym) {
            this->add(&leftAsSym->variable());
          } else {
            this->add(left->getType());
          }
        }
        continue;
      }
    }
  }
  return true;
}

bool SpglslAngleReservedWordsTraverser::visitGlobalQualifierDeclaration(sh::Visit visit,
    sh::TIntermGlobalQualifierDeclaration * node) {
  sh::TIntermSymbol * symbolNode = node->getSymbol();
  if (symbolNode) {
    this->add(&symbolNode->variable());
  }
  return true;
}

void SpglslAngleReservedWordsTraverser::visitFunctionPrototype(sh::TIntermFunctionPrototype * node) {
  const sh::TFunction * func = node->getFunction();
  if (func) {
    this->add(this->target.getSymbolName(func));
    this->add(func->getReturnType());
    for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
      this->add(func->getParam(i));
    }
  }
}

bool SpglslAngleReservedWordsTraverser::visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) {
  if (node->getOp() == sh::EOpConstruct) {
    this->add(node->getType());
  }
  return true;
}

/////////////// SpglslAngleManglerNameGenerator ///////////////

static const char * _leadingChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char * _allChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

static const int _leadingCharsLen = (int)strlen(_leadingChars);
static const int _allCharsLen = (int)strlen(_allChars);

SpglslAngleManglerNameGenerator::SpglslAngleManglerNameGenerator(SpglslAngleReservedWords & reserved,
    SpglslAngleManglerNameGenerator * parent) :
    nextNameIndex(parent ? parent->nextNameIndex + 1 : 0), reserved(reserved), parent(nullptr) {
}

std::string SpglslAngleManglerNameGenerator::generateShortName(int index) {
  std::ostringstream ss;
  ss.put(_leadingChars[index % _leadingCharsLen]);
  index = floor((double)index / (double)_leadingCharsLen);
  while (index > 0) {
    index -= 1;
    ss.put(_allChars[index % _allCharsLen]);
    index = floor((double)index / (double)_allCharsLen);
  }
  return ss.str();
}

bool SpglslAngleManglerNameGenerator::getShortName(const std::string & input, std::string & out) {
  if (input.length() <= 1 && this->reserved.isSecondPass) {
    return false;
  }
  const auto found = this->renameMap.find(input);
  if (found != this->renameMap.end()) {
    out = found->second;
    return true;
  }
  for (;;) {
    std::string n = this->generateShortName(this->nextNameIndex);
    if (this->isReserved(n)) {
      ++this->nextNameIndex;
      continue;
    }
    if (!this->reserved.isSecondPass) {
      n = "___" + n;
    } else if (n[0] != '_' && n.length() >= input.length()) {
      return false;  // Too long, no point using this.
    }
    ++this->nextNameIndex;
    if (this->usedKeys.emplace(n).second) {
      renameMap.emplace(input, n);
      out = n;
      return true;
    }
  }
}

bool SpglslAngleManglerNameGenerator::isReserved(const std::string & input) {
  if (this->renameMap.find(input) != this->renameMap.end()) {
    return true;
  }
  if (this->usedKeys.find(input) != this->usedKeys.end()) {
    return true;
  }
  if (this->parent) {
    return this->parent->isReserved(input);
  }
  return this->reserved.isReserved(input);
}

/////////////// SpglslAngleManglerTraverser ///////////////

SpglslAngleManglerTraverser::SpglslAngleManglerTraverser(SpglslAngleReservedWords & reserved,
    sh::TSymbolTable * symbolTable) :
    sh::TIntermTraverser(true, false, true, symbolTable),
    reserved(reserved),
    namesRoot(new SpglslAngleManglerNameGenerator(reserved, nullptr)) {
  this->_namesStack.push(this->namesRoot);
}

void SpglslAngleManglerTraverser::exec(SpglslAngleReservedWords & reserved,
    sh::TSymbolTable * symbolTable,
    sh::TIntermNode * root) {
  SpglslAngleManglerTraverser traverser(reserved, symbolTable);
  root->traverse(&traverser);
}

SpglslAngleManglerTraverser::~SpglslAngleManglerTraverser() {
  while (!this->_namesStack.empty()) {
    this->scopePop();
  }
}

SpglslAngleManglerNameGenerator * SpglslAngleManglerTraverser::scopePush() {
  auto * result = new SpglslAngleManglerNameGenerator(this->reserved, this->_namesStack.top());
  this->_namesStack.push(result);
  return result;
}

bool SpglslAngleManglerTraverser::scopePop() {
  if (this->_namesStack.empty()) {
    return false;
  }
  delete this->_namesStack.top();
  this->_namesStack.pop();
  return true;
}

bool SpglslAngleManglerTraverser::visitLoop(sh::Visit visit, sh::TIntermLoop * node) {
  if (node->getType() == sh::ELoopFor) {
    if (visit == sh::PreVisit) {
      this->scopePush();
    } else if (visit == sh::PostVisit) {
      this->scopePop();
    }
  }
  return true;
}

bool SpglslAngleManglerTraverser::visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) {
  const sh::TFunction * func = node->getFunction();

  sh::TIntermBlock * body = node->getBody();

  this->rename(func, false);

  if (body) {
    this->scopePush();

    if (func) {
      for (size_t i = 0, len = func->getParamCount(); i < len; ++i) {
        this->rename(func->getParam(i), true);
      }
    }

    if (body) {
      body->traverse(this);
    }

    this->scopePop();
  }

  return false;
}

bool SpglslAngleManglerTraverser::visitBlock(sh::Visit visit, sh::TIntermBlock * node) {
  if (this->getCurrentTraversalDepth() == 0) {
    return true;
  }
  if (visit == sh::PreVisit) {
    this->scopePush();
  } else if (visit == sh::PostVisit) {
    this->scopePop();
  }
  return true;
}

bool SpglslAngleManglerTraverser::visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) {
  if (visit == sh::PreVisit) {
    size_t childCount = node->getChildCount();
    for (size_t i = 0; i < childCount; ++i) {
      auto * child = node->getChildNode(i);
      if (child) {
        sh::TIntermSymbol * symbolNode = child->getAsSymbolNode();
        if (symbolNode) {
          this->rename(&symbolNode->variable(), false);
          continue;
        }
        sh::TIntermBinary * binaryNode = child->getAsBinaryNode();
        if (binaryNode) {
          sh::TIntermTyped * left = binaryNode->getLeft();
          if (left) {
            sh::TIntermSymbol * leftAsSym = left->getAsSymbolNode();
            if (leftAsSym) {
              this->rename(&leftAsSym->variable(), false);
            }
          }
          continue;
        }
      }
    }
  }
  return true;
}

void SpglslAngleManglerTraverser::rename(const sh::TSymbol * symbol, bool isParameter) {
  if (!symbol) {
    return;
  }

  if (symbol->symbolType() != sh::SymbolType::UserDefined && symbol->symbolType() != sh::SymbolType::AngleInternal) {
    return;
  }

  if (symbol->isFunction()) {
    const auto * func = static_cast<const sh::TFunction *>(symbol);
    if (func->isMain() || func->name().beginsWith("main")) {
      return;
    }
    if (spglslIsWordReserved(this->reserved.getSymbolName(func))) {
      return;
    }
  } else if (symbol->isVariable()) {
    const auto * var = static_cast<const sh::TVariable *>(symbol);
    if (var->isInterfaceBlock()) {
      return;
    }

    const sh::TType & type = var->getType();
    if (!isParameter && type.getQualifier() != sh::EvqTemporary && type.getQualifier() != sh::EvqGlobal &&
        type.getQualifier() != sh::EvqConst) {
      return;
    }

    if (spglslIsWordReserved(this->reserved.getSymbolName(var))) {
      return;
    }
  } else {
    return;  // struct and interface blocks still not supported.
  }

  std::string renamed;
  if (this->reserved.isSecondPass) {
    if (this->reserved.symRemap.count(symbol) != 0) {
      return;
    }

    if (this->_namesStack.top()->getShortName(this->reserved.getSymbolName(symbol), renamed)) {
      this->reserved.symRemap.emplace(symbol, renamed);
    }
  } else {
    if (this->reserved.firstPassSymRemap.count(symbol) != 0) {
      return;
    }

    if (this->_namesStack.top()->getShortName(this->reserved.getSymbolName(symbol), renamed)) {
      this->reserved.firstPassSymRemap.emplace(symbol, renamed);
    }
  }
}
