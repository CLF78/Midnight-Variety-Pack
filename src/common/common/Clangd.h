#pragma once

// Fixes various non-existent Clangd errors, do not include
#ifdef __CLANGD__

    // Fix size asserts
    #define wchar_t s16

    // Fix assembly functions
    #define asm
    #define nofralloc __asm {
    #define blr }
    #define register
    #define r0 0
    #define r1 1
    #define r2 2
    #define r3 3
    #define r4 4
    #define r5 5
    #define r6 6
    #define r7 7
    #define r8 8
    #define r9 9
    #define r10 10
    #define r11 11
    #define r12 12
    #define r13 13
    #define r14 14
    #define r15 15
    #define r16 16
    #define r17 17
    #define r18 18
    #define r19 19
    #define r20 20
    #define r21 21
    #define r22 22
    #define r23 23
    #define r24 24
    #define r25 25
    #define r26 26
    #define r27 27
    #define r28 28
    #define r29 29
    #define r30 30
    #define r31 31
    #define f0 0
    #define f1 1
    #define f2 2
    #define f3 3
    #define f4 4
    #define f5 5
    #define f6 6
    #define f7 7
    #define f8 8
    #define f9 9
    #define f10 10
    #define f11 11
    #define f12 12
    #define f13 13
    #define f14 14
    #define f15 15
    #define f16 16
    #define f17 17
    #define f18 18
    #define f19 19
    #define f20 20
    #define f21 21
    #define f22 22
    #define f23 23
    #define f24 24
    #define f25 25
    #define f26 26
    #define f27 27
    #define f28 28
    #define f29 29
    #define f30 30
    #define f31 31

    // Dummy preprocessor macros
    #define REPLACE
    #define REPLACE_STATIC
    #define REPLACED(...) NULL
    #define BRANCH_CPP(fn)
    #define CALL_CPP(fn)

    // Fix varargs
    #define __builtin_va_info

    // Provide signatures for intrinsic functions
    void __eieio();
    void __sync();
    void __isync();

    int __abs(int);
    float __fabs(float);
    float __fabsf(float);
    float __fnabs(float);
    float __fnabsf(float);
    long __labs(long);

    int __lhbrx(const void*, int);
    int __lwbrx(const void*, int);
    void __sthbrx(unsigned short, const void*, int);
    void __stwbrx(unsigned int, const void*, int);

    float __setflm(float);

    int __rlwinm(int, int, int, int);
    int __rlwnm(int, int, int, int);
    int __rlwimi(int, int, int, int, int);
    int __cntlzw(int);

    void __dcbf(const void*, int);
    void __dcbt(const void*, int);
    void __dcbst(const void*, int);
    void __dcbtst(const void*, int);
    void __dcbz(const void*, int);
    void __dcba(const void*, int);

    int __mulhw(int, int);
    unsigned int __mulhwu(unsigned int, unsigned int);
    double __fmadd(double, double, double);
    double __fmsub(double, double, double);
    double __fnmadd(double, double, double);
    double __fnmsub(double, double, double);
    float __fmadds(float, float, float);
    float __fmsubs(float, float, float);
    float __fnmadds(float, float, float);
    float __fnmsubs(float, float, float);
    double __mffs();

    void* __alloca(unsigned long);
    char* __strcpy(char*, const char*);
    void* __memcpy(void*, void*, unsigned int);

#endif
