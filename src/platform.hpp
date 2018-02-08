#pragma once

#include "alltypes.hpp"

#if CS_OSX
#include <stdlib.h>
#elif CS_WINDOWS
#include <malloc.h>
#include <windows.h>
#endif

template <typename T> static inline void* platform_alloc(size_t amt) {
  void* block = malloc(amt * sizeof(T));
  assert(block);
  return block;
}

static inline void platform_free(void* block) {
  assert(block);
  free(block);
}

template <typename T>
static inline void* platform_realloc(void* old_block, size_t amt) {
  if (amt == 0) {
    warn("Freeing a block via realloc.");
    platform_free(old_block);
    return nullptr;
  }
  void* new_block = realloc(old_block, amt * sizeof(T));
  assert(new_block);
  return new_block;
}