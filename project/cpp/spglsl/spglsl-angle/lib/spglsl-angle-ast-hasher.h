#ifndef _SPGLSL_ANGLE_AST_HASHER_H_
#define _SPGLSL_ANGLE_AST_HASHER_H_

#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>
#include "../../core/hash-stream.h"

class AngleAstHasher : public sh::TIntermTraverser, public SpglslHasher {
 public:
  explicit AngleAstHasher(sh::TSymbolTable * symbolTable = nullptr);

  AngleAstHasher & traverseNode(sh::TIntermNode * node);

  void visitSymbol(sh::TIntermSymbol * node) override;
  void visitConstantUnion(sh::TIntermConstantUnion * node) override;
  bool visitSwizzle(sh::Visit visit, sh::TIntermSwizzle * node) override;
  bool visitBinary(sh::Visit visit, sh::TIntermBinary * node) override;
  bool visitUnary(sh::Visit visit, sh::TIntermUnary * node) override;
  bool visitTernary(sh::Visit visit, sh::TIntermTernary * node) override;
  bool visitIfElse(sh::Visit visit, sh::TIntermIfElse * node) override;
  bool visitSwitch(sh::Visit visit, sh::TIntermSwitch * node) override;
  bool visitCase(sh::Visit visit, sh::TIntermCase * node) override;
  void visitFunctionPrototype(sh::TIntermFunctionPrototype * node) override;
  bool visitFunctionDefinition(sh::Visit visit, sh::TIntermFunctionDefinition * node) override;
  bool visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) override;
  bool visitBlock(sh::Visit visit, sh::TIntermBlock * node) override;
  bool visitGlobalQualifierDeclaration(sh::Visit visit, sh::TIntermGlobalQualifierDeclaration * node) override;
  bool visitDeclaration(sh::Visit visit, sh::TIntermDeclaration * node) override;
  bool visitLoop(sh::Visit visit, sh::TIntermLoop * node) override;
  bool visitBranch(sh::Visit visit, sh::TIntermBranch * node) override;
  void visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) override;

  SpglslHashValue computeNodeHash(sh::TIntermNode * root);
  SpglslHashValue & computeNodeHash(sh::TIntermNode * root, SpglslHashValue & output);
  bool computeNodeHashChanged(sh::TIntermNode * root, SpglslHashValue & hashValue);
  bool nodesAreTheSame(sh::TIntermNode * a, sh::TIntermNode * b);

 private:
  AngleAstHasher & writeDirective(sh::PreprocessorDirective directive, const char * command);
  AngleAstHasher & writeTMemoryQualifier(const sh::TMemoryQualifier & q);
  AngleAstHasher & writeTTypeLayoutQualifier(const sh::TType & type);
  const sh::TConstantUnion * writeConstantUnion(const sh::TType * type, const sh::TConstantUnion * pConstUnion);
  AngleAstHasher & writeVariableType(const sh::TType & type, bool isFunctionArgument);
  AngleAstHasher & writeVariableDeclarationSymbol(sh::TIntermNode & child);
  AngleAstHasher & writeArraySizes(const sh::TType & type);
  void traverseCodeBlock(sh::TIntermBlock * node);
  void traverseCodeBlock(sh::TIntermBlock * body, bool allowIf);
  void traverseWithParentheses(sh::TIntermNode * node, int operandIndex);
  AngleAstHasher & writeSymbolRef(const sh::TSymbol & symbol);
  AngleAstHasher & writeTypeRef(const sh::TType & type);
};

#endif
