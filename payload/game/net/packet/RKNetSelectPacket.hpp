#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>
#include <game/system/Identifiers.hpp>
#include <midnight/cup/CupData.hpp>
#pragma pack(push, 1)

struct RKNetSELECTPlayer {

    RKNetSELECTPlayer() :
        prevRaceRank(0),
        sumPoints(0),
        character(CHARACTER_COUNT),
        vehicle(VEHICLE_COUNT),
        reserved(0),
        starRank(0) {}

    u16 prevRaceRank;
    u16 sumPoints;
    u8 character;
    u8 vehicle;

    // Modified structure
    u8 reserved;
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
        reserved(0),
        phase(PREPARE),
        winningVoterAid(0xFF) {}

    s64 timeSender;
    s64 timeReceived;
    RKNetSELECTPlayer playerData[2];
    u32 selectId;
    RKNetBattleTeamData battleTeamData;
    RKNetAidPidMap aidPidMap;

    // Modified structure
    u8 reserved;
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
        winningCourse(CupData::NO_TRACK) {}

    u16 courseVote;
    u16 winningCourse;
};

// Custom expansion structure
struct RKNetSELECTPacketEx : RKNetSELECTPacket, RKNetSELECTPacketExpansion {
    RKNetSELECTPacketEx() {}
};

#pragma pack(pop)
