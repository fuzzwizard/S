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

void __STOP(const char* file, const char* function, int line,
            const char* message1, const char* message2 = 0) {
  printf("#############################################\n");
  printf("# %s", message1);
  if (message2) {
    printf("# %s", message2);
  }
  printf("@ file: %s, func: %s, line: %d", file, function, line);
  printf("#############################################\n");
  exit(1);
}

#ifdef CS_SLOW

#define assert(Cond)                                                           \
  ((Cond)                                                                      \
       ? ((void)0)                                                             \
       : (__STOP(__FILE__, __func__, __LINE__, "Assertation failed: " #Cond)))
#define assertm(Cond, Msg)                                                     \
  ((Cond) ? ((void)0)                                                          \
          : (__STOP(__FILE__, __func__, __LINE__,                              \
                    "Assertation failed: " #Cond, Msg)))
#define unreachable(...)                                                       \
  __STOP(__FILE__, __func__, __LINE__, "Reached unreachable code.")
#define unimplemented(...)                                                     \
  __STOP(__FILE__, __func__, __LINE__, "Reached unreachable code.")

#else

#define assert(...)
#define unreachable(...)
#define unimplemented(...)

#endif

struct Buffer {
  u8* data;
  size_t data_size;
  size_t occupied;
};

struct String_Builder {
  Buffer current_buffer;
  size_t consumed;
  String_Builder* next_builder;
};