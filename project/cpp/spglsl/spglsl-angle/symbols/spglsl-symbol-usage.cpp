#include <ctype.h>
#include <list>

#include "../spglsl-angle-webgl-output.h"
#include "spglsl-symbol-usage.h"

static auto _cmp_SpglslSymbolUsageInfo(const SpglslSymbolUsageInfo * a, const SpglslSymbolUsageInfo * b) {
  auto af = a->frequency;
  auto bf = b->frequency;
  if (af != bf) {
    return af > bf;
  }
  auto ar = a->isReserved;
  auto br = b->isReserved;
  if (ar != br) {
    return ar < br;
  }
  return a->entry->insertionOrder < b->entry->insertionOrder;
}

class ScopeSymbols {
 public:
  ScopeSymbols * parent;
  sh::TIntermFunctionDefinition * node;
  std::vector<ScopeSymbols *> children;

  std::unordered_set<SpglslSymbolUsageInfo *> declarations;
  std::unordered_set<SpglslSymbolUsageInfo *> usedSymbols;

  inline explicit ScopeSymbols(ScopeSymbols * parent = nullptr, sh::TIntermFunctionDefinition * node = nullptr) :
      parent(parent), node(node) {
  }

  bool isSymbolUsed(SpglslSymbolUsageInfo * symbol) const {
    if (this->usedSymbols.count(symbol) != 0) {
      return true;
    }
    for (const auto * child : this->children) {
      if (child->isSymbolUsed(symbol)) {
        return true;
      }
    }
    return false;
  }

  bool isMangleIdUsed(int mangleId) const {
    if (mangleId <= 0) {
      return false;
    }
    for (const auto * used : this->usedSymbols) {
      if (used->mangleId == mangleId) {
        return true;
      }
    }
    for (const auto * child : this->children) {
      if (child->isMangleIdUsed(mangleId)) {
        return true;
      }
    }
    return false;
  }

  inline void addSymbolUsed(SpglslSymbolUsageInfo * symbol) {
    this->usedSymbols.emplace(symbol);
  }
};

class ScopeSymbolsManager {
 public:
  SpglslSymbolUsage & usage;
  ScopeSymbols * rootScope;
  ScopeSymbols * currentScope = nullptr;
  int declarationsCount = 0;
  std::list<ScopeSymbols> allScopes;

  explicit ScopeSymbolsManager(SpglslSymbolUsage & usage) : usage(usage) {
    this->rootScope = &this->allScopes.emplace_back();
  }

  void beginScope(sh::TIntermFunctionDefinition * node) {
    if (this->currentScope == nullptr) {
      if (this->rootScope == nullptr) {
        this->rootScope = &this->allScopes.emplace_back(nullptr, node);
      }
      this->currentScope = this->rootScope;
    } else {
      auto & newScope = this->allScopes.emplace_back(this->currentScope, node);
      this->currentScope->children.push_back(&newScope);
      this->currentScope = &newScope;
    }
  }

  void addDeclaredSymbol(SpglslSymbolUsageInfo * symbol) {
    auto * scope = this->currentScope;
    if (scope && scope->declarations.emplace(symbol).second) {
      ++this->declarationsCount;
    }
  }

  void endScope() {
    if (this->currentScope) {
      this->currentScope = this->currentScope->parent;
    }
  }
};

class SpglslAngleWebglOutputCounter : public SpglslAngleWebglOutput {
 public:
  SpglslSymbolGenerator * symbolGenerator;
  ScopeSymbolsManager & scopeSymbolsManager;
  SpglslSymbolUsage & usage;

  explicit SpglslAngleWebglOutputCounter(ScopeSymbolsManager & scopeSymbolsManager,
      std::ostream & out,
      SpglslSymbolUsage & usage,
      const SpglslGlslPrecisions & precisions,
      SpglslSymbolGenerator * symbolGenerator = nullptr) :
      SpglslAngleWebglOutput(out, usage.symbols, precisions, false),
      scopeSymbolsManager(scopeSymbolsManager),
      usage(usage),
      symbolGenerator(symbolGenerator) {
  }

  void onScopeBegin() override {
    this->scopeSymbolsManager.beginScope(this->getCurrentFunctionDefinition());
    SpglslAngleWebglOutput::onScopeBegin();
  }

  void beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
      sh::TIntermFunctionDefinition * definition) override {
    SpglslAngleWebglOutput::beforeVisitFunctionPrototype(node, definition);
  }

  void onSymbolDeclaration(const sh::TSymbol * symbol,
      sh::TIntermNode * node,
      SpglslSymbolDeclarationKind kind) override {
    this->scopeSymbolsManager.addDeclaredSymbol(&this->usage.get(symbol));
    SpglslAngleWebglOutput::onSymbolDeclaration(symbol, node, kind);
  }

  std::string getBuiltinTypeName(const sh::TType * type) override {
    auto result = SpglslAngleWebglOutput::getBuiltinTypeName(type);
    if (this->symbolGenerator != nullptr) {
      this->symbolGenerator->addReservedWord(result);
    }
    return result;
  }

  const std::string & getSymbolName(const sh::TSymbol * symbol) override {
    auto & symentry = this->usage.get(symbol);
    if (symentry.isReserved) {
      return SpglslAngleWebglOutput::getSymbolName(symbol);  // Reserved.
    }
    ++symentry.frequency;
    this->scopeSymbolsManager.currentScope->addSymbolUsed(&symentry);
    return Strings::empty;
  }

  void onScopeEnd() override {
    SpglslAngleWebglOutput::onScopeEnd();
    this->scopeSymbolsManager.endScope();
  }
};

////////////////////////////////////////
//    Class SpglslMangleIdOptimizer
////////////////////////////////////////

inline std::string _functionOverloadKey(const sh::TFunction * fn) {
  std::string newName;
  auto paramCount = fn->getParamCount();
  for (size_t i = 0u; i < paramCount; ++i) {
    newName += fn->getParam(i)->getType().getMangledName();
  }
  return newName;
}

void generateMangledOverloads(ScopeSymbols & scope,
    std::unordered_map<SpglslSymbolUsageInfo *, std::vector<SpglslSymbolUsageInfo *>> & output) {
  std::vector<std::vector<SpglslSymbolUsageInfo *>> functionsByArgs;
  std::unordered_map<std::string, uint32_t> argsMap;
  size_t maxGroupSize = 0;

  for (auto * entry : scope.declarations) {
    const auto * symbol = entry->entry->symbol;
    if (!symbol->isFunction()) {
      continue;
    }
    if (entry->mangleId < 0 || !entry->entry) {
      continue;
    }
    auto & id = argsMap[_functionOverloadKey(static_cast<const sh::TFunction *>(symbol))];
    if (id == 0) {
      functionsByArgs.emplace_back();
      id = functionsByArgs.size();
    }
    auto & list = functionsByArgs[id - 1];
    list.push_back(entry);
    if (list.size() > maxGroupSize) {
      maxGroupSize = list.size();
    }
  }
  if (!functionsByArgs.empty()) {
    for (size_t i = 0; i < maxGroupSize; ++i) {
      SpglslSymbolUsageInfo * selectedOverload = nullptr;

      std::vector<SpglslSymbolUsageInfo *> tmpOverloads;
      for (auto & list : functionsByArgs) {
        if (i < list.size()) {
          tmpOverloads.push_back(list[i]);
        }
      }
      if (tmpOverloads.size() > 1) {
        auto * selectedOverload =
            *std::min_element(tmpOverloads.begin(), tmpOverloads.end(), _cmp_SpglslSymbolUsageInfo);

        for (auto * item : tmpOverloads) {
          if (item == selectedOverload) {
            continue;
          }

          output[selectedOverload].push_back(item);

          selectedOverload->frequency += item->frequency;
          item->frequency = 0;
        }
      }
    }
  }
}

void mangleScopeDeclarations(ScopeSymbolsManager & scopeSymbolsManager, ScopeSymbols & scope) {
  std::unordered_map<SpglslSymbolUsageInfo *, std::vector<SpglslSymbolUsageInfo *>> overloads;
  generateMangledOverloads(scope, overloads);

  std::vector<SpglslSymbolUsageInfo *> sortedDeclarations(scope.declarations.begin(), scope.declarations.end());
  std::sort(sortedDeclarations.begin(), sortedDeclarations.end(), _cmp_SpglslSymbolUsageInfo);

  int lastMangleId = 1;
  for (auto * declInfo : sortedDeclarations) {
    if (declInfo->mangleId != 0 || declInfo->frequency == 0) {
      continue;  // Already renamed.
    }
    bool renamed = false;
    for (; lastMangleId <= scopeSymbolsManager.declarationsCount; ++lastMangleId) {
      if (!scope.isMangleIdUsed(lastMangleId)) {
        auto newMangleId = lastMangleId++;
        declInfo->mangleId = newMangleId;
        renamed = true;

        auto declOverloads = overloads.find(declInfo);
        if (declOverloads != overloads.end()) {
          for (auto * overload : declOverloads->second) {
            overload->mangleId = newMangleId;
          }
        }
        break;
      }
    }
    if (!renamed) {
      break;  // No more avaialble ids.
    }
  }
}

////////////////////////////////////////
//    Class SpglslSymbolUsage
////////////////////////////////////////

SpglslSymbolUsage::SpglslSymbolUsage(SpglslSymbols & symbols) : symbols(symbols) {
}

void SpglslSymbolUsage::load(sh::TIntermBlock * root,
    const SpglslGlslPrecisions & precisions,
    SpglslSymbolGenerator * generator) {
  ScopeSymbolsManager scopeSymbolsManager(*this);

  {
    std::stringstream ss;
    SpglslAngleWebglOutputCounter counter(scopeSymbolsManager, ss, *this, precisions);
    root->traverse(&counter);
    if (generator) {
      generator->load(ss.str());
    }
  }

  std::vector<SpglslSymbolUsageInfo *> tmpSorted;
  tmpSorted.reserve(this->map.size());
  for (auto & kv : this->map) {
    if (!kv.second.isReserved) {
      tmpSorted.push_back(&kv.second);
    }
  }

  std::sort(tmpSorted.begin(), tmpSorted.end(), _cmp_SpglslSymbolUsageInfo);

  std::unordered_map<const sh::TSymbol *, int> newMangleIds;

  this->sorted.clear();
  this->sorted.reserve(tmpSorted.size());
  for (auto & entry : tmpSorted) {
    this->sorted.push_back(entry);
  }

  if (generator) {
    for (auto & scope : scopeSymbolsManager.allScopes) {
      mangleScopeDeclarations(scopeSymbolsManager, scope);
    }

    for (const auto & kv : this->symbols._map) {
      auto & entry = this->get(kv.first);
      if (entry.isReserved || entry.mangleId <= 0) {
        generator->addReservedWord(kv.second.symbolName);
      }
    }
  }
}

////////////////////////////////////////
//    Class SpglslSymbolGenerator
////////////////////////////////////////

inline bool charLess(char a, char b) {
  bool alow = islower(a) != 0;
  bool blow = islower(b) != 0;
  if (alow != blow) {
    return alow;
  }
  return a < b;
}

SpglslSymbolGenerator::SpglslSymbolGenerator(SpglslSymbolUsage & usage) : usage(usage) {
  this->_additionalReservedWords.emplace(Strings::empty);
  this->_additionalReservedWords.emplace("PI");
}

bool SpglslSymbolGenerator::isReservedWord(const std::string & word) const {
  return this->_additionalReservedWords.count(word) > 0 || spglslIsWordReserved(word);
}

void SpglslSymbolGenerator::addReservedWord(const std::string & word) {
  this->_additionalReservedWords.emplace(word);
}

void SpglslSymbolGenerator::load(const std::string & text) {
  std::unordered_map<char, uint32_t> asciiAndNums;
  std::unordered_map<char, uint32_t> ascii;
  std::unordered_map<std::string, uint32_t> words;

  std::string one;
  one.resize(1);

  std::string two;
  two.resize(2);

  for (char c = 'a'; c <= 'z'; ++c) {
    ascii[c] = 1;
    asciiAndNums[c] = 1;

    one[0] = c;
    words[one]++;
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    ascii[c] = 1;
    asciiAndNums[c] = 1;

    one[0] = c;
    words[one]++;
  }
  for (char c = '0'; c <= '9'; ++c) {
    asciiAndNums[c] = 1;
  }
  char prevChar = 0;

  for (size_t i = 0; i != text.size(); ++i) {
    const char c = text[i];
    if (isalpha(c)) {
      ++ascii[c];
      ++asciiAndNums[c];

      one[0] = c;
      words[one]++;

    } else if (isalnum(c)) {
      ++asciiAndNums[c];
    }

    if (isalpha(prevChar) && isalnum(c)) {
      two[0] = prevChar;
      two[1] = c;
      words[two]++;
    }

    prevChar = c;
  }

  std::vector<std::pair<char, uint32_t>> asciiSorted(ascii.begin(), ascii.end());
  std::sort(asciiSorted.begin(), asciiSorted.end(), [](const auto & a, const auto & b) {
    return a.second > b.second || (a.second == b.second && charLess(a.first, b.first));
  });

  this->chars.resize(asciiSorted.size());
  for (size_t i = 0; i != asciiSorted.size(); ++i) {
    this->chars[i] = asciiSorted[i].first;
  }

  std::vector<std::pair<char, uint32_t>> asciiAndNumsSorted(asciiAndNums.begin(), asciiAndNums.end());
  std::sort(asciiAndNumsSorted.begin(), asciiAndNumsSorted.end(), [](const auto & a, const auto & b) {
    return a.second > b.second || (a.second == b.second && charLess(a.first, b.first));
  });

  this->charsAndNumbers.resize(asciiAndNumsSorted.size());
  for (size_t i = 0; i != asciiAndNumsSorted.size(); ++i) {
    this->charsAndNumbers[i] = asciiAndNumsSorted[i].first;
  }

  std::vector<std::pair<std::string, uint32_t>> wordsSorted(words.begin(), words.end());
  std::sort(wordsSorted.begin(), wordsSorted.end(), [](const auto & a, const auto & b) {
    if (a.first.size() != b.first.size()) {
      return a.first.size() < b.first.size();
    }
    if (a.second != b.second) {
      return a.second > b.second;
    }
    if (a.first[0] != b.first[0]) {
      return charLess(a.first[0], b.first[0]);
    }
    return a.first.size() > 1 && charLess(a.first[1], b.first[1]);
  });

  this->words.clear();
  this->words.reserve(wordsSorted.size());
  for (const auto & kv : wordsSorted) {
    if (!this->isReservedWord(kv.first)) {
      this->words.push_back(kv.first);
    }
  }
}

const std::string & SpglslSymbolGenerator::getOrCreateMangledName(int mangleId) {
  auto & result = this->_mangleMap[mangleId];

  if (result.empty()) {
    for (;;) {
      if (this->_usedWords < this->words.size()) {
        result = this->words[this->_usedWords++];
      } else {
        auto index = _genCounter++;
        std::ostringstream ss;
        ss.put(this->chars[index % this->chars.size()]);
        index = floor((double)index / (double)this->chars.size());
        while (index > 0) {
          index -= 1;
          ss.put(this->charsAndNumbers[index % this->charsAndNumbers.size()]);
          index = floor((double)index / (double)this->charsAndNumbers.size());
        }
        result = ss.str();
      }
      if (this->isReservedWord(result)) {
        continue;
      }
      if (this->_usedNames.emplace(result).second) {
        break;
      }
    };
  }

  return result;
}
