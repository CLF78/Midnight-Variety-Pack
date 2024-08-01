#pragma once
#include "RKNetFriend.hpp"
#include "RKNetMutex.hpp"
#include "RKNetPacketHolder.hpp"
#include "RKNetStatusData.hpp"
#include "WifiDisconnectInfo.hpp"
#include "packet/RKNetPacketCommon.hpp"
#include <dwc/dwc_node.h>
#include <egg/core/eggExpHeap.hpp>
#include <egg/core/eggTaskThread.hpp>

class RKNetController {
public:

    struct Sub {
        s64 _0; // A timer
        u32 connectionCount;
        u32 playerCount;
        u32 availableAids; // Bitfield
        u32 directConnectedAids; // Bitfield
        u32 groupId;
        u32 friendToJoin;
        u8 localPlayerCount;
        u8 myAid;
        u8 hostAid;
        DWCConnectionUserData connectionUserDatas[12]; // 1 per aid
        bool matchingSuspended;
        // 4 bytes padding
    };

    enum ConnectionState {
        STATE_SHUTDOWN,
        STATE_UNK_1,
        STATE_LOGIN_START,
        STATE_LOGIN_AUTHORISED,
        STATE_LOGIN_FRIENDS_SYNCED,
        STATE_UNK_5,
        STATE_MATCHING,
        STATE_FAILED,
        STATE_ERROR,
    };

    enum SearchType {
        SEARCH_NONE,
        SEARCH_VS_WW,
        SEARCH_VS_REG,
        SEARCH_BT_WW,
        SEARCH_BT_REG,
    };

    Sub* getCurrentSub() {
        return &subs[currentSub];
    }

    RKNetPacketHolder* getPacketSendBuffer(u8 aid, u8 section) {
        return splitSendRACEPackets[lastSendBufferUsed[aid]][aid]->sections[section];
    }

    RKNetPacketHolder* getPacketRecvBuffer(u8 aid, u8 section) {
        return splitRecvRACEPackets[lastRecvBufferUsed[aid][section]][aid]->sections[section];
    }

    bool isPlayerHost() {
        Sub* sub = getCurrentSub();
        return sub->myAid == sub->hostAid;
    }

    bool isLocalPlayer(u8 aid) {
        Sub* sub = getCurrentSub();
        return aid == sub->myAid;
    }

    bool isConnectedPlayer(u8 aid) {
        Sub* sub = getCurrentSub();
        if (isLocalPlayer(aid)) return false;
        return (1 << aid) & sub->availableAids;
    }

    RKNetController(EGG::Heap* heap);
    virtual ~RKNetController();

    bool isConnectedToAnyone();
    void updateAidPidMap();

    void processRacePacket(u32 aid, void* data, u32 dataLength);

    WifiDisconnectInfo getWifiDisconnectInfo();
    u32 getLocalPlayerIdx(u32 localPlayerNum);

    RKNetMutex mutex;
    EGG::ExpHeap* heap;
    EGG::TaskThread* taskThread;

    ConnectionState connState;
    WifiDisconnectInfo disconnectInfo;
    // 4 bytes padding

    Sub subs[2];
    SearchType searchType;
    int _EC; // Something with suspend

    RKNetRACEPacketHolder* splitSendRACEPackets[2][12]; // Double buffered, 1 per aid
    RKNetRACEPacketHolder* splitRecvRACEPackets[2][12]; // Double buffered, 1 per aid
    RKNetPacketHolder* fullSendRACEPackets[12]; // 1 per aid

    s64 lastRACESendTimes[12]; // Time since a packet was last sent to each aid
    s64 lastRACERecvTimes[12]; // Time since a packet was last received from each aid
    s64 RACESendTimesTaken[12]; // Time between the last two packets sent to each aid
    s64 RACERecvTimesTaken[12]; // Time between the last two packets received from each aid
    u8 lastRACESendAid; // The last aid a packet was sent to

    // Modified structure
    // 3 bytes padding
    u8* fullRecvRACEPackets[12];
    u8 unused[0x25E4 - 0x394];

    RKNetStatusData ownStatus;
    RKNetFriend friendStatus[30];
    bool friendsListChanged;
    bool shutdownScheduled;
    bool friendStatusUpdateScheduled;
    bool _2757; // Something with disc errors

    bool profanityCheckFailed;
    // 3 bytes padding
    int badWordsNum;

    // Used for matchmaking
    // TODO expand this so we can use multiple VR/BR values as we need
    int _2760; // Pulled from savegame, looks like some sort of winrate?
    int vr;
    int br;

    int lastSendBufferUsed[12]; // 1 per aid
    int lastRecvBufferUsed[12][RKNET_SECTION_COUNT]; // 1 per packet section per aid
    int currentSub;
    RKNetAidPidMap aidPidMap;
    u32 disconnectedAids; // Bitfield
    u32 disconnectedPlayerIds; // Bitfield

    // Used for matchmaking
    u16 maxVrGaps[10];
    u16 maxBrGaps[10];
    // 4 bytes padding

    s64 countdownTimers[12]; // Used in RACEHEADER_1
    u32 _29C0; // Another timer
    // 4 bytes padding

    static RKNetController* instance;
    static u32 packetBufferSizes[RKNET_SECTION_COUNT];
};
size_assert(RKNetController, 0x29C8);
