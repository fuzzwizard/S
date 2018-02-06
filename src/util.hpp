#pragma once

#include "alltypes.h"
#include "platform.h"

namespace util {
inline void memzero(void* ptr, size_t n) {
  u8* copy = (u8*)ptr;
  for (int i = 0; i < n; i++) {
    copy[i] = 0;
  }
};

template <typename T> inline void structzero(T* s) {
  u8* copy = (u8*)s;
  for (int i = 0; i < sizeof(T); i++) {
    copy[i] = 0;
  }
};

inline void memset(void* ptr, u8 byte, size_t n) {
  u8* copy = (u8*)ptr;
  for (int i = 0; i < n; i++) {
    copy[i] = byte;
  }
};

inline void memcpy(void* dest, void* source, size_t n) {
  u8* copy_source = (u8*)source;
  u8* copy_dest = (u8*)dest;
  for (int i = 0; i < n; i++) {
    copy_dest[i] = copy_source[i];
  }
};

inline void memcpy(void* dest, const char* source, size_t n) {
  u8* copy_source = (u8*)source;
  u8* copy_dest = (u8*)dest;
  for (int i = 0; i < n; i++) {
    copy_dest[i] = copy_source[i];
  }
}

inline void memmove(void* dest, void* source, size_t n) {
  u8* copy_source = (u8*)source;
  u8* copy_dest = (u8*)dest;

  u8* buffer = (u8*)alloca(n);

  size_t counter = n;
  u8* buffer_ptr = buffer;
  for (int i = 0; i < counter; i++) {
    buffer[i] = copy_source[i];
  }

  counter = n;
  buffer_ptr = buffer;
  for (int i = 0; i < counter; i++) {
    copy_dest[i] = buffer[i];
  }
};

inline size_t strlen(char* target) {
  size_t result = 0;
  while (target[result] != '\0') {
    result++;
  }
  return result;
}

int strncmp(const char* a, const char* b, size_t n) {
  while (n && *a && (*a == *b)) {
    ++a;
    ++b;
    --n;
  }
  if (n == 0) {
    return 0;
  } else {
    return (*((u8*)a) - *((u8*)b));
  }
}
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