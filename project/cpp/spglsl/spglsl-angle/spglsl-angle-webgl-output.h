#ifndef _SPGLSL_ANGLE_WEBGL_OUTPUT_H_
#define _SPGLSL_ANGLE_WEBGL_OUTPUT_H_

#include <angle/src/compiler/translator/ExtensionBehavior.h>
#include <angle/src/compiler/translator/Pragma.h>
#include <angle/src/compiler/translator/tree_util/IntermTraverse.h>

#include <sstream>
#include <unordered_set>

#include "../core/string-utils.h"
#include "lib/spglsl-glsl-writer.h"
#include "spglsl-scoped-traverser.h"
#include "symbols/spglsl-symbol-info.h"

class SpglslAngleWebglOutput : public SpglslScopedTraverser, public SpglslGlslWriter {
 public:
  std::unordered_set<const sh::TStructure *> declaredStructs;

  SpglslAngleWebglOutput(std::ostream & out,
      SpglslSymbols & symbols,
      const SpglslGlslPrecisions & precisions,
      bool beautify);

  void visitSymbol(sh::TIntermSymbol * node) override;
  void visitConstantUnion(sh::TIntermConstantUnion * node) override;
  void visitPreprocessorDirective(sh::TIntermPreprocessorDirective * node) override;
  bool visitSwizzle(sh::Visit visit, sh::TIntermSwizzle * node) override;
  bool visitBinary(sh::Visit visit, sh::TIntermBinary * node) override;
  bool visitUnary(sh::Visit visit, sh::TIntermUnary * node) override;
  bool visitTernary(sh::Visit visit, sh::TIntermTernary * node) override;
  bool visitIfElse(sh::Visit visit, sh::TIntermIfElse * node) override;
  bool visitSwitch(sh::Visit visit, sh::TIntermSwitch * node) override;
  bool visitCase(sh::Visit visit, sh::TIntermCase * node) override;
  bool visitAggregate(sh::Visit visit, sh::TIntermAggregate * node) override;
  bool visitGlobalQualifierDeclaration(sh::Visit visit, sh::TIntermGlobalQualifierDeclaration * node) override;
  bool visitBranch(sh::Visit visit, sh::TIntermBranch * node) override;
  bool visitVariableDeclaration(sh::TIntermNode * node, sh::TIntermDeclaration * declarationNode) override;

  void writeHeader(int shaderVersion, const TPragma & pragma, const sh::TExtensionBehavior & extensionBehavior);
  void writeTOperatorNode(sh::TIntermOperator * node);

  virtual const std::string & getSymbolName(const sh::TSymbol * symbol);
  virtual std::string getTypeName(const sh::TType * type);
  virtual std::string getFieldName(const sh::TField * field);
  std::string getFunctionName(sh::TIntermAggregate * aggregateNode);

 protected:
  virtual std::string getBuiltinTypeName(const sh::TType * type);

  void beforeVisitFunctionDefinition(sh::TIntermFunctionDefinition * node) override;
  void beforeVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
      sh::TIntermFunctionDefinition * definition) override;
  void afterVisitFunctionPrototype(sh::TIntermFunctionPrototype * node,
      sh::TIntermFunctionDefinition * definition) override;
  void onVisitBlock(sh::TIntermBlock * node) override;
  void onVisitForLoop(sh::TIntermLoop * node, bool infinite) override;
  void onVisitWhileLoop(sh::TIntermLoop * node) override;
  void onVisitDoWhileLoop(sh::TIntermLoop * node) override;

 private:
  void traverseCodeBlock(sh::TIntermBlock * node);
  void traverseWithParentheses(sh::TIntermNode * node, int operandIndex);
  void traverseCodeBlock(sh::TIntermBlock * body, bool allowIf);

  const sh::TConstantUnion * writeConstantUnion(const sh::TType * type,
      const sh::TConstantUnion * pConstUnion,
      bool needsParentheses);

  void writeVariableType(const sh::TType & type, bool isFunctionArgument);
  void declareStruct(const sh::TStructure & structure);
  void declareInterfaceBlock(const sh::TInterfaceBlock & interfaceBlock);

  void writeVariableDeclaration(sh::TIntermNode & child);
  void writeConstantUnionSingleValue(const sh::TConstantUnion * value, bool needsParentheses, bool needsFloat);

  void clearLastWrittenVarDecl();
  bool needsToClearLastWrittenVarDecl();

  int _isInsideForInit = 0;
  bool _canForwardVarDecl = false;
  bool _skipNextBlockBraces = true;
  const sh::TType * _lastWrittenVarDecl = nullptr;
};

#endif
