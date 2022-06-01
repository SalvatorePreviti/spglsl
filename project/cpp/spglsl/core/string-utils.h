#ifndef _SPGLSL_STRING_UTILS_H_
#define _SPGLSL_STRING_UTILS_H_

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

struct Strings {
  const static std::string empty;
};

inline bool stringReplaceOne(std::string & str, const std::string & from, const std::string & to) {
  size_t start_pos = str.find(from);
  if (start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

std::vector<std::string> stringSplit(const std::string & s, const std::string & delimiter);

// trim from start (in place)
void ltrim(std::string & s);

// trim from end (in place)
void rtrim(std::string & s);

// trim from both ends (in place)
void trim(std::string & s);

bool glslCharNeedSpace(char ch);

#endif
