#include <common/Common.hpp>
#include <dwc/dwc_node.h>
#include <egg/core/eggExpHeap.hpp>
#include <egg/core/eggTaskThread.hpp>
#include <game/net/RKNetFriend.hpp>
#include <game/net/RKNetMutex.hpp>
#include <game/net/RKNetPacketHolder.hpp>
#include <game/net/RKNetStatusData.hpp>

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
        VS_WW = 1,
        VS_REGIONAL,
        BT_WW,
        BT_REGIONAL,
    };

    Sub* getCurrentSub() {
        return &subs[currentSub];
    }

    bool isPlayerHost() {
        Sub* sub = getCurrentSub();
        return sub->myAid == sub->hostAid;
    }

    virtual ~RKNetController();

    void processRacePacket(u32 aid, void* data, u32 dataLength);

    RKNetMutex mutex;
    EGG::ExpHeap* heap;
    EGG::TaskThread* taskThread;

    ConnectionState connState;
    int _2C; // Error code type i think
    int _30; // Error code i think
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

    // TODO replace this with a pointer to a buffer for custom size
    u8 fullRecvRACEPackets[12][736]; // 1 per aid
    // 3 bytes padding

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
    int lastRecvBufferUsed[12][8]; // 1 per packet section per aid
    int currentSub;
    u8 aidPidMap[12]; // Index is player id, value is aid
    u32 disconnectedAids; // Bitfield
    u32 disconnectedPlayerIds; // Bitfield

    // Used for matchmaking
    u16 maxVrGaps[10];
    u16 maxBrGaps[10];
    // 4 bytes padding

    s64 _2960[12]; // More timers
    u32 _29C0; // Another timer
    // 4 bytes padding

    static RKNetController* instance;
};
size_assert(RKNetController, 0x29C8);
