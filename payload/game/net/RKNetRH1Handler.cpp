#include <common/Common.hpp>
#include <game/net/RKNetRH1Handler.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupCounts.h>

///////////////////////
// Custom Cup System //
///////////////////////

// RKNetRH1Handler::getTrackId() override
// Replace the course check to get the track id from the correct player
kmBranchDefCpp(0x80664560, NULL, u32, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].timer != 0 && self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].trackId;
    }

    return -1;
}

// RKNetRH1Handler::getBattleType() override
// Replace the course check to get the battle type from the correct player
kmBranchDefCpp(0x806646C8, NULL, u8, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].battleTeamData.battleType;
    }

    return RaceConfig::Settings::BATTLE_INVALID;
}

// RKNetRH1Handler::getRandomSeed() override
// Replace the course check to get the random seed from the correct player
kmBranchDefCpp(0x80664944, NULL, u32, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].randomSeed;
    }

    return 0;
}

// RKNetRH1Handler::getEngineClass() override
// Replace the course check to get the engine class from the correct player
kmBranchDefCpp(0x80664A3C, NULL, u8, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].engineClass;
    }

    return RaceConfig::Settings::CC_50;
}

// RKNetRH1Handler::getAidPidMap() override
// Replace the course check to get the map from the correct player
kmBranchDefCpp(0x80664B34, NULL, u8*, RKNetRH1Handler* self) {

    for (int i = 0; i < ARRAY_SIZE(self->datas); i++) {
        if (self->datas[i].trackId < TRACK_COUNT)
            return self->datas[i].aidPidMap.playerIds;
    }

    return nullptr;
}
