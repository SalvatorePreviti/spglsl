#ifndef _SPGLSL_COMPILER_H_
#define _SPGLSL_COMPILER_H_

#include <angle/include/GLSLANG/ShaderVars.h>
#include <angle/include/angle_gl.h>
#include <angle/src/compiler/translator/Compiler.h>
#include <angle/src/compiler/translator/Diagnostics.h>
#include <angle/src/compiler/translator/IntermNode.h>
#include <emscripten/bind.h>

#include "../core/hash-stream.h"
#include "../core/non-copyable.h"
#include "lib/spglsl-glsl-precisions.h"
#include "lib/spglsl-t-compiler.h"
#include "spglsl-angle-call-dag.h"
#include "spglsl-module-metadata.h"
#include "symbols/spglsl-symbol-info.h"

class SpglslAngleCompilerHandle;
class SpglslAngleCompilerBase : NonCopyable {};

class SpglslAngleCompiler : public SpglslTCompilerHolder {
 public:
  const SpglslCompileOptions & compilerOptions;
  SpglslModuleMetadata metadata;
  sh::TSymbolTable symbolTable;
  SpglslSymbols symbols;
  sh::TIntermBlock * body;
  SpglslAngleCallDag callDag;
  SpglslGlslPrecisions precisions;

  explicit SpglslAngleCompiler(sh::GLenum shaderType, const SpglslCompileOptions & compilerOptions);

  bool compile(const char * sourceCode);

  void loadPrecisions(bool reload);

  std::string decompileOutput();

 private:
  bool _checkAndSimplifyAST(sh::TIntermBlock * root, const sh::TParseContext & parseContext);
  void _mangle(sh::TIntermBlock * root);

  std::vector<SpglslAngleFunctionMetadata> _functionMetadata;
};

#endif
