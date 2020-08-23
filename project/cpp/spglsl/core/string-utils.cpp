#include "string-utils.h"

#include <sstream>

const std::string Strings::empty = "";

void ltrim(std::string & s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
void rtrim(std::string & s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string & s) {
  ltrim(s);
  rtrim(s);
}

bool glslCharNeedSpace(char ch) {
  if (ch == 0) {
    return false;
  }
  return ((ch >= 97 && ch <= 122) ||  // a-z
      (ch >= 65 && ch <= 90) ||  // A-Z
      (ch >= 48 && ch <= 57) ||  // 0-9
      ch == 95 ||  // _
      ch == 36  // $
  );
}
