#include "math-utils.h"

#include <angle/src/common/mathutil.h>

#include <algorithm>
#include <cfloat>
#include <sstream>

#include "../spglsl-init.h"
#include "string-utils.h"

std::unordered_map<float, std::string> _floatToGlslCache;
std::unordered_map<std::string, std::string> _knownConversions;

static const std::string PositiveInfinity = "(1./0.)";
static const std::string ParentesizedPositiveInfinity = "(1./0.)";

static const std::string NegativeInfinity = "(-1./0.)";
static const std::string ParentesizedNegativeInfinity = "(-1./0.)";

static const std::string NaN = "0./0.";
static const std::string ParentesizedNaN = "(0./0.)";

const float FLOAT_MAX = 3.40282346638528859812e38f;

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

std::string _floatToGlslInner(float value, bool scientific = false) {
  std::string s;
  float diff = 0;
  for (int i = 8; i >= 0; --i) {
    std::stringstream ss;
    if (scientific) {
      ss.unsetf(std::ios::fixed);
      ss.setf(std::ios::scientific);
    } else {
      ss.setf(std::ios::fixed);
      ss.unsetf(std::ios::scientific);
    }
    ss.precision(i);
    ss << value;

    float v = std::stof(ss.str());
    float ndiff = std::abs(v - value);
    if (ndiff <= diff || s.empty() || v == value) {
      ndiff = diff;
      s = ss.str();
    }
  }

  bool hasDot = s.find('.') != std::string::npos;

  if (s.size() > 1 && s[0] == '+') {
    s.erase(0, 1);
  }

  auto expSplit = stringSplit(s, "e");
  bool isExponential = expSplit.size() > 1;
  if (isExponential) {
    s = expSplit[0] + "e" + std::to_string(std::stoi(expSplit[1]));
  }
  if (s.size() > 2 && s[s.size() - 2] == 'e' && s[s.size() - 1] == '0') {
    s = s.substr(0, s.size() - 2);
    isExponential = false;
  }

  if (!isExponential && !hasDot) {
    s += '.';
  }
  if (hasDot) {
    while (s.size() > 1 && s[s.size() - 1] == '0') {
      s.erase(s.size() - 1, 1);
    }
  }
  if (s.size() > 2 && s[0] == '0' && s[1] == '.') {
    s.erase(0, 1);
  }
  if (s.size() > 3 && s[0] == '-' && s[1] == '0' && s[2] == '.') {
    s.erase(0, 2);
    s = "-" + s;
  }
  if (s == "." || s == "-." || s == "-0.") {
    return "0.";
  }
  if (s.empty()) {
    return "0.";
  }

  return s;
}

void _initKnownConversions() {
  _knownConversions[_floatToGlslInner(3.141592653589793f)] = "acos(-1.)";
  _knownConversions[_floatToGlslInner(3.141592653589793f * 2)] = "(acos(-1.)*2.)";
  _knownConversions[_floatToGlslInner(1.5707963267948966f)] = "acos(0.)";

  for (int i = 0; i < 50; ++i) {
    float f = ((float)i) / 10.0f;
    _knownConversions[_floatToGlslInner(cosf(f))] = "cos(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(sin(f))] = "sin(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(expf(f))] = "exp(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(sqrtf(f))] = "sqrt(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(atanf(f))] = "atan(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(asinhf(f))] = "asinh(" + _floatToGlslInner(f) + ")";
    _knownConversions[_floatToGlslInner(coshf(f))] = "cosh(" + _floatToGlslInner(f) + ")";
  }
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

  if (value > FLOAT_MAX) {
    value = FLOAT_MAX;
  } else if (value < -FLOAT_MAX) {
    value = -FLOAT_MAX;
  }

  // return SpglslImports::floatToGlsl(value, needsParentheses, needsFloat);

  std::string result;

  auto cached = _floatToGlslCache.find(value);
  if (cached != _floatToGlslCache.end()) {
    result = cached->second;
  } else {
    auto s1 = _floatToGlslInner(value, false);
    auto s2 = _floatToGlslInner(value, true);
    result = s1.size() <= s2.size() ? s1 : s2;

    if (_floatToGlslCache.size() > 8000) {
      for (auto p : _floatToGlslCache) {
        _floatToGlslCache.erase(p.first);
        if (_floatToGlslCache.size() <= 4000) {
          break;
        }
      }
    }

    _floatToGlslCache[value] = result;
  }

  if (!needsFloat && result[result.size() - 1] == '.') {
    result.erase(result.size() - 1, 1);
  }

  if (_knownConversions.empty()) {
    _initKnownConversions();
  }

  auto knownConversion = _knownConversions.find(result);
  if (knownConversion != _knownConversions.end() && knownConversion->second.size() <= result.size()) {
    return knownConversion->second;
  }

  return result;
}
