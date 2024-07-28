#pragma once
#include <common/Common.hpp>
#include <game/net/packet/RKNetUserPacket.hpp>

class RKNetUSERHandler {
public:
    void SetSendUSERPacket(u8 aid);

    bool isPrepared;
    // 7 bytes padding

    RKNetUSERPacket sendPacket;
    RKNetUSERPacket recvPackets[12];

    u8 unk[0x9EC - 0x9C8];
    // 4 bytes padding

    static RKNetUSERHandler* instance;
};
size_assert(RKNetUSERHandler, 0x9F0);
