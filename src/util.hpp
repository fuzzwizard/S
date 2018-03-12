#pragma once

#include "alltypes.hpp"
#include "platform.hpp"

namespace util {
template <typename T> inline void memzero(T* ptr, size_t n) {
  assert(ptr);
  for (int i = 0; i < n; i++) {
    ptr[i] = 0;
  }
};

template <typename T> inline void zero_struct(T* s) {
  u8* copy = (u8*)s;
  for (int i = 0; i < sizeof(T); i++) {
    copy[i] = 0;
  }
};

template <typename T> void memmove(T* dest, T* source, size_t n) {
  assert(n * sizeof(T) < 8194);
  T* buffer = (T*)alloca(n * sizeof(T));

  for (int i = 0; i < n; i++) {
    buffer[i] = source[i];
  }

  for (int i = 0; i < n; i++) {
    dest[i] = buffer[i];
  }
};

template <typename T>
inline void memcpy(T* dest, T* source, size_t n) {
  for (int i = 0; i < n; i++) {
    dest[i] = source[i];
  }
};
}; // namespace util

template <typename Fn> struct __Defer_Wrapper {
  __Defer_Wrapper(Fn f) : f(f){};
  ~__Defer_Wrapper() { f(); };
  Fn f;
};

template <typename Fn> __Defer_Wrapper<Fn> __defer_impl(Fn f) {
  return __Defer_Wrapper<Fn>(f);
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x) DEFER_2(x, __COUNTER__)
#define defer(Code) auto DEFER_3(_defer_) = __defer_impl([&]() { Code; })