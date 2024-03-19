#include <common/Common.hpp>

union RKNetBattleTeamData {
    struct { u8 battleType : 1; u32 teams : 31; };
    u32 raw;
};

union RKNetAidPidMap {
    u8 playerIds[12];
    u32 raw[3];
};
