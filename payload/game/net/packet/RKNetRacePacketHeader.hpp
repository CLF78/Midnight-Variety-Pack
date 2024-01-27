#include <common/Common.hpp>

struct RKNetRACEPacketHeader {

    enum Section {
        HEADER,
        RACEHEADER_1,
        RACEHEADER_2,
        ROOM_SELECT,
        RACEDATA,
        USER,
        ITEM,
        EVENT,
    };

    u32 empty;
    u32 crc;
    u8 sizes[8];
};
size_assert(RKNetRACEPacketHeader, 0x10);
