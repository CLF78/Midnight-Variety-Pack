#include <kamek.h>

class GlobalContext {
public:
    u8 unk[0x60];

    u32 currentRace;
    u32 raceCount;
    u32 redTeamVictories;
    u32 blueTeamVictories;
    int lastVictoryTeam;
    u32 _74;

    // Modified
    u16* trackOrder;
    u32 nextTracks[31]; // free for use

    int vsRaceLimit;

    // Modified
    u16* arenaOrder;
    u32 nextArenas[9]; // free for use

    u32 humanPlayerCount;
    u32 _128;

    int playerCharacters[4];
    int playerVehicles[4];

    s32 lastCourse;
    s32 lastStage;
    u8 unk2[0x3BC];
};
size_assert(GlobalContext, 0x510);
