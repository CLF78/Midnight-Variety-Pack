#include <common/Common.hpp>
#include <dwc/dwc_report.h>
#include <midnight/util/DolphinDevice.hpp>
#include <platform/string.h>
#include <revolution/sc.h>
#include <revolutionex/nhttp/NHTTP.h>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Challenge.hpp>

////////////////////////////
// Wiimmfi Authentication //
////////////////////////////

// DWCi_Auth_SendRequest() patch
// Insert custom authentication parameters
// Credits: Wiimmfi
kmCallDefCpp(0x800EDEE8, void, NHTTPReq* req, const char* key, const char* val) {
    NHTTPAddPostDataAscii(req, key, val); // original call
    Wiimmfi::Auth::AppendAuthParameters(req);
}

// DWCi_Auth_HandleResponse() patch
// Parse the custom response data
// Credits: Wiimmfi
kmCallDefCpp(0x800EEA08, void, DWCReportFlag level, const char* fmt, const char* arg) {
    DWC_Printf(level, fmt, arg); // original call
    Wiimmfi::Auth::ParseAuthResponse(arg);
}

/////////////////////////////
// Wiimmfi Challenge Check //
/////////////////////////////

// DWCi_Auth_HandleResponse() patch
// Save the login challenge for sending later
// Credits: Wiimmfi
kmCallDefCpp(0x800EE74C, void, char* dest, const char* src) {
    strcpy(dest, src); // original call
    Wiimmfi::Challenge::Save(src);
}
