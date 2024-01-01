#include <common/Common.hpp>
#include <dwc/dwc_login.h>
#include <dwc/dwc_report.h>
#include <revolutionex/nhttp/NHTTP.h>
#include <wiimmfi/Auth.hpp>

////////////////////////////
// Wiimmfi Authentication //
////////////////////////////

// Insert custom authentication parameters
kmCallDefCpp(0x800EDEE8, void, NHTTPReq* req, const char* key, const char* val) {

    // Original call
    NHTTPAddPostDataAscii(req, key, val);

    // Send Wiimmfi data
    Wiimmfi::Auth::AppendAuthParameters(req);
}

// Parse the custom response data
kmCallDefCpp(0x800EEA08, void, DWCReportFlag level, const char* fmt, const char* arg) {

    // Original call
    DWC_Printf(level, fmt, arg);

    // Parse the response data
    Wiimmfi::Auth::ParseAuthResponse(arg);
}

/////////////////////
// Error 60000 Fix //
/////////////////////

kmBranchDefCpp(0x800D05A8, 0x800D0610, void) {

    // Code ported from Wiimmfi without changes, i don't know exactly what it is doing...
    u64 userId = DWCi_Auth_GetConsoleUserId();
    stpLoginCnt->state = DWC_LOGIN_STATE_GPGETINFO;
    stpLoginCnt->tempLoginId.userId[0] = (userId >> 32) | 0x800; // this OR might be to prevent Nice FCs, not sure tho
    stpLoginCnt->tempLoginId.userId[1] = userId & 0xFFFFFFFF;
    DWCUserData* userData = stpLoginCnt->userdata;
    stpLoginCnt->tempLoginId.playerId = userData->pseudo.playerId;
    userData->flag = 1;
}
