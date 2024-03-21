#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>

union RKNetBattleTeamData {

    RKNetBattleTeamData() { raw = 0; }
    struct { u8 battleType : 1; u32 teams : 31; };
    u32 raw;
};

union RKNetAidPidMap {

    RKNetAidPidMap() { raw[0] = 0xFFFFFFFF; raw[1] = 0xFFFFFFFF; raw[2] = 0xFFFFFFFF; }
    u8 playerIds[12]; // Index is player id, value is aid
    u32 raw[3];
};

union RKNetEngineClassData {

    // Custom values here, not game defaults!
    enum Value {
    	CLASS_BATTLE,
        CLASS_150CC,
        CLASS_200CC,
        CLASS_500CC,
    };

    RKNetEngineClassData() { raw = 0; }
    RKNetEngineClassData(u8 data) { raw = data; }

    int getEngineClass() {
        switch(engineClass) {
            case CLASS_150CC:
                return RaceConfig::Settings::CC_150;

            case CLASS_200CC:
                return RaceConfig::Settings::CC_200;

            case CLASS_500CC:
                return RaceConfig::Settings::CC_500;

            default:
                return RaceConfig::Settings::CC_50;
        }
    }

    void setEngineClass(int cls) {
        switch(cls) {
            case RaceConfig::Settings::CC_150:
                engineClass = CLASS_150CC;
                break;

            case RaceConfig::Settings::CC_200:
                engineClass = CLASS_200CC;
                break;

            case RaceConfig::Settings::CC_500:
                engineClass = CLASS_500CC;
                break;

            default:
                engineClass = CLASS_BATTLE;
        }
    }

    int getIsMirrorFlag() {
        return isMirror ? RaceConfig::Settings::FLAG_MIRROR : RaceConfig::Settings::FLAG_NONE;
    }

    void setIsMirrorFlag(int flags) {
        isMirror = (flags & RaceConfig::Settings::FLAG_MIRROR);
    }

    struct { bool isMirror : 1; u8 engineClass : 7; };
    u8 raw;
};
