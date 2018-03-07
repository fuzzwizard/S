#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define global_variable static
#define local_persist static

typedef unsigned int uint;
#define UIntType(Num) typedef uint##Num##_t u##Num
#define SIntType(Num) typedef int##Num##_t i##Num

// clang-format off
UIntType(8); UIntType(16); UIntType(32); UIntType(64);
SIntType(8); SIntType(16); SIntType(32); SIntType(64);
// clang-format on

typedef float r32;
typedef double r64;

typedef u32 bool32;
typedef u16 bool16;
typedef u8 bool8;
#define true32 ((bool32)1)
#define false32 ((bool32)0)
#define true16 ((bool16)1)
#define false16 ((bool16)0)
#define true8 ((bool8)1)
#define false8 ((bool8)0)

#define Kilobyte(N) ((i64)((N)*1024))
#define Megabyte(N) ((i64)(Kilobyte(N) * 1024))
#define Gigabyte(N) ((i64)(Megabyte(N) * 1024))

#define Clamp(Value, Bottom, Top)                                              \
  ((Value) > (Top) ? (Top) : (Value) < (Bottom) ? (Bottom) : (Value))

#define Lerp(InputA, InputB, AlphaValue)                                       \
  (((1 - (AlphaValue)) * (InputA)) + ((AlphaValue)*InputB))

#define Max(A, B) (((A) >= (B)) ? (A) : (B))
#define Min(A, B) (((A) <= (B)) ? (A) : (B))
#define Abs(X) (((X) < 0) ? -(X) : (X))

#define IsPowerOfTwo(X) (((X) & ((X)-1)) == 0)
#define IsEven(X) (((X) % 2) == 0)
#define IsOdd(X) (((X) % 2) != 0)

#define Swap(X, Y)                                                             \
  {                                                                            \
    X = X + Y;                                                                 \
    Y = X - Y;                                                                 \
    X = X - Y;                                                                 \
  }

#define Newline() printf("\n")

#define _Cat(A, B) A##B
#define Cat(A, B) _Cat(A, B)

#define AlignPow2(Value, Alignment)                                            \
  ((Value + ((Alignment)-1)) & ~((Value - Value) + (Alignment)-1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)

#define IsWhiteSpace(Char) ((Char) == ' ' || (Char) == '\n' || (Char) == '\t')
#define IsNumber(Char) ((Char) >= '0' && (Char) <= '9')
#define IsAlpha(Char)                                                          \
  (((Char) >= 'a' && (Char) <= 'z') && ((Char) >= 'A' && (Char) <= 'Z'))
#define IsBoundary(Char)                                                       \
  ((Char) == '{' || (Char) == '}' || (Char) == '(' || (Char) == ')' ||         \
   (Char) == '[' || (Char) == ']')
#define IsAlphaNum(Char) (IsAlpha(Char) && IsNumber(Char))

void __stop_everything(const char* file, const char* function, int line,
                       const char* message1, const char* message2 = 0) {
  printf("#############################################\n");
  printf("# %s\n", message1);
  if (message2) {
    printf("# %s\n", message2);
  }
  printf("# (%s:%s:%d)\n", file, function, line);
  printf("#############################################\n");
  exit(1);
}

void __print_warning(const char* file, const char* function, int line,
                     const char* message) {
  printf("⚠ WARNING: %s ", message);
  printf("(file:%s func:%s line:%d)\n", file, function, line);
}

#ifdef CS_SLOW
#define assert(Cond)                                                           \
  ((Cond) ? ((void)0)                                                          \
          : (__stop_everything(__FILE__, __func__, __LINE__,                   \
                               "Assertation failed: " #Cond)))
#define assert_m(Cond, Msg)                                                    \
  ((Cond) ? ((void)0)                                                          \
          : (__stop_everything(__FILE__, __func__, __LINE__,                   \
                               "Assertation failed: " #Cond, Msg)))
#define unreachable(...)                                                       \
  __stop_everything(__FILE__, __func__, __LINE__, "Reached unreachable code.")
#define unimplemented(...)                                                     \
  __stop_everything(__FILE__, __func__, __LINE__, "Reached unimlemented code.")
#define warn(Msg) __print_warning(__FILE__, __func__, __LINE__, Msg)
#else
#define assert(...)
#define unreachable(...)
#define unimplemented(...) NOT IMPLEMENTED !!!!!!
#define warn(...)
#endif

// NOTE: Testing required
template <typename T> struct Queue {
  T* inbox; // data home
  size_t inbox_size;
  T* outbox;
  size_t outbox_size;
  size_t space_left;

  Queue(size_t cap) {
    space_left = cap;
    outbox_size = inbox_size = 0;
    inbox = outbox = nullptr;
  }

  void initialize() {
    inbox = (T*)realloc(inbox, space_left);
    outbox = inbox + (space_left - 1);
  }

  void queue(T item) {
    // push into the inbox
    assert(space_left > 0);
    push_inbox(item);
  }

  T dequeue() {
    if (outbox_size == 0 && flush_inbox() == 0) {
      return {0};
    }
    T result = pop_outbox();
    space_left++;
    return result;
  }

  void push_inbox(T item) {
    inbox[inbox_size++] = item;
    space_left--;
  }

  T pop_inbox() {
    T result = inbox[inbox_size--];
    space_left++;
    return result;
  }

  T pop_outbox() {
    T result = outbox[-outbox_size];
    outbox_size--;
    space_left++;
    return result;
  }

  size_t flush_inbox() {
    // TODO: TEST THIS
    while (inbox_size > 0) {
      outbox[-inbox_size] = inbox[inbox_size];
      outbox_size++;
      inbox_size--;
    }
    return outbox_size;
  }

  void push_outbox(T item) {
    outbox[-outbox_size] = item;
    space_left--;
    outbox_size++;
  }
};

template <typename T> struct Array {
  size_t count;
  size_t cap;
  T* data;

  T& operator[](int i) {
    assert(i > 0);
    assert(i < count);
    return data[i];
  }
  Array(size_t _cap = 0) { initialize(_cap); }

  void initialize(size_t _cap, T* _data = nullptr, size_t _count = 0) {
    __init(_cap, _data, _count);
    __allocate();
  }

  void __init(size_t _cap, T* _data = nullptr, size_t _count = 0) {
    count = _count;
    cap = _cap < 8 ? 8 : _cap;
    data = _data;
  }

  void __allocate() { data = (T*)realloc(data, cap); }

  void __maybe_realloc(int amt_needed) {
    // overflow risk adding an int to a uint
    size_t cap_needed = count + amt_needed;
    if (cap_needed >= cap) {
      while (cap_needed >= cap)
        cap *= 2;
      __allocate();
    }
  }

  void free() {
    cap = 0;
    count = 0;
    __allocate();
  }

  T last() { return data[count - 1]; }

  void push(T value) {
    __maybe_realloc(1);
    data[count++] = value;
  }

  T pop() { return data[--count]; }

  // add n unintialized items and return a ptr to the first
  T* add(int n) {
    __maybe_realloc(n);
    T* res = &data[count];
    count += n;
    return res;
  }

  void fast_remove(size_t idx) {
    assert(idx < count);
    data[idx] = pop();
  }

  void remove(size_t idx) {
    assert(idx < count);
    while (idx < count)
      data[idx] = data[++idx];
    count--;
  }

  void insert(T value, size_t idx) {
    assert(idx < count);
    __maybe_realloc(1);
    size_t swap_index = ++count;
    while (swap_index >= idx) {
      data[swap_index - 1] = data[swap_index--];
    }
    data[idx] = value;
  }

  size_t count_in_bytes() { return count * sizeof(T); }

  size_t cap_in_bytes() { return cap * sizeof(T); }
};

using Buffer = Array<u8>;

enum Token_Type {
  TokenType_stream_terminator,
  TokenType_invalid,

  TokenType_atom,
  TokenType_numeric_literal,
  TokenType_char_literal,
  TokenType_string_literal,

  TokenType_open_paren,
  TokenType_close_paren,
  TokenType_open_bracket,
  TokenType_close_bracket,
  TokenType_open_brace,
  TokenType_close_brace,

  TokenType_comma,
  TokenType_semicolon,
  TokenType_whitespace,
  TokenType_comment,

  TokenType_COUNT
};

struct Token {
  size_t start_index = 0;
  size_t boundary_index = 0;
  Token_Type type = TokenType_invalid;
};
