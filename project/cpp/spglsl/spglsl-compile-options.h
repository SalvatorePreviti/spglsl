#ifndef _SPGLSL_RESOURCE_LIMITS_H_
#define _SPGLSL_RESOURCE_LIMITS_H_

#include <angle/src/compiler/translator/Compiler.h>
#include <emscripten/bind.h>

#include "core/non-copyable.h"

enum EShLanguage { EShLangFragment, EShLangVertex };

enum class SpglslCompileMode { Validate, Compile, Optimize };

enum class SpglslDefaultPrecision { undefined, lowp, mediump, highp };

class SpglslCompileOptions : NonCopyable {
 public:
  EShLanguage language;
  SpglslCompileMode compileMode;
  int parseShaderVersion;
  int outputShaderVersion;
  ShBuiltInResources angle;
  bool minify;
  bool mangle;
  bool recordConstantPrecision;

  SpglslDefaultPrecision floatPrecision;
  SpglslDefaultPrecision intPrecision;

  explicit SpglslCompileOptions();
  ~SpglslCompileOptions();

  void loadFromVal(emscripten::val input, emscripten::val resourceLimits);
};

SpglslCompileMode parseSpglslCompileMode(emscripten::val input);
SpglslCompileMode parseSpglslCompileMode(const std::string & input);

EShLanguage parseEShLanguage(emscripten::val input);
EShLanguage parseEShLanguage(const std::string & input);

#endif
