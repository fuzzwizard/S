#pragma once

#include "alltypes.hpp"
#include "util.hpp"

void buffer_init(Buffer* buf, size_t _capacity) {
  size_t real_cap = Max(16 /* TODO: constant this */, _capacity);
  buf->data = new u8[real_cap];
  assert(buf->data);
  buf->data_size = real_cap;
  buf->occupied = 0;
}

void buffer_free(Buffer* buf) {
  delete[] buf->data;
  util::structzero(buf);
}