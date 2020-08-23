#ifndef _SPGLSL_HASH_STREAM_H_
#define _SPGLSL_HASH_STREAM_H_

#include <ostream>

#include "../external/highwayhash/highwayhash.h"

struct SpglslHashValue {
  union {
    struct {
      uint64_t a, b, c, d;
    };
    uint64_t data[4];
  };

  inline SpglslHashValue() : a(0), b(0), c(0), d(0) {
  }

  inline SpglslHashValue(const SpglslHashValue & copy) : a(copy.a), b(copy.b), c(copy.c), d(copy.d) {
  }

  inline bool operator==(const SpglslHashValue & other) const {
    return this->a == other.a && this->b == other.b && this->c == other.c && this->d == other.d;
  }

  inline bool operator!=(const SpglslHashValue & other) const {
    return this->a != other.a || this->b != other.b || this->c != other.c || this->d != other.d;
  }
};

class SpglslHasher {
 public:
  highwayhash::HighwayHashCat state;

  inline SpglslHasher() {
    this->resetHash();
  }

  inline SpglslHasher & resetHash() {
    const uint64_t hkey[4] = {0x125231, 0x876832, 0x8876263, 0x7486864};
    highwayhash::HighwayHashCatStart(hkey, &this->state);
    return *this;
  }

  inline SpglslHasher & begin(int header = 0) {
    int h[2] = {0xAC0FFEE, header};
    highwayhash::HighwayHashCatAppend((const uint8_t *)&h, sizeof(h), &this->state);
    return *this;
  }

  inline SpglslHasher & end() {
    int h[2] = {0x3C123ABC, 0xABBA};
    highwayhash::HighwayHashCatAppend((const uint8_t *)&h, sizeof(h), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const char * value) {
    if (value) {
      highwayhash::HighwayHashCatAppend((const uint8_t *)&value, strlen(value) + 1, &this->state);
    }
    return *this;
  }

  inline SpglslHasher & write(const std::string & value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)value.c_str(), value.size() + 1, &this->state);
    return *this;
  }

  inline SpglslHasher & write(const bool value) {
    char v = value ? 1 : 0;
    highwayhash::HighwayHashCatAppend((const uint8_t *)&v, sizeof(v), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const char value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const unsigned char value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const short value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const unsigned short value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const int value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const unsigned int value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const nullptr_t) {
    auto value = nullptr;
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(nullptr_t), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const size_t value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const int64_t value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const uint64_t value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const float value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  inline SpglslHasher & write(const double value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(value), &this->state);
    return *this;
  }

  template <typename T>
  inline SpglslHasher & writeStruct(const T & value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)&value, sizeof(T), &this->state);
    return *this;
  }

  template <typename T>
  inline SpglslHasher & writeStruct(const T * value) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)value, sizeof(T), &this->state);
    return *this;
  }

  template <typename T>
  inline SpglslHasher & writePtr(const T * ptr, size_t size) {
    highwayhash::HighwayHashCatAppend((const uint8_t *)ptr, size * sizeof(T), &this->state);
    return *this;
  }

  inline SpglslHashValue & digest(SpglslHashValue & result) const {
    highwayhash::HighwayHashCatFinish256(&this->state, result.data);
    return result;
  }

  inline SpglslHashValue digest() const {
    SpglslHashValue result;
    highwayhash::HighwayHashCatFinish256(&this->state, result.data);
    return result;
  }

  inline bool digestChanged(SpglslHashValue & hashValue) const {
    SpglslHashValue temp;
    highwayhash::HighwayHashCatFinish256(&this->state, temp.data);
    if (temp != hashValue) {
      hashValue = temp;
      return true;
    }
    return false;
  }
};

class SpglslHashStreamBuf : public std::streambuf, public SpglslHasher {
 public:
 protected:
  std::streamsize xsputn(const char_type * s, std::streamsize n) override;
  int_type overflow(int_type c) override;
};

class SpglslHashStream : public std::ostream {
 public:
  SpglslHashStreamBuf streambuf;

  SpglslHashStream() : std::ostream(&this->streambuf) {
  }

  inline SpglslHashStream & resetHash() {
    this->clear();
    this->streambuf.resetHash();
    return *this;
  }

  inline SpglslHashValue digest() const {
    return this->streambuf.digest();
  }

  inline SpglslHashValue & digest(SpglslHashValue & result) const {
    return this->streambuf.digest(result);
  }

  inline bool digestChanged(SpglslHashValue & hashValue) const {
    return this->streambuf.digestChanged(hashValue);
  }
};

#endif