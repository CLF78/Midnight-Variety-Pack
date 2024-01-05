#include <common/Common.h>
#include <game/net/RKNetStatusData.hpp>

////////////////////////////
// Wiimmfi Security Fixes //
////////////////////////////

// Fix a potential buffer overflow in DWC_GetFriendStatusData by enforcing the destination buffer size
kmWrite32(0x800CE220, 0x38C00000 | sizeof(RKNetStatusData[2]));
