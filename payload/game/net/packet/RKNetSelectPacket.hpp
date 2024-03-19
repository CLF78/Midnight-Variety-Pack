#include <common/Common.hpp>
#include <game/net/packet/RKNetPacketCommon.hpp>

/*
TODO update this structure to fit extra data:
- Make courseVote an u16 and change the 0x43/0xFF values to 0xFFFE and 0xFFFF
    - Space-saving approach 1: make prevRaceRank an u8 and move starRank after it
    - Space-saving approach 2: remove starRank and determine the value through something else (VR?)
- An u8 should suffice to store characters and vehicles
*/
struct RKNetSELECTPlayer {
    u16 prevRaceRank;
    u16 sumPoints;
    u8 character;
    u8 vehicle;
    u8 courseVote;
    u8 starRank;
};
size_assert(RKNetSELECTPlayer, 0x8);

/*
TODO update this structure to fit extra data:
- Make winningCourse an u16 and change the 0xFF value to 0xFFFF
- Add any other setting we wish to send over
*/
struct RKNetSELECTPacket {
    s64 timeSender;
    s64 timeReceived;
    RKNetSELECTPlayer playerData[2];
    u32 selectId;
    RKNetBattleTeamData battleTeamData;
    RKNetAidPidMap aidPidMap;
    u8 winningCourse;
    u8 phase; // 0 = prepare, 1 = voting, 2 = lottery
    u8 winningVoterAid;
    u8 engineClass;
};
size_assert(RKNetSELECTPacket, 0x38);
