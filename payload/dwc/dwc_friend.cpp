#include <common/Common.h>
#include <game/net/RKNetStatusData.hpp>

///////////////////////////////////////
// Friend Status Buffer Overflow Fix //
///////////////////////////////////////

// DWC_GetFriendStatusData() patch
// Enforce the destination buffer size to prevent an overflow
// Credits: Wiimmfi
kmWrite32(0x800CE220, 0x38C00000 | sizeof(RKNetStatusData[2]));
