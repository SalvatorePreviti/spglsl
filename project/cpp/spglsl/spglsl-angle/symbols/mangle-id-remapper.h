#ifndef _SPGLSL_MANGLE_ID_REMAPPER_
#define _SPGLSL_MANGLE_ID_REMAPPER_

#include <unordered_map>

class MangleIdRemapper {
 public:
  std::unordered_map<int, int> _remap;

  inline bool has(int mangleId) const {
    return this->_remap.count(mangleId) != 0;
  }

  inline void set(int oldMangleId, int newMangleId) {
    if (oldMangleId == newMangleId) {
      this->_remap.erase(oldMangleId);
    } else {
      _remap[oldMangleId] = newMangleId;
    }
  }

  inline int remapped(int mangleId) const {
    int result = mangleId;
    for (;;) {
      auto found = this->_remap.find(result);
      if (found == this->_remap.end()) {
        break;
      }
      result = found->second;
      if (result < 0 || result == mangleId) {
        break;
      }
    }
    return result;
  }
};

#endif
