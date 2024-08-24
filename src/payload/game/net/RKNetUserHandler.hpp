#pragma once
#include "packet/RKNetUserPacket.hpp"

class RKNetUSERHandler {
public:
    void SetSendUSERPacket(u8 aid);

    bool isPrepared;
    PAD(7);

    RKNetUSERPacket sendPacket;
    RKNetUSERPacket recvPackets[12];

    UNK(0x9EC - 0x9C8);
    PAD(4);

    static RKNetUSERHandler* instance;
};
size_assert(RKNetUSERHandler, 0x9F0);
