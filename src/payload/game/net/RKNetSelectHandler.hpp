#pragma once
#include "packet/RKNetSelectPacket.hpp"
#include <mvp/online/RepickQueue.hpp>
#include <revolution/os/OS.h>

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
    RKNetSELECTPlayer* getPlayerData(u8 aid, u8 localPlayerIdx);
    u16 getPlayerVote(u8 aid);
    u32 getRandomSeed();
    u16 getWinningTrack();
    u8 getWinningVoter();

    void setPlayerData(int characterId, int vehicleId, int courseVote, int localPlayerIdx, u8 starRank);

    void decideBattleAndTeams();
    void decideEngineClass();
    void decideSelectId();
    void decidePidToAidMap();
    void decideTrack();
    void copyRepickQueue() { expansion.sendPacketEx.repickQueue = RepickQueue::instance.queue; }

    bool raceSettingsDetermined() { return sendPacket.battleTeamData.raw != 0; }
    bool trackVoted() { return expansion.sendPacketEx.courseVote != CupData::UNDECIDED_TRACK_VOTE; }
    bool voteDetermined() { return expansion.sendPacketEx.winningCourse != CupData::NO_TRACK; }

    void storeUpdatedRaceSettings(u8 aid);
    bool checkUpdatedRaceSettings(u8 aid);
    bool checkUpdatedRaceSettingsAll();

    void storeVote(u8 aid);
    bool checkVote(u8 aid);
    bool checkVoteAll();

    void storeUpdatedVoteData(u8 aid);
    bool checkUpdatedVoteData(u8 aid);
    bool checkUpdatedVoteDataAll();

    void calcPhase();
    bool hasUnprocessedRecvPackets();
    void importNewPackets();
    void handleRecvPacketDelay(u8 aid);

    void prepareSendPacket(u8 aid, s64 sendTime);
    void setSendPacket();

    static void getStaticInstance();

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
