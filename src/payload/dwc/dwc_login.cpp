#include "dwc_login.h"
#include "dwc_report.h"
#include <wiimmfi/Natify.hpp>

/////////////////////
// Error 60000 Fix //
/////////////////////

// Credits: NitroDWC
inline u32 DWCi_Acc_GetMaskBits(u32 data, u32 shift, u32 mask) {
    return ((data >> shift) & mask);
}

// Credits: NitroDWC
BOOL DWCi_Acc_SetMaskBits(u32* ptr, u32 data, u32 shift, u32 mask) {
    if ((data & ~mask) != 0)
        return FALSE;

    *ptr = (*ptr & ~(mask << shift)) | (data << shift);
    return TRUE;
}

// Credits: NitroDWC
u32 DWCi_Acc_GetFlags(const DWCAccFlag* userdata) {
    return DWCi_Acc_GetMaskBits(userdata->flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK);
}

// Credits: NitroDWC
void DWCi_Acc_SetFlags(DWCAccFlag* userdata, u32 flags) {
    DWCi_Acc_SetMaskBits(&userdata->flags, flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK);
}

// Credits: NitroDWC
void DWCi_Acc_SetFlag_DataType(DWCAccFlag* userdata, int type) {
    u32 flags = DWCi_Acc_GetFlags(userdata);
    flags = (flags & ~DWC_FRIENDDATA_MASK) | type;
    DWCi_Acc_SetFlags(userdata, flags);
}

// DWCi_GPConnectCallback() patch
// Clear the Profile ID if error 60000 is encountered
// Credits: Wiimmfi
kmBranchDefCpp(0x800D05A8, 0x800D0610, void) {

    // Debug report
    LOG_DEBUG("Invalid profile ID detected, requesting new one...");

    // Set state to GPGETINFO (AKA lastname acquisition during GP login)
    stpLoginCnt->state = DWC_LOGIN_STATE_GPGETINFO;

    // Set the user ID and the data type
    DWCi_Acc_SetUserId(&stpLoginCnt->tempLoginId, DWCi_Auth_GetConsoleUserId());
    DWCi_Acc_SetFlag_DataType((DWCAccFlag*)&stpLoginCnt->tempLoginId, DWC_FRIENDDATA_LOGIN_ID);

    // Replace the player ID with the pseudo one and mark user data as dirty
    stpLoginCnt->tempLoginId.playerId = stpLoginCnt->userdata->pseudo.playerId;
    stpLoginCnt->userdata->flag = DWC_USER_DATA_FLAG_DIRTY;
}

/////////////////////////
// NATify Improvements //
/////////////////////////

// DWCi_RemoteLoginProcess() patch
// Start custom NATify procedure
// Credits: Wiimmfi
kmCallDefCpp(0x800D086C, void, DWCReportFlag level, const char* str) {

    // Original call
    DWC_Printf(level, str);

    // Start NATify
    Wiimmfi::Natify::Start();
}
