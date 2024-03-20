#include <common/Common.hpp>

union RKNetBattleTeamData {

    RKNetBattleTeamData() { raw = 0; }
    struct { u8 battleType : 1; u32 teams : 31; };
    u32 raw;
};

union RKNetAidPidMap {

    RKNetAidPidMap() { raw[0] = 0xFFFFFFFF; raw[1] = 0xFFFFFFFF; raw[2] = 0xFFFFFFFF; }
    u8 playerIds[12];
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
    struct { bool isMirror : 1; u8 engineClass : 7; };
    u8 raw;
};
