#ifndef _SPGLSL_SYMBOL_INFO_
#define _SPGLSL_SYMBOL_INFO_

#include <emscripten/bind.h>
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
#include "../../spglsl-compile-options.h"

bool spglslIsValidIdentifier(const std::string & str);
bool spglslIsWordReserved(const std::string & word);

class SpglslSymbolInfo : NonCopyable {
 public:
  const sh::TSymbol * symbol = nullptr;
  std::string symbolName;
  std::string renamed;
  bool mustBeRenamedUnique = false;

  inline int uniqueId() const {
    const auto * symbol = this->symbol;
    return symbol ? symbol->uniqueId().get() : 0;
  }
};

class SpglslSymbols {
 private:
  uint32_t _uniqueCounter = 0;

 public:
  sh::TSymbolTable * symbolTable;
  std::unordered_map<const sh::TSymbol *, SpglslSymbolInfo> _map;
  SpglslCompileOptions & compileOptions;

  SpglslSymbols(sh::TSymbolTable * symbolTable, SpglslCompileOptions & compileOptions);

  SpglslSymbolInfo & get(const sh::TSymbol * symbol);

  bool has(const sh::TSymbol * symbol) const;

  /** Mark a variable as function parameter */
  SpglslSymbolInfo & declareParameter(const sh::TVariable * variable);

  inline const std::string & getName(const sh::TSymbol * symbol, bool renamed = true) {
    auto & info = this->get(symbol);
    if (renamed && !info.renamed.empty()) {
      return info.renamed;
    }
    if (info.symbolName.empty()) {
      return Strings::empty;
    }
    if (info.mustBeRenamedUnique) {
      info.mustBeRenamedUnique = false;
      std::stringstream ss;
      ss << info.symbolName << "SP_" << std::hex << ++this->_uniqueCounter << "_";
      return info.renamed = ss.str();
    }
    return info.symbolName;
  }

  bool isReserved(const SpglslSymbolInfo & info) const;

  void renameUnique(const sh::TIntermSymbol * symbolNode);
  void renameUnique(const sh::TSymbol * symbol);
};

#endif
