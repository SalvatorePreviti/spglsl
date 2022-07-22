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

///////// spglslConvertLineDirectivesForAngle /////////

const char * kLine = "line";

std::string spglslConvertLineDirectivesForAngle(const std::string & sourceStr) {
  const char * source = sourceStr.c_str();
  size_t len = sourceStr.length();
  size_t index = 0;
  std::ostringstream out;
  out.imbue(std::locale::classic());
  while (index < len) {
    if (source[index] == '\n') {
      ++index;
      out.put('\n');
      continue;
    }
    bool hasSpace = false;
    if (std::isspace(source[index]) && source[index]) {
      hasSpace = true;
      do {
        ++index;
      } while (index < len && std::isspace(source[index]) && source[index] != '\n');
    }
    size_t start = index;
    if (index < len && source[index] == '#') {
      size_t didx = index + 1;
      while (didx < len && std::isspace(source[didx]) && source[didx] != '\n') {
        ++didx;
      }
      size_t di = 0;
      while (di != 4 && didx < len && kLine[di] == source[didx]) {
        ++di;
        ++didx;
      }
      if (di == 4) {
        while (didx < len) {
          char c = source[didx];
          if (c == '\\' && source[didx + 1] == '\n') {
            didx += 2;
            continue;
          }
          if (c == '\n' || !std::isspace(c)) {
            break;
          }
          ++didx;
        }
        out << "#line ";
        while (didx < len) {
          char c = source[didx];
          if (c == '\\' && source[didx + 1] == '\n') {
            didx += 2;
            continue;
          }
          if (c != '"') {
            out.put(c);
          }
          if (c == '\n') {
            break;
          }
          ++didx;
        }
        index = didx + 1;
        continue;
      }
    }
    if (hasSpace) {
      out.put(' ');
    }
    while (index < len && source[index] != '\n') {
      ++index;
    }
    out.write(source + start, index - start);
    out.put('\n');
    ++index;
  }
  return out.str();
}
