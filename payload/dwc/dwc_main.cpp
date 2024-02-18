#include <common/Common.hpp>
#include <dwc/dwc_main.h>
#include <gs/gt2/gt2Utility.h>
#include <wiimmfi/Natneg.hpp>
#include <wiimmfi/Port.hpp>

/////////////////
// Fast NATNEG //
/////////////////

// DWCi_MatchedCallback() patch
// Update NATNEG
kmCallDefCpp(0x800D3188, void, DWCError error, int cancel, int self, int isServer, int index, void* param) {

    // Original call
    stpDwcCnt->userMatchedCallback(error, cancel, self, isServer, index, param);

    // Update NATNEG with the self value
    Wiimmfi::Natneg::Calc(self);
}

//////////////////////////
// Wiimmfi Port Binding //
//////////////////////////

// DWCi_GT2Startup() patch
// Use the server-provided port for GT2Sockets
kmCallDefCpp(0x800D2884, char*, u32 ip, u16 port, char* string) {

    // Override port if set
    if (Wiimmfi::Port::sPort)
        port = Wiimmfi::Port::sPort;

    // Original call
    return gt2AddressToString(ip, port, string);
}
