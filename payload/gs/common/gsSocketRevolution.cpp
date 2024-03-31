#include <common/Common.hpp>
#include <dwc/dwc_common.h>
#include <gs/common/gsSocketRevolution.h>

//////////////////////////
// Wiimmfi Port Binding //
//////////////////////////

// Try to bind on the custom port, retry on a different port on failure
// Credits: Wiimmfi
REPLACE int bind(int sock, SOSockAddrIn* addr, int len) {

    // Try binding to the given port
    int ret = REPLACED(sock, addr, sizeof(*addr));
    if (ret >= 0)
        return ret;

    // If binding fails, try on a different random port up to 10 times
    DEBUG_REPORT("[WIIMMFI_PORT] Bind failed on port %d\n", addr->port)
    for (int i = 0; i < 10; i++) {
        addr->port = (DWCi_GetMathRand32(0x4000) + 0xC000) & 0xFFFF;
        ret = REPLACED(sock, addr, sizeof(*addr));
        if (ret >= 0) {
            DEBUG_REPORT("[WIIMMFI_PORT] Bind successful on alternate port %d\n", addr->port)
            return ret;
        }
    }

    // Everything failed, give up
    return -1;
}
