#ifndef _SPGLSL_MATH_UTILS_H_
#define _SPGLSL_MATH_UTILS_H_

#include <string>

bool floatIsNaN(float value);
bool floatIsInfinity(float value);

inline bool floatIsFinite(float value) {
  return !floatIsNaN(value) && !floatIsInfinity(value);
}

std::string int32ToHex(int32_t value);
std::string uint32ToHex(uint32_t value);

inline std::string int32ToGlsl(int32_t value) {
  std::string dec = std::to_string(value);
  std::string hex = int32ToHex(value);
  return dec.length() <= hex.length() ? dec : hex;
}

inline std::string uint32ToGlsl(uint32_t value) {
  std::string dec = std::to_string(value);
  std::string hex = uint32ToHex(value);
  std::string result = dec.length() <= hex.length() ? dec : hex;
  return result += 'u';
}

std::string floatToGlsl(float value, bool needsParentheses);

#endif