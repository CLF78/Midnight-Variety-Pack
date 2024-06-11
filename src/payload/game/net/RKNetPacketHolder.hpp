#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>

class RKNetPacketHolder {
public:
    void copyData(void* data, u32 dataSize);
    void appendData(void* data, u32 dataSize);
    void clear();

    void* buffer;
    u32 bufferSize;
    u32 currentPacketSize;
};
size_assert(RKNetPacketHolder, 0xC);

union RKNetRACEPacketHolder {
    struct {
        RKNetPacketHolder* header;
        RKNetPacketHolder* raceHeader1;
        RKNetPacketHolder* raceHeader2;
        RKNetPacketHolder* select;
        RKNetPacketHolder* racedata;
        RKNetPacketHolder* user;
        RKNetPacketHolder* item;
        RKNetPacketHolder* event;
    };

    RKNetPacketHolder* sections[RKNET_SECTION_COUNT];
};
size_assert(RKNetRACEPacketHolder, 0x20);
