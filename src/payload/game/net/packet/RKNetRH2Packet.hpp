#include <common/Common.hpp>

// TODO actual structure
struct RKNetRH2Packet {
    u8 data[0x28];
};
size_assert(RKNetRH2Packet, 0x28);
