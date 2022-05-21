#include <ctype.h>
#include <bit>

#include "../spglsl-angle-webgl-output.h"
#include "spglsl-symbol-usage.h"

bool _isSymReserved(const SpglslSymbolInfo & entry);

class ScopeSymbols {
 public:
  ScopeSymbols * parent = nullptr;
  std::vector<ScopeSymbols *> children;

  std::unordered_set<const sh::TSymbol *> declarations;
  std::unordered_set<const sh::TSymbol *> usedSymbols;

  bool isSymbolUsed(const sh::TSymbol * symbol) const {
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

  void addSymbolUsed(const sh::TSymbol * symbol) {
    this->usedSymbols.emplace(symbol);
  }

  bool isSymbolDeclared(const sh::TSymbol * symbol) const {
    return this->declarations.count(symbol) != 0 || (this->parent && this->parent->declarations.count(symbol) != 0);
  }

  void addDeclaredSymbol(const sh::TSymbol * symbol) {
    if (!this->isSymbolDeclared(symbol)) {
      this->declarations.emplace(symbol);
    }
    this->usedSymbols.emplace(symbol);
  }

  void renameUsedSymbol(const sh::TSymbol * source, const sh::TSymbol * target) {
    if (this->usedSymbols.erase(source) != 0) {
      this->usedSymbols.emplace(target);
    }
    for (auto * child : this->children) {
      child->renameUsedSymbol(source, target);
    }
  }
};

typedef std::unordered_map<sh::TIntermNode *, ScopeSymbols> ScopeSymbolsMap;

class SpglslAngleWebglOutputCounter : public SpglslAngleWebglOutput {
 public:
  SpglslSymbolUsage & usage;
  std::string _dummyVariable = "$";

  ScopeSymbolsMap & usedSymbols;
  ScopeSymbols * currentUsedScope = nullptr;

  explicit SpglslAngleWebglOutputCounter(ScopeSymbolsMap & usedSymbols,
      std::ostream & out,
      SpglslSymbolUsage & usage,
      const SpglslGlslPrecisions & precisions) :
      SpglslAngleWebglOutput(out, usage.symbols, precisions, false), usedSymbols(usedSymbols), usage(usage) {
  }

  void onScopeBegin() override {
    auto * scope = this->getCurrentScope();
    auto & n = this->usedSymbols[scope];
    this->currentUsedScope->children.push_back(&n);
    n.parent = this->currentUsedScope;
    this->currentUsedScope = &n;
    SpglslAngleWebglOutput::onScopeBegin();
  }

  void beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
      sh::TIntermFunctionDefinition * definition) override {
    SpglslAngleWebglOutput::beforeVisitFunctionPrototype(node, definition);
  }

  void onSymbolDeclaration(const sh::TSymbol * symbol,
      sh::TIntermNode * node,
      SpglslSymbolDeclarationKind kind) override {
    this->currentUsedScope->addDeclaredSymbol(symbol);
    SpglslAngleWebglOutput::onSymbolDeclaration(symbol, node, kind);
  }

  std::string getBuiltinTypeName(const sh::TType * type) override {
    auto result = SpglslAngleWebglOutput::getBuiltinTypeName(type);
    this->usage.addReservedWord(result);
    return result;
  }

  const std::string & getSymbolName(const sh::TSymbol * symbol) override {
    auto & symentry = this->symbols.get(symbol);
    auto & entry = this->usage.map[symbol];
    if (!entry.entry) {
      entry.entry = &symentry;
    }
    ++entry.frequency;

    if (symentry.symbol) {
      this->currentUsedScope->addSymbolUsed(symbol);
    }

    if (symentry.mangleId == -2) {
      return SpglslAngleWebglOutput::getSymbolName(symbol);
    }

    if (_isSymReserved(symentry)) {
      symentry.mangleId = -2;
      return SpglslAngleWebglOutput::getSymbolName(symbol);
    }
    return this->_dummyVariable;
  }

  void onScopeEnd() override {
    SpglslAngleWebglOutput::onScopeEnd();
    this->currentUsedScope = this->currentUsedScope->parent;
  }
};

class SpglslMangleIdAssigner : public SpglslScopedTraverser {
 public:
  SpglslSymbolUsage & usage;

  ScopeSymbolsMap & usedSymbols;
  std::unordered_map<const sh::TSymbol *, int> newMangleIds;

  SpglslMangleIdAssigner(ScopeSymbolsMap & usedSymbols, SpglslSymbolUsage & usage) :
      SpglslScopedTraverser(usage.symbols), usage(usage), usedSymbols(usedSymbols) {
  }

  void beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
      sh::TIntermFunctionDefinition * definition) override {
  }

  void renameSymbolsInScope() {
    auto * node = this->getCurrentScope();
    auto & scope = this->usedSymbols[node];

    std::vector<SpglslSymbolInfo *> sortedDeclarations;
    sortedDeclarations.reserve(scope.declarations.size());
    for (const auto * declaration : scope.declarations) {
      sortedDeclarations.push_back(&this->usage.symbols.get(declaration));
    }
    std::sort(sortedDeclarations.begin(), sortedDeclarations.end(),
        [](const SpglslSymbolInfo * a, const SpglslSymbolInfo * b) { return a->mangleId < b->mangleId; });

    size_t candidateIndex = 0;

    for (auto * declInfo : sortedDeclarations) {
      int mangleId = declInfo->mangleId;

      if (mangleId < 0) {
        continue;  // Symbol is reserved.
      }

      const auto * declSym = declInfo->symbol;

      for (; candidateIndex < usage.sorted.size(); ++candidateIndex) {
        auto * candidate = usage.sorted[candidateIndex];
        if (candidate->entry->mangleId >= mangleId) {
          break;  // Nothing better found
        }

        const auto * candidateSym = candidate->entry->symbol;
        if (!scope.isSymbolUsed(candidateSym)) {
          this->newMangleIds[declSym] = candidate->entry->mangleId;
          scope.renameUsedSymbol(declSym, candidateSym);
          ++candidateIndex;
          break;  // Symbol renamed.
        }
      }
    }
  }

  void onScopeBegin() override {
    renameSymbolsInScope();
  }
};

SpglslSymbolUsage::SpglslSymbolUsage(SpglslSymbols & symbols) : symbols(symbols) {
}

#include <iostream>

void SpglslSymbolUsage::load(sh::TIntermBlock * root,
    const SpglslGlslPrecisions & precisions,
    SpglslSymbolGenerator * generator) {
  ScopeSymbolsMap scopesUsedSymbols;

  std::stringstream ss;

  SpglslAngleWebglOutputCounter counter(scopesUsedSymbols, ss, *this, precisions);

  root->traverse(&counter);

  this->_additionalReservedWords.emplace(Strings::empty);
  for (const auto & kv : this->symbols._map) {
    if (_isSymReserved(kv.second)) {
      this->addReservedWord(kv.second.symbolName);
    }
  }

  std::cout << "GENERATED:" << ss.str() << std::endl;

  if (generator) {
    generator->load(ss.str());
  }

  this->symbols.clearMangleId();

  std::vector<SpglslSymbolUsageInfo *> tmpSorted;
  tmpSorted.reserve(this->map.size());
  for (auto & kv : this->map) {
    if (kv.second.entry) {
      if (!_isSymReserved(*kv.second.entry)) {
        tmpSorted.push_back(&kv.second);
      } else {
        kv.second.entry->mangleId = -2;
        this->addReservedWord(this->symbols.getName(kv.second.entry->symbol));
      }
    }
  }

  std::sort(tmpSorted.begin(), tmpSorted.end(), [](const SpglslSymbolUsageInfo * a, const SpglslSymbolUsageInfo * b) {
    return a->frequency > b->frequency ||
        (a->frequency == b->frequency && a->entry->insertionOrder < b->entry->insertionOrder);
  });

  this->sorted.clear();
  this->sorted.reserve(tmpSorted.size());
  for (auto & sym : tmpSorted) {
    if (sym->entry->mangleId == -1) {
      sym->entry->mangleId = (int)this->sorted.size();
      this->sorted.push_back(sym);
    }
  }

  SpglslMangleIdAssigner mangleIdAssigner(scopesUsedSymbols, *this);

  root->traverse(&mangleIdAssigner);

  for (auto & sym : this->sorted) {
    auto & entry = sym->entry;
    auto found = mangleIdAssigner.newMangleIds.find(entry->symbol);
    if (found != mangleIdAssigner.newMangleIds.end()) {
      entry->mangleId = found->second;
    }
  }
}

bool SpglslSymbolUsage::isReservedWord(const std::string & word) const {
  return this->_additionalReservedWords.count(word) > 0 || spglslIsWordReserved(word);
}

void SpglslSymbolUsage::addReservedWord(const std::string & word) {
  this->_additionalReservedWords.emplace(word);
}

////////////////////////////////////////
//    Class SpglslSymbolGenerator
////////////////////////////////////////

inline bool charLess(char a, char b) {
  bool aalpha = isalpha(a) != 0;
  bool balpha = isalpha(b) != 0;
  if (aalpha != balpha) {
    return aalpha;
  }
  bool alow = islower(a) != 0;
  bool blow = islower(b) != 0;
  if (alow != blow) {
    return alow;
  }
  auto ba = __builtin_popcount(a);
  auto bb = __builtin_popcount(b);
  return ba != bb ? ba < bb : a < b;
}

SpglslSymbolGenerator::SpglslSymbolGenerator(SpglslSymbolUsage & usage) : usage(usage) {
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
    if (!this->usage.isReservedWord(kv.first)) {
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
      if (this->usage.isReservedWord(result)) {
        continue;
      }
      if (this->_usedNames.emplace(result).second) {
        break;
      }
    };
  }

  return result;
}

bool _isSymReserved(const SpglslSymbolInfo & entry) {
  const auto * symbol = entry.symbol;

  if (!symbol || entry.symbolName.empty()) {
    return true;
  }

  auto st = symbol->symbolType();
  if (st != sh::SymbolType::UserDefined && st != sh::SymbolType::AngleInternal) {
    return true;
  }

  if (symbol->isFunction()) {
    const auto * func = static_cast<const sh::TFunction *>(symbol);
    return (func->isMain() || func->name().beginsWith("main"));
  }

  if (symbol->isVariable()) {
    const auto * var = static_cast<const sh::TVariable *>(symbol);
    if (var->isInterfaceBlock()) {
      return true;
    }

    const sh::TType & type = var->getType();

    switch (type.getQualifier()) {
      case sh::EvqParamIn:
      case sh::EvqParamOut:
      case sh::EvqParamInOut:
      case sh::EvqParamConst:
      case sh::EvqTemporary:
      case sh::EvqGlobal:
      case sh::EvqConst: return false;

      default: return true;
    }
  }

  // Note: interfaces are considered always reserved.

  return !symbol->isStruct();
}
