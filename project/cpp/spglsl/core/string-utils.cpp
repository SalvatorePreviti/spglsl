#include "string-utils.h"

#include <sstream>
#include <vector>

const std::string Strings::empty;

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

std::vector<std::string> stringSplit(const std::string & s, const std::string & delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}
