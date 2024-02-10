#include <common/Common.hpp>
#include <gs/gt2/gt2Utility.h>
#include <wiimmfi/Port.hpp>

//////////////////////////
// Wiimmfi Port Binding //
//////////////////////////

// DWCi_GT2Startup() patch
// Use the server-provided port for GT2Sockets
kmCallDefCpp(0x800D2884, char*, u32 ip, u16 port, char* string) {

    // Override port if set
    if (Wiimmfi::Port::port)
        port = Wiimmfi::Port::port;

    // Original call
    return gt2AddressToString(ip, port, string);
}
