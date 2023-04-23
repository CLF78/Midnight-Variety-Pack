/*
 * Kamek Standard Library
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

#ifndef __MWERKS__
#error "Kamek requires the CodeWarrior compiler!"
#endif

#define static_assert(cond) __static_assert((cond), #cond)
#define size_assert(type, size) static_assert(sizeof(type) == (size))
#define NULL 0

// Base types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;
typedef unsigned long size_t;
typedef unsigned long ulong;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;

// Macros
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "kamek/hooks.h"
#include "kamek/gekko.h"
#include "config.h"
