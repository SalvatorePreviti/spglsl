#include <ctype.h>
#include <bit>

#include "../spglsl-angle-webgl-output.h"
#include "spglsl-symbol-usage.h"

#include <iostream>

class SpglslAngleWebglOutputCounter : public SpglslAngleWebglOutput {
 public:
  SpglslSymbolUsage & usage;

  explicit SpglslAngleWebglOutputCounter(std::ostream & out,
      SpglslSymbolUsage & usage,
      const SpglslGlslPrecisions & precisions) :
      SpglslAngleWebglOutput(out, usage.symbols, precisions, false), usage(usage) {
  }

  std::string getBuiltinTypeName(const sh::TType * type) override {
    auto result = SpglslAngleWebglOutput::getBuiltinTypeName(type);
    this->usage.addReservedWord(result);
    return result;
  }

  const std::string & getSymbolName(const sh::TSymbol * symbol) override {
    auto & symentry = this->symbols.get(symbol);
    if (!symentry.symbol || symentry.symbolName.empty()) {
      return Strings::empty;
    }
    if (this->symbols.getIsReserved(symentry)) {
      return symentry.symbolName;
    }
    auto & entry = this->usage.map[symbol];
    if (!entry.entry) {
      entry.entry = &symentry;
      entry.insertionOrder = (uint32_t)(this->usage.map.size());
    }
    ++entry.frequency;
    for (auto & scope : this->scopesStack) {
      auto & scopeSymbols = this->usage.scopesUsedSymbols[scope];
      if (!scopeSymbols.usedSymbols.emplace(symbol).second || scopeSymbols.declaredSymbols.count(symbol) != 0) {
        break;
      }
    }
    return Strings::empty;
  }

  void onSymbolDeclaration(const sh::TSymbol * symbol,
      sh::TIntermNode * node,
      SpglslSymbolDeclarationKind kind) override {
    this->usage.scopesUsedSymbols[this->getCurrentScope()].declaredSymbols.emplace(symbol);
  }
};

SpglslSymbolUsage::SpglslSymbolUsage(SpglslSymbols & symbols) : symbols(symbols) {
}

void SpglslSymbolUsage::load(sh::TIntermBlock * root,
    const SpglslGlslPrecisions & precisions,
    SpglslSymbolGenerator * generator) {
  std::stringstream ss;
  SpglslAngleWebglOutputCounter counter(ss, *this, precisions);

  root->traverse(&counter);

  this->_additionalReservedWords.emplace(Strings::empty);
  for (const auto & kv : this->symbols._map) {
    if (this->symbols.getIsReserved(kv.second)) {
      this->_additionalReservedWords.emplace(kv.second.symbolName);
    }
  }

  if (generator) {
    generator->load(ss.str());
  }

  this->sorted.clear();
  this->sorted.reserve(this->map.size());
  for (auto & kv : this->map) {
    this->sorted.push_back(&kv.second);
  }
  std::sort(
      this->sorted.begin(), this->sorted.end(), [](const SpglslSymbolUsageInfo * a, const SpglslSymbolUsageInfo * b) {
        return a->frequency > b->frequency || (a->frequency == b->frequency && a->insertionOrder < b->insertionOrder);
      });
}

bool SpglslSymbolUsage::isReservedWord(const std::string & word) const {
  return this->_additionalReservedWords.count(word) > 0 || spglslIsWordReserved(word);
}

void SpglslSymbolUsage::addReservedWord(const std::string & word) {
  this->_additionalReservedWords.emplace(word);
}

class GenMangleIdTraverser : public SpglslScopedTraverser {
 public:
  SpglslSymbolUsage & usage;

  std::stack<int> scopeStack;

  int maxId;

  explicit GenMangleIdTraverser(SpglslSymbolUsage & usage) :
      SpglslScopedTraverser(usage.symbols), usage(usage), maxId(0) {
    this->scopeStack.push(0);
  }

  void assignMangleId(const sh::TSymbol * symbol) {
    auto & info = this->symbols.get(symbol);
    if (info.mangleId == -1) {
      if (this->symbols.getIsReserved(info)) {
        info.mangleId = -2;
      } else {
        int newId = this->scopeStack.top()++;
        info.mangleId = newId;
        if (newId > this->maxId) {
          this->maxId = newId;
        }
      }
    }
  }

  void onScopeBegin(sh::TIntermNode * node) override {
    this->scopeStack.push(this->scopeStack.top());
  }

  void onSymbolDeclaration(const sh::TSymbol * symbol,
      sh::TIntermNode * node,
      SpglslSymbolDeclarationKind kind) override {
    this->assignMangleId(symbol);
  }

  void onScopeEnd(sh::TIntermNode * node) override {
    if (!this->scopeStack.empty()) {
      this->scopeStack.pop();
    }
  }
};

int SpglslSymbolUsage::genMangleIds(sh::TIntermBlock * root) {
  this->symbols.clearMangleId();
  GenMangleIdTraverser traverser(*this);
  root->traverse(&traverser);
  return traverser.maxId;
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
