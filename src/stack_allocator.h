#pragma once

#include "typedefs.h"

struct Stack_Allocator {
  size_t alignment = 8;

  u8* data_home = nullptr;
  u8* current_loc = nullptr;
  size_t bytes_left = 0;
  size_t capacity = 0;

  Stack_Allocator(size_t);
  Stack_Allocator(void*, size_t);

  void* alloc(size_t);
  void reset(void);

  typedef u8* frame;
  frame get_frame();
  void reset_to_frame(frame);
};

// NOTE: we dont enforce a min size for the allocator because we assume the user
// isnt going to make many of these, and can reason their way to making one work
Stack_Allocator::Stack_Allocator(size_t _size) {
  data_home = new u8[_size];
  current_loc = data_home;
  capacity = _size;
  bytes_left = _size;
};

Stack_Allocator::Stack_Allocator(void* _data, size_t _size) {
  data_home = (u8*)_data;
  current_loc = data_home;
  capacity = _size;
  bytes_left = _size;
};

void* Stack_Allocator::alloc(size_t amount) {
  size_t excess = amount - (amount % alignment); // TODO: check if this aligns
  amount += excess;

  void* result = current_loc;
  bytes_left -= amount;
  current_loc += amount;
  return result;
};

void Stack_Allocator::reset(void) {
  current_loc = data_home;
  bytes_left = capacity;
};
