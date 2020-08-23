#ifndef _SPGLSL_NON_COPYABLE_H_
#define _SPGLSL_NON_COPYABLE_H_

class NonCopyable {
 protected:
  constexpr NonCopyable() = default;
  ~NonCopyable() = default;

 private:
  NonCopyable(const NonCopyable &) = delete;
  void operator=(const NonCopyable &) = delete;
};

#endif