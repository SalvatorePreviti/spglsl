#include "spglsl-init.h"

#include <clocale>
#include <cstdio>
#include <cwchar>
#include <string>

#include <angle/include/GLSLANG/ShaderLang.h>

emscripten::val SpglslImports::imports = emscripten::val::null();

bool spglsl_init(emscripten::val imports) {
  std::setlocale(LC_ALL, "en_US.utf8");

  if (!sh::Initialize()) {
    return false;
  }

  SpglslImports::imports = imports;

  return true;
}
