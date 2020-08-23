#include "hash-stream.h"

std::streamsize SpglslHashStreamBuf::xsputn(const SpglslHashStreamBuf::char_type * s, std::streamsize n) {
  if (s && n) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)s, n * sizeof(SpglslHashStreamBuf::char_type), &this->state);
    return n;
  }
  return 0;
}

SpglslHashStreamBuf::int_type SpglslHashStreamBuf::overflow(SpglslHashStreamBuf::int_type c) {
  if (c != EOF) {
    const uint8_t s[1] = {(uint8_t)c};
    highwayhash::HighwayHashCatAppend(s, 1, &this->state);
  }
  return c;
}
