#ifndef _SPGLSL_INIT_H_
#define _SPGLSL_INIT_H_

#include <emscripten/bind.h>

#include "core/non-copyable.h"

class SpglslImports : NonCopyable {
 public:
  static emscripten::val imports;

  static inline std::string floatToGlsl(float value, bool needsParentheses, bool needsFloat) {
    return SpglslImports::_floatToGlsl(value, needsParentheses, needsFloat).as<std::string>();
  }

 private:
  SpglslImports() = default;

  friend bool spglsl_init(emscripten::val imports);
  static emscripten::val _floatToGlsl;
};

bool spglsl_init(emscripten::val imports);

#endif