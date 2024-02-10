#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 len;    // size of socket address structure
    u8 family; // address family
    u16 port;  // port number
    u32 addr;  // ip address
} SOSockAddrIn;

#ifdef __cplusplus
}
#endif
