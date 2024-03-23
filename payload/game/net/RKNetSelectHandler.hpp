#include <common/Common.hpp>
#include <game/net/packet/RKNetSelectPacket.hpp>
#include <revolution/os/OS.h>
#include <platform/new.hpp>

class RKNetSELECTHandlerEx {
public:
    RKNetSELECTHandlerEx() : sendPacketEx(), recvPacketsEx() {}
    static RKNetSELECTHandlerEx* construct(void* buffer) { return new(buffer) RKNetSELECTHandlerEx; }

    RKNetSELECTPacketExpansion sendPacketEx;
    RKNetSELECTPacketExpansion recvPacketsEx[12];
};

class RKNetSELECTHandler {
public:
    enum Mode {
        MODE_NONE,
        MODE_PUBLIC_VS,
        MODE_PUBLIC_BATTLE,
        MODE_PRIVATE_VS,
        MODE_PRIVATE_BATTLE,
    };

    int mode;
    // 4 bytes padding

    RKNetSELECTPacket sendPacket;
    RKNetSELECTPacket recvPackets[12];
    u8 lastSentToAid;
    // 7 bytes padding

    s64 lastSentTime;
    s64 lastRecvTimes[12];
    s64 lastRecvDelays[12];
    u32 timelyRecvPacketCounts[12];

    // Bitfields follow
    u32 aidsWithNewSelect;
    u32 aidsWithNewRH1;
    u32 aidsWithNewRaceSettings;
    u32 aidsWithAccurateAidPidMap;
    u32 aidsThatHaveVoted;
    u32 reserved; // previously padding

    // Custom data
    RKNetSELECTHandlerEx expansion;

    static RKNetSELECTHandler* instance;
};
size_assert(RKNetSELECTHandler, OSRoundUp(0x3F8 + sizeof(RKNetSELECTHandlerEx), 8));
