#ifndef _SPGLSL_INIT_H_
#define _SPGLSL_INIT_H_

#include <emscripten/bind.h>

#include "core/non-copyable.h"

class SpglslImports : NonCopyable {
 public:
  static emscripten::val imports;

 private:
  SpglslImports() = default;

  friend bool spglsl_init(emscripten::val imports);
};

bool spglsl_init(emscripten::val imports);

#endif
