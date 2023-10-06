#include <kamek.h>

#ifdef __cplusplus
extern "C" {
#endif

u32 PPCmfecid_u() {
    register u32 ecidU;
    asm { mfspr ecidU, 924 }
    return ecidU;
}

u32 PPCMfecid_m() {
    register u32 ecidM;
    asm { mfspr ecidM, 925 }
    return ecidM;
}

u32 PPCMfecid_l()  {
    register u32 ecidL;
    asm { mfspr ecidL, 926 }
    return ecidL;
}

#ifdef __cplusplus
}
#endif
