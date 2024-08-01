#pragma once
#include <game/system/Mii.hpp>

struct RKNetUSERPacket {
    u32 recordId;
    u16 miiCount;
    // 2 bytes padding

    RawMii miis[2];
    u64 wiiFc;
    u64 fc;
    u8 country;
    u8 state;
    u16 city;
    u16 longitude;
    u16 latitude;
    u16 vr;
    u16 br;
    u8 gameId;
    u8 region;
    // 2 bytes padding
};
size_assert(RKNetUSERPacket, 0xC0);
