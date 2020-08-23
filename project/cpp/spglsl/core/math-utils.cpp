#include "math-utils.h"

#include <angle/src/common/mathutil.h>

#include <algorithm>
#include <cfloat>
#include <sstream>

#include "../spglsl-init.h"
#include "string-utils.h"

static const std::string PositiveInfinity = "(1./0.)";
static const std::string ParentesizedPositiveInfinity = "(1./0.)";

static const std::string NegativeInfinity = "(-1./0.)";
static const std::string ParentesizedNegativeInfinity = "(-1./0.)";

static const std::string NaN = "0./0.";
static const std::string ParentesizedNaN = "(0./0.)";

std::string uint32ToHex(uint32_t value) {
  std::ostringstream ss;
  ss << "0x" << std::hex << value;
  return ss.str();
}

std::string int32ToHex(int32_t value) {
  std::ostringstream ss;
  if (value < 0) {
    if (value == -2147483648) {
      return "-0x80000000";
    }
    value = -value;
    ss << '-';
  }
  ss << "0x" << std::hex << value;
  return ss.str();
}

bool floatIsNaN(float value) {
  return std::isnan(value) || gl::isNaN(value);
}

bool floatIsInfinity(float value) {
  return std::isinf(value) || gl::isInf(value);
}

std::string floatToGlsl(float value, bool needsParentheses, bool needsFloat) {
  if (floatIsNaN(value)) {
    return needsParentheses ? ParentesizedNaN : NaN;
  }
  if (floatIsInfinity(value)) {
    if (needsParentheses) {
      return value > 0 ? ParentesizedPositiveInfinity : ParentesizedNegativeInfinity;
    }
    return value > 0 ? PositiveInfinity : NegativeInfinity;
  }
  value = std::min(FLT_MAX, std::max(-FLT_MAX, value));

  float absValue = std::abs(value);

  if (floatIsNaN(absValue) || absValue < FLT_MIN) {
    return needsFloat ? "0." : "0";
  }

  return SpglslImports::floatToGlsl(value, needsParentheses, needsFloat);
}