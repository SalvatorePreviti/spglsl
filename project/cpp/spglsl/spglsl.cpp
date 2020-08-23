#include <emscripten/bind.h>

#include <iostream>
#include <sstream>

#include "spglsl-angle/spglsl-angle-compiler-handle.h"
#include "spglsl-compile-options.h"
#include "spglsl-init.h"

emscripten::val spglsl_angle_compile(emscripten::val cinput,
    emscripten::val resourceLimitsVal,
    const std::string & mainSourceCode) {
  EShLanguage language = parseEShLanguage(cinput["language"]);
  int outputShaderVersion = cinput["outputVersion"].as<int>();

  emscripten::val wresult = emscripten::val::object();
  wresult.set("output", emscripten::val::null());

  SpglslCompileOptions coptions;
  coptions.loadFromVal(cinput, resourceLimitsVal);

  SpglslAngleCompilerHandle angleCompiler(coptions);
  if (angleCompiler.isInitialized()) {
    bool angleValid = angleCompiler.compile(mainSourceCode);
    wresult.set("infoLog", emscripten::val(angleCompiler.getInfoLog()));
    wresult.set("valid", emscripten::val(angleValid));

    if (angleValid) {
      wresult.set("output", angleCompiler.decompileOutput());
    }
  }
  return wresult;
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(spglsl) {
  function("spglsl_init", &spglsl_init);
  function("spglsl_angle_compile", &spglsl_angle_compile);
}