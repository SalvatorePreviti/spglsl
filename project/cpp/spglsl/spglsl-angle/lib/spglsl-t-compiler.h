#ifndef _SPGLSL_DUMMY_T_COMPILER_H_
#define _SPGLSL_DUMMY_T_COMPILER_H_

#include <angle/include/angle_gl.h>
#include <angle/src/compiler/translator/Compiler.h>

#include "../../core/non-copyable.h"
#include "../../spglsl-compile-options.h"

class SpglslTCompiler : public sh::TCompiler {
 public:
  explicit SpglslTCompiler(sh::GLenum shaderType, ShShaderSpec shaderSpec);

  inline sh::TExtensionBehavior & getExtensionBehavior() {
    return const_cast<sh::TExtensionBehavior &>(sh::TCompiler::getExtensionBehavior());
  }

  inline ShBuiltInResources & getResources() {
    return const_cast<ShBuiltInResources &>(sh::TCompiler::getResources());
  }

  inline angle::PoolAllocator & getAllocator() {
    return this->allocator;
  }

 protected:
  bool shouldFlattenPragmaStdglInvariantAll() override;
  virtual bool translate(sh::TIntermBlock * root,
      const ShCompileOptions & compileOptions,
      sh::PerformanceDiagnostics * perfDiagnostics) override;
};

class SpglslTCompilerHolder : NonCopyable {
 public:
  SpglslTCompiler tCompiler;
  sh::TExtensionBehavior & extensionBehavior;
  sh::TInfoSink & infoSink;
  sh::TDiagnostics diagnostics;

  inline angle::PoolAllocator & getAllocator() {
    return this->tCompiler.getAllocator();
  }

  SpglslTCompilerHolder(sh::GLenum shaderType, int shaderVersion);
};

class TScopedSymbolTableLevel : angle::NonCopyable {
 public:
  sh::TSymbolTable * symbolTable;

  explicit TScopedSymbolTableLevel(sh::TSymbolTable * symbolTable);

  ~TScopedSymbolTableLevel();
};

#endif
