#ifndef _SPGLSL_COMPILER_HANDLE_H_
#define _SPGLSL_COMPILER_HANDLE_H_

#include <map>
#include "../core/non-copyable.h"
#include "../spglsl-compile-options.h"

class SpglslAngleCompiler;

class SpglslAngleCompilerHandle : public NonCopyable {
 public:
  SpglslAngleCompiler * const compiler;

  explicit SpglslAngleCompilerHandle(SpglslAngleCompiler * compiler);

  explicit SpglslAngleCompilerHandle(SpglslCompileOptions & compilerOptions);

  bool isInitialized() const;
  bool compile(const std::string & sourceCode);

  std::string getInfoLog() const;
  std::string decompileOutput() const;
  const std::map<std::string, std::string> * getUniforms() const;
  const std::map<std::string, std::string> * getGlobals() const;

  ~SpglslAngleCompilerHandle();
};

std::string spglslConvertLineDirectivesForAngle(const std::string & source);

#endif
