#pragma once

// Base asserts
#ifndef __CWCC__
#error "Kamek requires the CodeWarrior compiler!"
#endif

#ifndef __CODE_REGION__
#error "Missing region specifier!"
#endif

#ifndef __LOG_LEVEL__
#error "Missing log level specifier!"
#endif

// Defines
#define NULL 0
#define FALSE 0
#define TRUE 1

// Base types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64 __attribute__ ((aligned(8))); // Ensure these are aligned correctly by Clang
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64 __attribute__ ((aligned(8))); // Ensure these are aligned correctly by Clang
typedef float f32;
typedef double f64;
typedef unsigned long ulong;
typedef s32 BOOL;
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

#ifdef __CLANGD__
    typedef unsigned int size_t;
#else
    typedef unsigned long size_t;
#endif

// Macros
#define ALIGN(a) __attribute__ ((aligned(a)))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define ARRAY_SIZE_STATIC(a, b) ARRAY_SIZE(((a*)NULL)->b)
#define BIT_FLAG(bit) ((bit) < 0 ? 0 : 1 << (bit))
#define FORCE_SEMICOLON(x) do { x } while(0)
#define MIN(a, b) (((b) < (a)) ? (b) : (a))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))
#define offsetof(a, b) ((int)(&(((a*)(0))->b)))
#define __PAD(name, counter, bytes) u8 name##counter[bytes]
#define _PAD(name, counter, bytes) __PAD(name, counter, bytes)
#define PAD(bytes) _PAD(pad, __COUNTER__, bytes)
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#define UNK(bytes) _PAD(unk, __COUNTER__, bytes)

#ifdef __CLANGD__
    #define AT_ADDR(addr)
    #define static_cassert(assert) _Static_assert(assert, #assert)
    #define size_cassert(type, size) static_cassert(sizeof(type) == (size))
#else
    #define AT_ADDR(addr) : (addr)
    #define static_cassert(assert) struct { u8 test [((assert) == 1) - 1]; }
    #define size_cassert(type, size) static_cassert(sizeof(type) == (size))
#endif

// Pragmas
#pragma warning off (10211) // Disable switch case variable initialization warning

// Include other base headers
#include <kamek/gekko.h>
#include <kamek/hooks.h>
#include <mvp/Config.h>

// C++ headers only
#ifdef __cplusplus

// Macros
#ifndef __CLANGD__
    #define static_assert(cond) __static_assert((cond), #cond)
    #define size_assert(type, size) static_assert(sizeof(type) == (size))
#else
    #define size_assert(type, size) static_assert(sizeof(type) == (size))
#endif

#include <kamek/ListHook.hpp>
#include <mvp/Log.hpp>
#include <mvp/Region.hpp>
#include <platform/new.hpp>
#endif
