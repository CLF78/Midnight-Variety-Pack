#include <common/Common.hpp>

class RKNetRH1Handler {
public:

    struct Data {
        u32 timer;
        int vehicles[2];
        int characters[2];
        int trackId;
        u32 team;
        u32 randomSeed;
        s8 aidPidMap[12];
        u8 engineClass;
        u8 starRanks[2];
        // 1 byte padding
    };

    bool prepared;
    bool _1;
    // 2 bytes padding

    u32 availableAids;
    u32 aidsWithUpdatedRH1;
    u32 aidsWithField17At1;
    u8 lastSendAid;
    // 7 bytes padding

    s64 lastSendTime;
    Data datas[12];

    static RKNetRH1Handler* instance;
};
size_assert(RKNetRH1Handler, 0x260);
