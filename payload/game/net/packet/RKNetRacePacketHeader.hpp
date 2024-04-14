#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>

struct RKNetRACEPacketHeader {
    u32 empty;
    u32 checksum;
    u8 sizes[RKNET_SECTION_COUNT];
};
size_assert(RKNetRACEPacketHeader, 0x10);
