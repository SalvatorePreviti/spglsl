#ifndef _SPGLSL_WRITER_H_
#define _SPGLSL_WRITER_H_

#include <angle/src/compiler/translator/BaseTypes.h>
#include <angle/src/compiler/translator/ImmutableString.h>
#include <angle/src/compiler/translator/Operator_autogen.h>
#include <angle/src/compiler/translator/Types.h>

#include <sstream>
#include <string>

#include "../../core/non-copyable.h"

namespace sh {
  enum class PreprocessorDirective;
}

class SpglslAngleLayoutNeeds {
 public:
  sh::TLayoutQualifier layoutQualifier;

  SpglslAngleLayoutNeeds(const sh::TType & type);

  bool needsToWriteLocation;
  bool needsToWriteIndex;
  bool needsToWriteYuv;
  bool needsToWriteBinding;
  bool needsToWriteImage;
  bool needsToWriteOffset;
  bool needsToWriteLayoutQualifier;
  bool needsToWriteLayout;
};

class SpglslGlslWriter : NonCopyable {
 public:
  std::ostream & out;
  bool beautify;

  sh::TPrecision floatPrecision;
  sh::TPrecision intPrecision;

  sh::TPrecision defaultFloatPrecision;
  sh::TPrecision defaultIntPrecision;

  explicit SpglslGlslWriter(std::ostream & out, bool beautify);

  SpglslGlslWriter & reset();

  inline size_t size() const {
    return this->_size;
  }

  inline int getIndentLevel() const {
    return this->_indentLevel;
  }

  inline void setIndentLevel(int value) {
    this->_indentLevel = value;
  }

  inline char getLastCh() const {
    return this->_lastCh;
  }

  inline char getLastLastCh() const {
    return this->_lastLastCh;
  }

  inline char getLastOrLastLastCh() const {
    char last = this->_lastCh;
    return (last == ' ' || last == '\n') ? this->_lastLastCh : last;
  }

  inline SpglslGlslWriter & indent() {
    ++this->_indentLevel;
    return *this;
  }

  inline SpglslGlslWriter & deindent() {
    --this->_indentLevel;
    return *this;
  }

  SpglslGlslWriter & write(const char * text, size_t length);

  SpglslGlslWriter & write(const char character);

  inline SpglslGlslWriter & write(const char * text) {
    return text ? this->write(text, strlen(text)) : *this;
  }

  inline SpglslGlslWriter & write(const std::string & text) {
    return this->write(text.c_str(), text.length());
  }

  inline SpglslGlslWriter & write(const sh::ImmutableString & s) {
    return this->write(s.data(), s.length());
  }

  SpglslGlslWriter & writeComma();
  SpglslGlslWriter & writeStatementSemicolon();
  SpglslGlslWriter & beautySpace();
  SpglslGlslWriter & beautyNewLine();
  SpglslGlslWriter & beautyDoubleNewLine();
  SpglslGlslWriter & beautyStatementNewLine(bool beforeStatement = false);

  SpglslGlslWriter & writeDirective(const char * directive, const char * command);

  // Angle specific

  SpglslGlslWriter & writeSwizzle(const sh::TVector<int> & offsets);
  SpglslGlslWriter & writeDirective(sh::PreprocessorDirective directive, const char * command);
  SpglslGlslWriter & writeTMemoryQualifier(const sh::TMemoryQualifier & q);
  SpglslGlslWriter & writeTypePrecision(const sh::TType & type);

  bool needsToWriteTTypeLayoutQualifier(const sh::TType & type);
  bool writeTTypeLayoutQualifier(const sh::TType & type);

 protected:
  inline SpglslGlslWriter & writeRaw(const char ch) {
    if (ch != '\0') {
      this->out.put(ch);
      this->_lastLastCh = this->_lastCh;
      this->_lastCh = ch;
      ++this->_size;
    }
    return *this;
  }

  inline SpglslGlslWriter & writeRaw(const char * s, size_t length) {
    if (length != 0) {
      this->out.write(s, length);
      this->_lastLastCh = this->_lastCh;
      this->_lastCh = s[length - 1];
      this->_size += length;
    }
    return *this;
  }

  inline SpglslGlslWriter & writeRaw(const char * raw) {
    return raw ? this->writeRaw(raw, strlen(raw)) : *this;
  }

 private:
  int _indentLevel;
  size_t _size;
  char _lastCh;
  char _lastLastCh;
  char _tmpChar[2];
};

#endif