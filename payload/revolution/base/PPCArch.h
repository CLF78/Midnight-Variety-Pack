#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////
// Registers //
///////////////
#define ECID_U 924
#define ECID_M 925
#define ECID_L 926

//////////
// ECID //
//////////
#define DEFAULT_ECID_U 0x0D96E200
#define DEFAULT_ECID_M 0x1840C00D
#define DEFAULT_ECID_L 0x82BB08E8

///////////
// FPSCR //
///////////
typedef enum {
    FPSCR_FX = 0x80000000, // Exception summary
    FPSCR_FEX = 0x40000000, // Enabled exception summary
    FPSCR_VX = 0x20000000, // Invalid operation
    FPSCR_OX = 0x10000000, // Overflow exception
    FPSCR_UX = 0x8000000, // Underflow exception
    FPSCR_ZX = 0x4000000, // Zero divide exception
    FPSCR_XX = 0x2000000, // Inexact exception
    FPSCR_VXSNAN = 0x1000000, // SNaN
    FPSCR_VXISI = 0x800000, // Infinity - Infinity
    FPSCR_VXIDI = 0x400000, // Infinity / Infinity
    FPSCR_VXZDZ = 0x200000, // 0 / 0
    FPSCR_VXIMZ = 0x100000, // Infinity * 0
    FPSCR_VXVC = 0x80000, // Invalid compare
    FPSCR_FR = 0x40000, // Fraction rounded
    FPSCR_FI = 0x20000, // Fraction inexact
    FPSCR_VXSOFT = 0x400, // Software request
    FPSCR_VXSQRT = 0x200, // Invalid square root
    FPSCR_VXCVI = 0x100, // Invalid integer convert
    FPSCR_VE = 0x80, // Invalid operation exception enable
    FPSCR_OE = 0x40, // Overflow exception enable
    FPSCR_UE = 0x20, // Underflow exception enable
    FPSCR_ZE = 0x10, // Zero divide exception enable
    FPSCR_XE = 0x8, // Inexact exception enable
    FPSCR_NI = 0x4, // Non-IEEE mode
} FPSCR;

///////////////
// Functions //
///////////////

inline u32 PPCGetECID_U() {
    register u32 val;
    __asm__ (mfspr val, ECID_U);
    return val;
}

inline u32 PPCGetECID_M() {
    register u32 val;
    __asm__ (mfspr val, ECID_M);
    return val;
}

inline u32 PPCGetECID_L()  {
    register u32 val;
    __asm__ (mfspr val, ECID_L);
    return val;
}

#ifdef __cplusplus
}
#endif
