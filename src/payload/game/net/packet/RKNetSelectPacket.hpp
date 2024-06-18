#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>
#include <game/system/Identifiers.hpp>
#include <midnight/cup/CupData.hpp>
#include <midnight/online/RepickQueue.hpp>
#pragma pack(push, 1)

struct RKNetSELECTPlayer {

    RKNetSELECTPlayer() :
        prevRaceRank(0),
        sumPoints(0),
        character(CHARACTER_COUNT),
        vehicle(VEHICLE_COUNT),
        vanillaCourseVote(CupData::VANILLA_UNDECIDED_TRACK_VOTE),
        starRank(0) {}

    u16 prevRaceRank;
    u16 sumPoints;
    u8 character;
    u8 vehicle;

    // Modified structure
    u8 vanillaCourseVote;
    // u8 courseVote;

    u8 starRank;
};
size_assert(RKNetSELECTPlayer, 0x8);

struct RKNetSELECTPacket {

    enum Phase {
        PREPARE,
        VOTING,
        LOTTERY,
    };

    RKNetSELECTPacket() :
        timeSender(0),
        timeReceived(0),
        playerData(),
        selectId(0),
        battleTeamData(),
        aidPidMap(),
        vanillaWinningCourse(CupData::VANILLA_NO_TRACK),
        phase(PREPARE),
        winningVoterAid(0xFF) {}

    s64 timeSender;
    s64 timeReceived;
    RKNetSELECTPlayer playerData[2];
    u32 selectId;
    RKNetBattleTeamData battleTeamData;
    RKNetAidPidMap aidPidMap;

    // Modified structure
    u8 vanillaWinningCourse;
    // u8 winningCourse;

    u8 phase; // 0 = prepare, 1 = voting, 2 = lottery
    u8 winningVoterAid;
    RKNetEngineClassData engineClass;
};
size_assert(RKNetSELECTPacket, 0x38);

// TODO add whatever else we might need in the future
struct RKNetSELECTPacketExpansion {

    RKNetSELECTPacketExpansion() :
        courseVote(CupData::UNDECIDED_TRACK_VOTE),
        winningCourse(CupData::NO_TRACK),
        repickQueue() {}

    u16 courseVote;
    u16 winningCourse;

    RepickQueue::RawQueue repickQueue;
};

// Custom expansion structure
struct RKNetSELECTPacketEx : RKNetSELECTPacket, RKNetSELECTPacketExpansion {
    RKNetSELECTPacketEx() {}
};

#pragma pack(pop)
