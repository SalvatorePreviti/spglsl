#ifndef _SPGLSL_SYMBOL_USAGE_
#define _SPGLSL_SYMBOL_USAGE_

#include "../lib/spglsl-glsl-precisions.h"
#include "spglsl-symbol-info.h"

struct SpglslSymbolUsageInfo {
  SpglslSymbolInfo * entry = nullptr;
  uint32_t frequency = 0;
  uint32_t insertionOrder = 0;
};

class SpglslSymbolGenerator;

class SpglslScopeSymbols {
 public:
  std::unordered_set<const sh::TSymbol *> usedSymbols;
  std::unordered_set<const sh::TSymbol *> declaredSymbols;
};

class SpglslSymbolUsage {
 public:
  SpglslSymbols & symbols;
  std::unordered_map<const sh::TSymbol *, SpglslSymbolUsageInfo> map;
  std::vector<SpglslSymbolUsageInfo *> sorted;
  std::unordered_map<sh::TIntermNode *, SpglslScopeSymbols> scopesUsedSymbols;

  explicit SpglslSymbolUsage(SpglslSymbols & symbols);

  void load(sh::TIntermBlock * root,
      const SpglslGlslPrecisions & precisions,
      SpglslSymbolGenerator * generator = nullptr);

  bool isReservedWord(const std::string & word) const;
  void addReservedWord(const std::string & word);

 private:
  std::unordered_set<std::string> _additionalReservedWords;
};

class SpglslSymbolGenerator {
 public:
  SpglslSymbolUsage & usage;

  std::string chars;
  std::string charsAndNumbers;
  std::vector<std::string> words;

  explicit SpglslSymbolGenerator(SpglslSymbolUsage & usage);

  void load(const std::string & text);

  const std::string & getOrCreateMangledName(int mangleId);

 private:
  size_t _usedWords = 0;
  size_t _genCounter = 0;
  std::unordered_map<int, std::string> _mangleMap;
  std::unordered_set<std::string> _usedNames;
};

#endif
