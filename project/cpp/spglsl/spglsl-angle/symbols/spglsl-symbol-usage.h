#ifndef _SPGLSL_SYMBOL_USAGE_
#define _SPGLSL_SYMBOL_USAGE_

#include "../lib/spglsl-glsl-precisions.h"
#include "spglsl-symbol-info.h"

struct SpglslSymbolUsageInfo {
  SpglslSymbolInfo * entry = nullptr;
  uint32_t frequency = 0;
  int mangleId = -1;
};

class SpglslSymbolGenerator;

class SpglslSymbolUsage {
 public:
  SpglslSymbols & symbols;
  std::unordered_map<const sh::TSymbol *, SpglslSymbolUsageInfo> map;
  std::vector<SpglslSymbolUsageInfo *> sorted;

  explicit SpglslSymbolUsage(SpglslSymbols & symbols);

  void load(sh::TIntermBlock * root,
      const SpglslGlslPrecisions & precisions,
      SpglslSymbolGenerator * generator = nullptr);

  inline SpglslSymbolUsageInfo & get(const sh::TSymbol * symbol) {
    auto & found = this->map[symbol];
    if (!found.entry) {
      auto & info = this->symbols.get(symbol);
      found.entry = &info;
      found.mangleId = info.isReserved() ? -1 : (int)info.insertionOrder;
    }
    return found;
  }
};

class SpglslSymbolGenerator {
 public:
  SpglslSymbolUsage & usage;

  std::string chars;
  std::string charsAndNumbers;
  std::vector<std::string> words;

  bool isReservedWord(const std::string & word) const;
  void addReservedWord(const std::string & word);

  explicit SpglslSymbolGenerator(SpglslSymbolUsage & usage);

  void load(const std::string & text);

  const std::string & getOrCreateMangledName(int mangleId);

 private:
  size_t _usedWords = 0;
  size_t _genCounter = 0;
  std::unordered_map<int, std::string> _mangleMap;
  std::unordered_set<std::string> _usedNames;
  std::unordered_set<std::string> _additionalReservedWords;
};

#endif
