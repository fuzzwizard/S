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

// TODO: Actual SB impl
#if 0
static void sb__maybe_realloc(String_Builder* sb, int amt) {
  int amount_needed = sb->current_buffer.occupied + amt;
  if (amount_needed > sb->current_buffer.data_size) {
    unimplemented();
  }
};

static inline void sb_push_char(String_Builder* sb, char c) {
  sb__maybe_realloc(sb, 1);
  sb->current_buffer.data[sb->current_buffer.occupied] = c;
  sb->current_buffer.occupied++;
}

static inline char sb_peek_head(String_Builder* sb) {
  return sb->current_buffer.data[sb->consumed];
}

static inline void sb_consume(String_Builder* sb) {
  sb->consumed++;
  assert(sb->consumed <= sb->occupied);
}

static inline char sb_consume_w_return(String_Builder* sb) {
  sb_consume(sb);
  return sb_peek_head(sb);
}

static inline bool sb_eat_whitespace(String_Builder* sb) {
  if (IsWhiteSpace(sb_peek_head(sb))) {
    sb_consume(sb);
    return true;
  }
  return false;
}

static inline bool sb_eat_whitespaces(String_Builder* sb) {
  bool result;
  while (result = sb_eat_whitespace(sb)) {
    /* This just keeps eating whitespace, until whitespace is not found */
  };
  return result;
}

static inline char sb_eat_char(String_Builder* sb) {
  char c = 0;
  if (IsAlpha(sb_peek_head(sb))) {
    c = sb_consume_w_return(sb);
  }
  return c;
}
#endif