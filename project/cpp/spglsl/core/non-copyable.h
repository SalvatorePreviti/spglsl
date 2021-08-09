#ifndef _SPGLSL_NON_COPYABLE_H_
#define _SPGLSL_NON_COPYABLE_H_

class NonCopyable {
 protected:
  constexpr NonCopyable() throw() = default;
  ~NonCopyable() throw() = default;

 private:
  NonCopyable(const NonCopyable &) throw() = delete;
  void operator=(const NonCopyable &) throw() = delete;
};

#endif