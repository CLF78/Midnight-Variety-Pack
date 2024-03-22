#include <common/Common.hpp>
#include <game/net/packet/RKNetEventPacket.hpp>
#include <game/net/packet/RKNetRacedataPacket.hpp>

class RKNetPacketCreator {
public:
    bool isOnline;
    bool isSomeoneLagging;
    // 2 bytes padding

    u32 aidsWithSelectId;
    u32 aidsWithNewRoomOrSelect;
    u32 aidsWithRH1Value17At1;
    s16 lagFrames;
    s16 countdownTimer;
    RKNetRACEDATAPacket sendRacedataPackets[2];
    RKNetEVENTPacket sendEventPacket;
    // 4 bytes padding

    s64 lastFriendUpdateTime;
    u32 playerRh1Timers[12];
};
size_assert(RKNetPacketCreator, 0x1C8);
