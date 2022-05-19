#ifndef _SPGLSL_ANGLE_MANGLER_H_
#define _SPGLSL_ANGLE_MANGLER_H_

#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "./symbols/spglsl-symbol-info.h"

class SpglslAngleReservedWords : NonCopyable {
 public:
  std::unordered_set<std::string> definitions;
  std::unordered_map<const sh::TSymbol *, std::string> firstPassSymRemap;
  std::unordered_map<const sh::TSymbol *, std::string> symRemap;
  bool isSecondPass;

  explicit SpglslAngleReservedWords();

  std::string getTypeName(const sh::TType & type);

  inline std::string getSymbolName(const sh::TSymbol * symbol) {
    return symbol ? this->getSymbolName(*symbol) : Strings::empty;
  }

  std::string getSymbolName(const sh::TSymbol & symbol);

  bool isReserved(const std::string & name) const;
};

class SpglslAngleReservedWordsTraverser : public sh::TIntermTraverser, NonCopyable {
 public:
  SpglslAngleReservedWords & target;

  explicit SpglslAngleReservedWordsTraverser(SpglslAngleReservedWords & target, sh::TSymbolTable * symbolTable);

  void traverseNode(sh::TIntermNode * node);

  bool add(const sh::ImmutableString & name);
  bool add(const std::string & name);
  void add(const sh::TType & type);
  void add(const sh::TVariable * variable);

  void count();

  bool visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) override;
  bool visitGlobalQualifierDeclaration(sh::Visit visit, sh::TIntermGlobalQualifierDeclaration * node) override;
  void visitFunctionPrototype(sh::TIntermFunctionPrototype * node) override;
  bool visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) override;

  static void exec(SpglslAngleReservedWords & reserved, sh::TSymbolTable * symbolTable, sh::TIntermNode * root);

 protected:
  std::unordered_set<const sh::TStructure *> _declaredStructs;
  std::unordered_set<const sh::TInterfaceBlock *> _declaredInterfaces;
};

class SpglslAngleManglerNameGenerator : NonCopyable {
 public:
  SpglslAngleReservedWords & reserved;
  SpglslAngleManglerNameGenerator * parent;

  std::unordered_map<std::string, std::string> renameMap;
  std::unordered_set<std::string> usedKeys;
  int nextNameIndex;

  SpglslAngleManglerNameGenerator(SpglslAngleReservedWords & reserved, SpglslAngleManglerNameGenerator * parent);

  bool getShortName(const std::string & input, std::string & out);

  std::string generateShortName(int index);

  bool isReserved(const std::string & input);
};

class SpglslAngleManglerTraverser : public sh::TIntermTraverser, NonCopyable {
 public:
  SpglslAngleReservedWords & reserved;

  SpglslAngleManglerNameGenerator * namesRoot;

  explicit SpglslAngleManglerTraverser(SpglslAngleReservedWords & reserved, sh::TSymbolTable * symbolTable);
  ~SpglslAngleManglerTraverser() override;

  bool visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) override;
  bool visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) override;
  bool visitLoop(sh::Visit visit, sh::TIntermLoop * node) override;
  bool visitBlock(sh::Visit visit, sh::TIntermBlock * node) override;

  std::stack<SpglslAngleManglerNameGenerator *> _namesStack;
  SpglslAngleManglerNameGenerator * scopePush();
  bool scopePop();

  void rename(const sh::TSymbol * symbol, bool isParameter);

  static void exec(SpglslAngleReservedWords & reserved, sh::TSymbolTable * symbolTable, sh::TIntermNode * root);
};

#endif
