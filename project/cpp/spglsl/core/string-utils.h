#ifndef _SPGLSL_STRING_UTILS_H_
#define _SPGLSL_STRING_UTILS_H_

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

struct Strings {
  const static std::string empty;
  const static std::string trueString;
  const static std::string falseString;
};

// trim from start (in place)
void ltrim(std::string & s);

// trim from end (in place)
void rtrim(std::string & s);

// trim from both ends (in place)
void trim(std::string & s);

bool glslCharNeedSpace(char ch);

#endif