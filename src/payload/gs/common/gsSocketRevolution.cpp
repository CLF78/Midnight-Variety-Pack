#include "gsSocketRevolution.h"
#include <dwc/dwc_common.h>

//////////////////////////
// Wiimmfi Port Binding //
//////////////////////////

// Try to bind on the custom port, retry on a different port on failure
// Credits: Wiimmfi
REPLACE int bind(int sock, SOSockAddrIn* addr, int len) {

    // Try binding to the given port
    int ret = REPLACED(sock, addr, sizeof(*addr));
    if (ret >= 0) {
        return ret;
    }

    // If binding fails, try on a different random port up to 10 times
    LOG_WARN("Bind failed on port %d.", addr->port);
    for (int i = 0; i < 10; i++) {
        addr->port = (DWCi_GetMathRand32(0x4000) + 0xC000) & 0xFFFF;
        ret = REPLACED(sock, addr, sizeof(*addr));
        if (ret >= 0) {
            LOG_DEBUG("Bind successful on alternate port %d", addr->port);
            return ret;
        }
    }

    // Everything failed, give up
    LOG_ERROR("Could not bind to any port");
    return -1;
}
