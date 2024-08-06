#pragma once
#include <game/system/RaceConfig.hpp>

enum RKNetPacketSection {
    RKNET_SECTION_HEADER,
    RKNET_SECTION_RACEHEADER_1,
    RKNET_SECTION_RACEHEADER_2,
    RKNET_SECTION_ROOM_SELECT,
    RKNET_SECTION_RACEDATA,
    RKNET_SECTION_USER,
    RKNET_SECTION_ITEM,
    RKNET_SECTION_EVENT,
    RKNET_SECTION_COUNT,
};

union RKNetBattleTeamData {

    RKNetBattleTeamData() : raw(0) {}
    struct { u8 battleType : 1; u32 teams : 31; };
    u32 raw;
};
size_assert(RKNetBattleTeamData, 0x4);

union RKNetAidPidMap {

    RKNetAidPidMap() { raw[0] = 0xFFFFFFFF; raw[1] = 0xFFFFFFFF; raw[2] = 0xFFFFFFFF; }

    bool operator==(const RKNetAidPidMap& map) const {
        return raw[0] == map.raw[0] && raw[1] == map.raw[1] && raw[2] == map.raw[2];
    }

    u8 playerIds[12]; // Index is player id, value is aid
    u32 raw[3];
};
size_assert(RKNetAidPidMap, 0xC);

union RKNetEngineClassData {

    enum Value {
    	CLASS_BATTLE,
        CLASS_100CC,
        CLASS_150CC,
        CLASS_150CC_MIRROR,

        // Custom values from here onwards
        CLASS_200CC,
        CLASS_500CC,
    };

    RKNetEngineClassData() : raw(0) {}
    explicit RKNetEngineClassData(u8 data) : raw(data) {}

    int getEngineClass() {
        switch(engineClass) {
            case CLASS_100CC:
                return RaceConfig::Settings::CC_100;

            case CLASS_150CC:
            case CLASS_150CC_MIRROR:
                return RaceConfig::Settings::CC_150;

            case CLASS_200CC:
                return RaceConfig::Settings::CC_200;

            case CLASS_500CC:
                return RaceConfig::Settings::CC_500;

            case CLASS_BATTLE:
            default:
                return RaceConfig::Settings::CC_50;
        }
    }

    void setEngineClass(int cls) {

        switch(cls) {
            case RaceConfig::Settings::CC_100:
                engineClass = CLASS_100CC;
                break;

            case RaceConfig::Settings::CC_150:
                engineClass = CLASS_150CC;
                break;

            case RaceConfig::Settings::CC_200:
                engineClass = CLASS_200CC;
                break;

            case RaceConfig::Settings::CC_500:
                engineClass = CLASS_500CC;
                break;

            case RaceConfig::Settings::CC_50:
            default:
                engineClass = CLASS_BATTLE;
        }
    }

    int getIsMirrorFlag() {
        if (engineClass == CLASS_150CC_MIRROR) return RaceConfig::Settings::FLAG_MIRROR;
        return isMirror ? RaceConfig::Settings::FLAG_MIRROR : RaceConfig::Settings::FLAG_NONE;
    }

    void setIsMirrorFlag(int flags) {
        isMirror = (flags & RaceConfig::Settings::FLAG_MIRROR) != 0;
    }

    struct { bool isMirror : 1; u8 engineClass : 7; };
    u8 raw;
};
size_assert(RKNetEngineClassData, 0x1);
