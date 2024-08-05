#pragma once
#include "RKNetStatusData.hpp"

struct RKNetFriend {
    RKNetStatusData statusData;
    u8 status; // see DWCFriendStatus
    u8 flags; // bit flags, 4 if room open, 1 if online
    UNK(2);
};
size_assert(RKNetFriend, 0xC);
