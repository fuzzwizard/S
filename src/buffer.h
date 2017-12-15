#pragma once

#include "typedefs.h"
#include "util.h"

struct Buffer {
  const static size_t MIN_CAP = 8;
  u8* data;
  size_t capacity;
  size_t count;
};

void init(Buffer* buf, size_t _capacity) {
  size_t real_cap = Max(Buffer::MIN_CAP, _capacity);

  buf->data = new u8[real_cap];
  buf->count = 0;
  buf->capacity = real_cap;
}

void release(Buffer* buf) {
  delete[] buf->data;
  buf->count = 0;
  buf->capacity = 0;
}
