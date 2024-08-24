#pragma once

struct RKNetRACEDATAPacket {
    u8 data[0x40];
};
size_assert(RKNetRACEDATAPacket, 0x40);
