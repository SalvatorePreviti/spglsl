#include "spglsl-angle-compiler-handle.h"
#include "spglsl-angle-compiler.h"

#include "../core/string-utils.h"

sh::GLenum EShLanguageToGLenum(EShLanguage language) {
  switch (language) {
    case EShLangVertex: return GL_VERTEX_SHADER;
    default: return GL_FRAGMENT_SHADER;
  }
}

///////// SpglslAngleCompilerHandle /////////

SpglslAngleCompilerHandle::SpglslAngleCompilerHandle(SpglslAngleCompiler * compiler) : compiler(compiler) {
}

SpglslAngleCompilerHandle::SpglslAngleCompilerHandle(SpglslCompileOptions & compilerOptions) :
    SpglslAngleCompilerHandle(new SpglslAngleCompiler(EShLanguageToGLenum(compilerOptions.language), compilerOptions)) {
}

SpglslAngleCompilerHandle::~SpglslAngleCompilerHandle() {
  if (this->compiler) {
    delete this->compiler;
  }
}

bool SpglslAngleCompilerHandle::isInitialized() const {
  return this->compiler != nullptr;
}

bool SpglslAngleCompilerHandle::compile(const std::string & sourceCode) {
  return this->isInitialized() && this->compiler->compile(sourceCode.c_str());
}

std::string SpglslAngleCompilerHandle::getInfoLog() const {
  return this->compiler ? this->compiler->infoSink.info.str() : "ERROR 0:0 '' Failed to initialize ANGLE compiler";
}

std::string SpglslAngleCompilerHandle::decompileOutput() const {
  return this->compiler ? this->compiler->decompileOutput() : Strings::empty;
}

const std::map<std::string, std::string> * SpglslAngleCompilerHandle::getUniforms() const {
  return this->compiler ? &this->compiler->uniformsMap : nullptr;
}

const std::map<std::string, std::string> * SpglslAngleCompilerHandle::getGlobals() const {
  return this->compiler ? &this->compiler->globalsMap : nullptr;
}
