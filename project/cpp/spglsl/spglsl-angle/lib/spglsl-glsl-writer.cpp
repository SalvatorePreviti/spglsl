#include "spglsl-glsl-writer.h"

#include "../../core/math-utils.h"
#include "../../core/string-utils.h"

#include <angle/src/compiler/translator/Symbol.h>
#include <angle/src/compiler/translator/Types.h>
#include <angle/src/compiler/translator/util.h>

SpglslAngleLayoutNeeds::SpglslAngleLayoutNeeds(const sh::TType & type) : layoutQualifier(type.getLayoutQualifier()) {
  sh::TQualifier qualifier = type.getQualifier();

  const sh::TLayoutQualifier & layoutQualifier = this->layoutQualifier;

  this->needsToWriteLocation = layoutQualifier.location >= 0 &&
      (qualifier == sh::EvqFragmentOut || qualifier == sh::EvqVertexIn || IsVarying(qualifier));

  this->needsToWriteIndex = qualifier == sh::EvqFragmentOut && layoutQualifier.index >= 0;

  this->needsToWriteYuv = qualifier == sh::EvqFragmentOut && layoutQualifier.yuv == true;

  this->needsToWriteBinding = layoutQualifier.binding >= 0 && IsOpaqueType(type.getBasicType());

  this->needsToWriteImage = layoutQualifier.imageInternalFormat != sh::EiifUnspecified && IsImage(type.getBasicType());

  this->needsToWriteOffset = layoutQualifier.offset >= 0 && IsAtomicCounter(type.getBasicType());

  this->needsToWriteLayoutQualifier =
      this->needsToWriteLocation || this->needsToWriteYuv || this->needsToWriteBinding || this->needsToWriteImage;

  this->needsToWriteLayout = true;
  if (!this->needsToWriteLocation && !this->needsToWriteBinding && !this->needsToWriteIndex && !this->needsToWriteYuv &&
      !this->needsToWriteImage && !this->needsToWriteOffset) {
    this->needsToWriteLayout = false;
  }
}

///////////// SpglslGlslWriter /////////////

SpglslGlslWriter::SpglslGlslWriter(std::ostream & out, bool beautify) :
    out(out),
    beautify(beautify),
    _indentLevel(0),
    _lastCh('\n'),
    _lastLastCh('\n'),
    _size(0),
    floatPrecision(sh::EbpUndefined),
    intPrecision(sh::EbpUndefined),
    defaultFloatPrecision(sh::EbpUndefined),
    defaultIntPrecision(sh::EbpUndefined) {
  this->_tmpChar[1] = '\0';
  out.imbue(std::locale::classic());
}

SpglslGlslWriter & SpglslGlslWriter::reset() {
  this->_indentLevel = 0;
  this->_lastCh = '\n';
  this->_lastLastCh = '\n';
  this->_size = 0;
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::write(const char character) {
  if (character != '\0') {
    this->_tmpChar[0] = character;
    this->write(this->_tmpChar, 1);
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::write(const char * text, size_t length) {
  if (!length) {
    return *this;
  }

  char valFirst = text[0];
  if (!valFirst) {
    return *this;
  }

  if (this->beautify) {
    if (valFirst == '{') {
      if (this->_lastCh == ')') {
        this->beautySpace();
      } else if (this->_lastCh == '}') {
        this->beautyNewLine();
      }
    }

    if (this->_lastCh == '\n') {
      for (int i = 0; i < this->_indentLevel; ++i) {
        this->writeRaw(' ');
        this->writeRaw(' ');
      }
    }
  }

  bool valNeedSpace = glslCharNeedSpace(valFirst);
  if (valNeedSpace && glslCharNeedSpace(this->_lastCh)) {
    this->writeRaw(' ');
  } else if (this->_lastCh == '-' && valFirst == '-') {
    this->writeRaw(' ');
  } else if (this->_lastCh == '+' && valFirst == '+') {
    this->writeRaw(' ');
  }

  if (this->beautify) {
    if (valFirst != ' ' && valFirst != '\n' && (this->_lastCh == ';' || this->_lastCh == ',' || this->_lastCh == ':')) {
      if (this->_lastCh != ';' || (valFirst != ';' && valFirst != ')')) {
        this->beautySpace();
      }
    } else if ((this->_lastCh == '}' || this->_lastCh == ']') && valNeedSpace) {
      this->beautySpace();
    } else if (valFirst == '{' && (this->_lastCh == ')' || glslCharNeedSpace(this->_lastCh))) {
      this->beautySpace();
    }
  }

  this->writeRaw(text, length);
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeComma() {
  char last = this->getLastOrLastLastCh();
  if (last != '(' && last != '[' && last != ',') {
    this->write(',');
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeStatementSemicolon() {
  char last = this->getLastOrLastLastCh();
  if (last != ';') {
    this->write(';').beautyNewLine();
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::beautySpace() {
  if (this->beautify && this->_lastCh != ' ' && this->_lastCh != '\n') {
    this->writeRaw(' ');
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::beautyNewLine() {
  if (this->beautify && this->_lastCh != '\n') {
    this->writeRaw('\n');
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::beautyDoubleNewLine() {
  if (this->beautify) {
    if (this->_lastCh != '\n') {
      this->writeRaw('\n');
      this->writeRaw('\n');
    } else if (this->_lastLastCh != '\n') {
      this->writeRaw('\n');
    }
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::beautyStatementNewLine(bool beforeStatement) {
  char _lastCh = this->_lastCh;
  if (_lastCh == '}' || _lastCh == ';' || (_lastCh == ' ' && (this->_lastLastCh == '}' || this->_lastLastCh == ';'))) {
    if ((_lastLastCh == '}' && _lastCh == ';') || (beforeStatement && _lastCh == '}')) {
      this->beautyDoubleNewLine();
    } else {
      this->beautyNewLine();
    }
  }
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeDirective(const char * directive, const char * command) {
  if (!directive) {
    return *this;
  }
  size_t directiveLen = strlen(directive);
  if (!directiveLen) {
    return *this;
  }
  if (this->_lastCh != '\n') {
    this->writeRaw('\n');
  }
  this->writeRaw(directive, directiveLen);
  if (command) {
    size_t commandLen = strlen(command);
    if (commandLen) {
      if (glslCharNeedSpace(command[0])) {
        this->writeRaw(' ');
      }
      this->writeRaw(command, commandLen);
    }
  }
  this->writeRaw('\n');
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeTOperator(TOperator op) {
  const char * ops;
  switch (op) {
    case EOpKill: ops = "discard"; break;
    case EOpBreak: ops = "break"; break;
    case EOpContinue: ops = "continue"; break;
    case EOpReturn: ops = "return"; break;
    default: ops = GetOperatorString(op); break;
  }
  return this->write(ops);
}

SpglslGlslWriter & SpglslGlslWriter::writeSwizzle(const sh::TVector<int> & offsets) {
  std::ostringstream tmp;
  for (const int offset : offsets) {
    switch (offset) {
      case 0: tmp << "x"; break;
      case 1: tmp << "y"; break;
      case 2: tmp << "z"; break;
      case 3: tmp << "w"; break;
      default: break;
    }
  }
  this->write(tmp.str());
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeDirective(sh::PreprocessorDirective directive, const char * command) {
  const char * directiveStr;
  switch (directive) {
    case sh::PreprocessorDirective::Define: directiveStr = "#define"; break;
    case sh::PreprocessorDirective::Endif: directiveStr = "#endif"; break;
    case sh::PreprocessorDirective::If: directiveStr = "#if"; break;
    case sh::PreprocessorDirective::Ifdef: directiveStr = "#ifdef"; break;
    default: return *this;
  }
  return this->writeDirective(directiveStr, command);
}

SpglslGlslWriter & SpglslGlslWriter::writeTMemoryQualifier(const sh::TMemoryQualifier & q) {
  this->write(q.readonly && !q.writeonly ? "readonly" : nullptr);
  this->write(q.writeonly && !q.readonly ? "writeonly" : nullptr);
  this->write(q.coherent ? "coherent" : nullptr);
  this->write(q.restrictQualifier ? "restrict" : nullptr);
  this->write(q.volatileQualifier ? "volatile" : nullptr);
  return *this;
}

SpglslGlslWriter & SpglslGlslWriter::writeTypePrecision(const sh::TType & type) {
  sh::TPrecision precision = type.getPrecision();
  if (precision == sh::EbpUndefined) {
    return *this;
  }
  switch (type.getBasicType()) {
    case sh::EbtStruct:
    case sh::EbtInterfaceBlock:
    case sh::EbtVoid:
    case sh::EbtAtomicCounter:
    case sh::EbtBool: return *this;

    case sh::EbtInt:
    case sh::EbtUInt:
      if (this->intPrecision != sh::EbpUndefined) {
        if (precision == this->intPrecision) {
          return *this;
        }
      } else if (precision == this->defaultIntPrecision) {
        return *this;
      }
      break;

    case sh::EbtFloat:
      if (this->intPrecision != sh::EbpUndefined) {
        if (precision == this->floatPrecision) {
          return *this;
        }
      } else if (precision == this->defaultFloatPrecision) {
        return *this;
      }
      break;

    default: break;
  }
  return this->write(sh::getPrecisionString(precision));
}

bool SpglslGlslWriter::writeTTypeLayoutQualifier(const sh::TType & type) {
  if (type.getBasicType() == sh::EbtInterfaceBlock && type.getInterfaceBlock()) {
    auto blockStorage = type.getInterfaceBlock()->blockStorage();
    auto blockBinding = type.getInterfaceBlock()->blockBinding();
    bool hasBlockStorage = blockStorage != sh::EbsUnspecified && blockStorage != sh::EbsStd140;
    if (hasBlockStorage || blockBinding >= 0) {
      this->write("layout(");
      if (hasBlockStorage) {
        this->write(sh::getBlockStorageString(blockStorage));
      }
      if (blockBinding >= 0) {
        if (hasBlockStorage) {
          this->writeComma();
        }
        this->write("binding").beautySpace().write('=').beautySpace().write(int32ToGlsl(blockBinding));
      }
      this->write(')').beautySpace();
      return true;
    }
    return false;
  }

  SpglslAngleLayoutNeeds needs(type);
  if (!needs.needsToWriteLayout) {
    return false;
  }

  this->write("layout");
  this->write('(');
  if (needs.needsToWriteLocation) {
    this->write("location").beautySpace().write('=').beautySpace().write(int32ToGlsl(needs.layoutQualifier.location));
  }
  if (needs.needsToWriteBinding) {
    this->writeComma();
    this->write("binding").beautySpace().write('=').beautySpace().write(int32ToGlsl(needs.layoutQualifier.binding));
  }
  if (needs.needsToWriteIndex) {
    this->writeComma();
    this->write("index").beautySpace().write('=').beautySpace().write(int32ToGlsl(needs.layoutQualifier.index));
  }
  if (needs.needsToWriteYuv) {
    this->writeComma();
    this->write("yuv");
  }
  if (needs.needsToWriteImage) {
    this->writeComma();
    this->write(sh::getImageInternalFormatString(needs.layoutQualifier.imageInternalFormat));
  }
  if (needs.needsToWriteOffset) {
    this->writeComma();
    this->write("offset").beautySpace().write('=').beautySpace().write(int32ToGlsl(needs.layoutQualifier.offset));
  }
  this->write(')').beautySpace();
  return true;
}