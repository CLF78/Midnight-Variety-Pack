#include <common/Common.hpp>

enum RKNetStatus {
    STATUS_INACTIVE,
    STATUS_WAITING = 2,
    STATUS_ROOM_HOST,
    STATUS_ROOM_GUEST,
    STATUS_PUBLIC_VS,
    STATUS_PUBLIC_BT = 8,
    STATUS_PRIVATE_VS_HOST = 0xB,
    STATUS_PRIVATE_VS_GUEST,
    STATUS_PRIVATE_BT_HOST,
    STATUS_PRIVATE_BT_GUEST,
};

struct RKNetStatusData {
    u32 groupId;
    u8 searchRegion; // 0xFF for Worldwides
    u8 status; // see enum above
    u8 playerCount;
    u8 raceNumber;
};
