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

    // Dummy preprocessor macros
    #define REPLACE
    #define REPLACE_STATIC
    #define REPLACED(...) NULL
    #define BRANCH_CPP(fn) ;
    #define CALL_CPP(fn) ;

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
