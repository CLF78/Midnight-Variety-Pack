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

    u8 getPlayerCharacter(u8 aid, u8 localPlayerIdx);
    u16 getPlayerVote(u8 aid); // reimplemented function
    u16 getWinningTrack(); // reimplemented function
    bool hasUnprocessedRecvPackets();

    // Reimplemented functions
    bool raceSettingsDetermined() { return sendPacket.battleTeamData.raw != 0; }
    bool trackVoted() { return expansion.sendPacketEx.courseVote != CupData::UNDECIDED_TRACK_VOTE; }
    bool voteDetermined() { return expansion.sendPacketEx.winningCourse != CupData::NO_TRACK; }

    // Reimplemented functions
    void storeUpdatedRaceSettings(u8 aid);
    bool checkUpdatedRaceSettings(u8 aid);
    bool checkUpdatedRaceSettingsAll();

    // Reimplemented functions
    void storeVote(u8 aid);
    bool checkVote(u8 aid);
    bool checkVoteAll();

    // Reimplemented functions
    void storeUpdatedVoteData(u8 aid);
    bool checkUpdatedVoteData(u8 aid);
    bool checkUpdatedVoteDataAll();

    int mode;
    u32 reserved; // previously padding

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
    u32 aidsWithVoteData;
    u32 aidsThatHaveVoted;
    u32 reserved2; // previously padding

    // Custom data
    RKNetSELECTHandlerEx expansion;

    static RKNetSELECTHandler* instance;
};
size_assert(RKNetSELECTHandler, OSRoundUp(0x3F8 + sizeof(RKNetSELECTHandlerEx), 8));
