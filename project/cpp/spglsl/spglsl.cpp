#include <emscripten/bind.h>

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
    const auto * uniformsMap = angleCompiler.getUniforms();
    const auto * globalsMap = angleCompiler.getGlobals();

    emscripten::val uniforms = emscripten::val::object();
    emscripten::val globals = emscripten::val::object();

    if (uniformsMap) {
      for (const auto & item : *uniformsMap) {
        uniforms.set(item.first, item.second);
      }
    }

    if (globalsMap) {
      for (const auto & item : *globalsMap) {
        globals.set(item.first, item.second);
      }
    }

    wresult.set("uniforms", uniforms);
    wresult.set("globals", globals);
  }
  return wresult;
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(spglsl) {
  function("spglsl_init", &spglsl_init);
  function("spglsl_angle_compile", &spglsl_angle_compile);
}

namespace angle {
  bool IsDebuggerAttached() {
    return false;
  }

  void BreakDebugger() {
  }
};