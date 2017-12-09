#pragma once

#include <assert.h>
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
