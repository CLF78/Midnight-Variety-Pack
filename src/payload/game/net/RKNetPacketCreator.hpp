#pragma once
#include "packet/RKNetEventPacket.hpp"
#include "packet/RKNetRacedataPacket.hpp"

class RKNetPacketCreator {
public:
    void createRH1Racer();
    void createRH1Spectator();

    bool isOnline;
    bool isSomeoneLagging;
    PAD(2);

    u32 aidsWithSelectId;
    u32 aidsWithNewRoomOrSelect;
    u32 aidsWithRH1Value17At1;
    s16 lagFrames;
    s16 countdownTimer;
    RKNetRACEDATAPacket sendRacedataPackets[2];
    RKNetEVENTPacket sendEventPacket;
    PAD(4);

    s64 lastFriendUpdateTime;
    u32 playerRh1Timers[12];
};
size_assert(RKNetPacketCreator, 0x1C8);
