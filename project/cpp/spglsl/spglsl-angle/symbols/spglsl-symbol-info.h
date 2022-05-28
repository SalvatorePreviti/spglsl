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

class SpglslSymbolInfo : NonCopyable {
 public:
  const sh::TSymbol * symbol = nullptr;
  std::string symbolName;
  std::string renamed;

  bool isReserved() const;

  inline int uniqueId() const {
    const auto * symbol = this->symbol;
    return symbol ? symbol->uniqueId().get() : 0;
  }
};

class SpglslSymbols {
 public:
  sh::TSymbolTable * symbolTable;
  std::unordered_map<const sh::TSymbol *, SpglslSymbolInfo> _map;

  explicit SpglslSymbols(sh::TSymbolTable * symbolTable);

  SpglslSymbolInfo & get(const sh::TSymbol * symbol);

  bool has(const sh::TSymbol * symbol) const;

  /** Mark a variable as function parameter */
  SpglslSymbolInfo & declareParameter(const sh::TVariable * variable);

  inline const std::string & getName(const sh::TSymbol * symbol) {
    const auto & info = this->get(symbol);
    return info.renamed.empty() ? info.symbolName : info.renamed;
  }

  bool isSymbolReserved(SpglslSymbolInfo & syminfo);
};

#endif
