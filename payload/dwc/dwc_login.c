#include <common/Common.h>
#include <dwc/dwc_login.h>

/////////////////////
// Error 60000 Fix //
/////////////////////

// DWCi_GPConnectCallback() patch
// Clear the Profile ID if error 60000 is encountered
// Credits: Wiimmfi
kmBranchDefCpp(0x800D05A8, 0x800D0610, void) {
    u64 userId = DWCi_Auth_GetConsoleUserId();
    stpLoginCnt->state = DWC_LOGIN_STATE_GPGETINFO;
    stpLoginCnt->tempLoginId.userId[0] = (userId >> 32) | 0x800; // this OR might be to prevent Nice FCs, not sure tho
    stpLoginCnt->tempLoginId.userId[1] = userId & 0xFFFFFFFF;
    DWCUserData* userData = stpLoginCnt->userdata;
    stpLoginCnt->tempLoginId.playerId = userData->pseudo.playerId;
    userData->flag = 1;
}
