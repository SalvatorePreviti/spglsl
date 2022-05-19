#ifndef _SPGLSL_NON_COPYABLE_H_
#define _SPGLSL_NON_COPYABLE_H_

class NonCopyable {
 protected:
  constexpr NonCopyable() noexcept = default;
  ~NonCopyable() noexcept = default;

 public:
  NonCopyable(const NonCopyable &) noexcept = delete;
  void operator=(const NonCopyable &) noexcept = delete;
};

#endif
