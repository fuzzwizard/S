#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility>

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

#define IsWhitespace(Char) ((Char) == ' ' || (Char) == '\n' || (Char) == '\t')
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
  __stop_everything(__FILE__, __func__, __LINE__, "NOT IMPLEMENTED !!!!!!")
#define warn(Msg) __print_warning(__FILE__, __func__, __LINE__, Msg)
#else
#define assert(...)
#define unreachable(...)
#define unimplemented(...) NOT IMPLEMENTED !!!!!!
#define warn(...)
#endif

// TODO: Test these iteration macros
#define FROM_back(Ptr, N, M, Counter)                                          \
  size_t i_##Counter = N;                                                      \
  for (auto it = &(Ptr)[i_##Counter]; i_##Counter < M; i_##Counter++)
#define FROM(Ptr, N, M) FROM_back(Ptr, N, M, __COUNTER__)

// unsafe scan
#define SCAN_back(Ptr, N, Counter)                                             \
  size_t i_##Counter = N;                                                      \
  for (auto it = Ptr; i_##Counter; i_##Counter--, it++)
#define SCAN(Ptr, N) SCAN_back(Ptr, N, __COUNTER__)

#define FOR_back(ArrayPtr, Counter)                                            \
  size_t i_##Counter = 0;                                                      \
  for (auto it = (ArrayPtr)->data; i_##Counter < (ArrayPtr)->count;           \
       i_##Counter++, it++)
#define FOR(ArrayPtr) FOR_back(ArrayPtr, __COUNTER__)

#define FOR_IDX_back(Elem, ArrayPtr, Counter)                                  \
  auto Elem = (ArrayPtr)->data;                                               \
  for (auto it_index = 0; it_index < (ArrayPtr)->count;                        \
       it_index++, Elem++)
#define FOR_IDX(Elem, ArrayPtr) FOR_IDX_back(Elem, ArrayPtr, __COUNTER__)

#define UNIQUE(str) \
  Cat(str, __COUNTER__)

namespace _scope_guards {
enum class ScopeGuardOnExit {};

template <typename F> class ScopeGuard {
  F fn;
  ScopeGuard(F _fn) : fn(_fn){};
  ~ScopeGuard() { fn(); }
};

template <typename F>
ScopeGuard<F> operator+(ScopeGuardOnExit, F &&fn) {
  return ScopeGuard<F>(std::forward<F>(fn));
}
}

#define defer \
  auto UNIQUE(deference_) = _scope_guards::ScopeGuardOnExit() + [&]()

struct block_t {
  void* data;
  size_t size;
  struct block_t* next;
};

namespace mem {
static inline block_t alloc(size_t size) {
  void* data = ::malloc(size);
  if (!data) return {0};
  return { data, size, 0 };
}

static inline block_t realloc(block_t blk, size_t new_size) {
  blk.data = ::realloc(blk.data, new_size);
  blk.size = new_size;
  return blk;
};

static inline void free(block_t* blk) {
  ::free(blk->data);
};
}

template <typename T, int N> struct Mini_Array {
  T *start, *end;
  size_t cap;
  u8* data[N * sizeof(T)];

  Mini_Array() {
    end = start = (T*)data;
    cap = N;
  }

  T& operator[](int i) {
    assert(start + i < end);
    return start[i];
  }

  void push(T item) {
    assert(end < data + cap);
    *end++ = item;
  }

  T pop() {
    assert(end != start);
    return *end--;
  }
};

// TODO: Test the array
// A destructor-free array.
template <typename T> struct Array {
  size_t count;
  size_t cap;
  T* data;

  Array(size_t _cap = 0) { initialize(_cap); }
  T& operator[](int i) {
    assert(i > 0 && i < count);
    return data[i];
  }

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
      while (cap_needed >= cap) cap *= 2;
      __allocate();
    }
  }

  void free() {
    cap = 0;
    count = 0;
    __allocate();
  }

  T first() { return data[0]; }
  T last() { return data[count - 1]; }
  T* end_ptr() { return data + (count - 1); }
  T* begin_ptr() { return data; }
  T* at(size_t idx) {
    assert(idx > 0 && idx < count);
    return data + idx;
  }

  void push(T value) {
    __maybe_realloc(1);
    data[count++] = value;
  }
  T pop() { return data[--count]; }


  // add n unintialized items and return a ptr to the first
  T* add(int n) {
    __maybe_realloc(n);
    count += n;
    return &data[count - n];
  }

  void fast_remove(size_t idx) {
    assert(idx < count);
    data[idx] = pop();
  }

  void remove(size_t idx) {
    assert(idx < count);
    while (idx < count) data[idx] = data[++idx];
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
  void zero() { util::memzero<T>(data, count); }
  void zero_all() { util::memzero<T>(data, cap); }
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

struct Token_Chunk {
  Array<Token> tokens;
  Buffer* file;
};

struct Scanner {
  Buffer* buf;

  Scanner(Buffer* _buf) : buf(_buf) {}

  u8 peek() { return buf->last(); }

  bool at_end() { return buf->count == buf->cap; }

  bool match(const char* pattern) {
    int len = strlen(pattern);
    if (len > (buf->cap - buf->count)) return false;
    return strncmp(
      pattern,
      (const char *)buf->end_ptr(),
      len) == 0;
  }

  u8 peek_next() {
    if (at_end() || (buf->count + 1) == buf->cap) return '\0';
    u8 result = buf->end_ptr()[1];
    return result;
  }

  u8 advance() {
    if (at_end()) return '\0';
    buf->count++;
    return peek();
  }

  void eat_whitespace() {
    while (IsWhitespace(peek())) advance();
  }
};
