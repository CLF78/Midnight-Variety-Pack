/*
 * Kamek Standard Library
 * Wii game patching engine
 * (c) Treeki 2010-2018
 */

// Base asserts
#ifndef __CWCC__
#error "Kamek requires the CodeWarrior compiler!"
#endif

#if (!(defined(CODE_REGION_P) || defined(CODE_REGION_E) || defined(CODE_REGION_J) || defined(CODE_REGION_K) || defined(CODE_REGION_ALL)))
#error "Missing region specifier!"
#endif

// Defines
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
typedef void (*Func)();

// Macros
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define ALIGN(a) __attribute__ ((aligned(a)))
#define MIN(a, b) (((b) < (a)) ? (b) : (a))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#define STRINGIFY(x) #x

#ifdef __INTELLISENSE__
    #define AT_ADDR(addr)
    #define IGNORE_ERR(err) _Pragma(STRINGIFY(diag_suppress err))
    #define UNIGNORE_ERR(err) _Pragma(STRINGIFY(diag_default err))
#else
    #define AT_ADDR(addr) : (addr);
    #define IGNORE_ERR(err)
    #define UNIGNORE_ERR(err)
#endif

// C-specific stuff
#ifndef __cplusplus
#define bool s32
#define false 0
#define true 1
#endif

// Include other base headers
#include <kamek/hooks.h>
#include <kamek/gekko.h>
#include <midnight/config.h>
