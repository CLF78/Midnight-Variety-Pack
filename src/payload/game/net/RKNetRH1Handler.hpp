#pragma once
#include "packet/RKNetPacketCommon.hpp"

class RKNetRH1Handler {
public:
    struct Data {
        u32 timer;
        int vehicles[2];
        int characters[2];
        int trackId;
        RKNetBattleTeamData battleTeamData;
        u32 randomSeed;
        RKNetAidPidMap aidPidMap;
        RKNetEngineClassData engineClass;
        u8 starRanks[2];
        PAD(1);
    };
    size_assert(Data, 0x30);

    u8* getAidPidMap();
    u8 getBattleType();
    u8 getEngineClass();
    u32 getRandomSeed();
    u32 getTrackId();
    bool isTrackAvailable();

    void importRecvData();
    void exportDefaultPacket(u8 aid);
    void exportDefaultSpectatorPacket(u8 aid);

    bool prepared;
    bool _1;
    PAD(2);

    u32 availableAids;
    u32 aidsWithUpdatedRH1;
    u32 aidsWithField17At1;
    u8 lastSendAid;
    PAD(7);

    s64 lastSendTime;
    Data datas[12];

    static RKNetRH1Handler* instance;
};
size_assert(RKNetRH1Handler, 0x260);
