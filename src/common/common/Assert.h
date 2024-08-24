#pragma once

// Static asserts for C structures are implemented by tripping the compiler on a ghost struct
#ifdef __CLANGD__
    #define static_cassert(assert) _Static_assert(assert, #assert)
#else
    #define static_cassert(assert) struct { u8 test [((assert) == 1) - 1]; }
    #define static_assert(cond)    __static_assert((cond), #cond)
#endif

// Define size asserts to simplify usage
#define size_cassert(type, size) static_cassert(sizeof(type) == (size))
#define size_assert(type, size)  static_assert(sizeof(type) == (size))

// Handle a special case of Windows incompetence with bitfield alignment
#if (defined(__CLANGD__) && defined(_WIN32))
    #define size_cassert_bitfield(type, size)
    #define size_assert_bitfield(type, size)
#else
    #define size_cassert_bitfield(type, size) size_cassert(type, size)
    #define size_assert_bitfield(type, size)  size_assert(type, size)
#endif
