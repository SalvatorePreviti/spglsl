#ifndef _SPGLSL_COMPILED_INFO_H_
#define _SPGLSL_COMPILED_INFO_H_

#include <string>
#include <vector>

class SpglslCompiledInfo {
 public:
  std::vector<std::pair<std::string, std::string>> uniforms;

  inline void clear() {
    this->uniforms.clear();
  }
};

#endif
