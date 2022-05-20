#ifndef _SPGLSL_SYMBOL_INFO_
#define _SPGLSL_SYMBOL_INFO_

#include <unordered_map>

#include <angle/src/compiler/translator/BaseTypes.h>
#include <angle/src/compiler/translator/ImmutableString.h>
#include <angle/src/compiler/translator/IntermNode.h>
#include <angle/src/compiler/translator/Operator_autogen.h>
#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/Types.h>
#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

#include "../../core/non-copyable.h"
#include "../../core/string-utils.h"

bool spglslIsValidIdentifier(const std::string & str);
bool spglslIsWordReserved(const std::string & word);

class SpglslSymbolInfo {
 public:
  const sh::TSymbol * symbol = nullptr;
  std::string symbolName;
  std::string renamed;
  int mangleId = -1;

 private:
  int _isReserved = -1;
  friend class SpglslSymbols;
};

class SpglslSymbols {
 public:
  sh::TSymbolTable * symbolTable;
  std::unordered_map<const sh::TSymbol *, SpglslSymbolInfo> _map;

  explicit SpglslSymbols(sh::TSymbolTable * symbolTable);

  bool getIsReserved(const sh::TSymbol * symbol);
  bool getIsReserved(const SpglslSymbolInfo & entry);
  SpglslSymbolInfo & setIsReserved(const sh::TSymbol * symbol, bool value = true);

  SpglslSymbolInfo & get(const sh::TSymbol * symbol);

  bool has(const sh::TSymbol * symbol) const;

  /** Mark a variable as function parameter */
  SpglslSymbolInfo & declareParameter(const sh::TVariable * variable);

  /** Resets all mangleId to -1 */
  void clearMangleId();

  inline const std::string & getName(const sh::TSymbol * symbol) {
    const auto & info = this->get(symbol);
    return info.renamed.empty() ? info.symbolName : info.renamed;
  }
};

#endif
