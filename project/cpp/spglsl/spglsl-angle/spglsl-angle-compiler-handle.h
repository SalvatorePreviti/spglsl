#ifndef _SPGLSL_COMPILER_HANDLE_H_
#define _SPGLSL_COMPILER_HANDLE_H_

#include "../core/non-copyable.h"
#include "../spglsl-compile-options.h"

class SpglslAngleCompiler;

class SpglslAngleCompilerHandle : public NonCopyable {
 public:
  SpglslAngleCompiler * const compiler;

  explicit SpglslAngleCompilerHandle(SpglslAngleCompiler * compiler);

  explicit SpglslAngleCompilerHandle(const SpglslCompileOptions & compilerOptions);

  bool isInitialized() const;
  bool compile(const std::string & sourceCode);

  std::string getInfoLog() const;
  std::string decompileOutput() const;

  ~SpglslAngleCompilerHandle();
};

std::string spglslConvertLineDirectivesForAngle(const std::string & source);

#endif
