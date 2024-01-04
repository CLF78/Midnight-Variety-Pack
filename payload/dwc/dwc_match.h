#include <common/Common.h>
#include <dwc/dwc_account.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_node.h>
#include <gs/gp/gpi.h>
#include <gs/gt2/gt2Callback.h>
#include <gs/gt2/gt2Main.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DWCMatchedCallback)(DWCError error, int cancel, int self, int isServer, int index, void* param);
typedef void (*DWCNewClientCallback)(int friendIndex, void* param);
typedef int (*DWCEvalCallback)(int serverBrowserCount, void* param);
typedef int (*DWCConnectAttemptCallback)(u8* userData, void* param);

typedef enum {
    DWC_MATCH_STATE_INIT,
    DWC_MATCH_STATE_CL_WAITING,
    DWC_MATCH_STATE_CL_SEARCH_HOST,
    DWC_MATCH_STATE_CL_WAIT_RESV,
    DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST,
    DWC_MATCH_STATE_CL_NN,
    DWC_MATCH_STATE_CL_GT2,
    DWC_MATCH_STATE_CL_CANCEL_SYN,
    DWC_MATCH_STATE_CL_SYN,
    DWC_MATCH_STATE_CL_SVDOWN_1,
    DWC_MATCH_STATE_CL_SVDOWN_2,
    DWC_MATCH_STATE_CL_SVDOWN_3,
    DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST,
    DWC_MATCH_STATE_SV_WAITING,
    DWC_MATCH_STATE_SV_OWN_NN,
    DWC_MATCH_STATE_SV_OWN_GT2,
    DWC_MATCH_STATE_SV_WAIT_CL_LINK,
    DWC_MATCH_STATE_SV_CANCEL_SYN,
    DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT,
    DWC_MATCH_STATE_SV_SYN,
    DWC_MATCH_STATE_SV_SYN_WAIT,
    DWC_MATCH_STATE_WAIT_CLOSE,
    DWC_MATCH_STATE_SEARCH_OWN,
    DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT,
    DWC_MATCH_STATE_NUM,
} DWCMatchState;

typedef struct {
    u8 command;
    u8 count;
    u16 port;
    u32 ip;
    u32 data[32];
    int profileId;
    int len;
    s64 sendTime;
} DWCMatchCommandControl;

typedef struct {
    u8 isQr2;
    u8 retryCount;
    u16 port;
    u32 ip;
    int cookie;
} DWCNNInfo;

typedef struct {
    GPConnection gpConnection;
    GT2Socket gt2Socket;
    GT2Callbacks* gt2Callbacks;
    u8 gt2ConnectCount;
    u8 connectedOnceBefore;
    // 2 bytes padding

    void* qr2; // TODO header
    u8 qr2MatchType;
    u8 qr2MatchTypeExt;
    u8 hostState;
    u8 qr2NumEntry;
    u8 qr2IsReserved;
    u8 pad;
    // 2 bytes padding

    u32 suspendMatchBitmap;
    s64 suspendRequestTick;
    int suspendMatch;
    u8 suspend;
    u8 suspendServerResponse;
    u8 pad2;
    // 1 byte padding

    DWCNodeInfoList nodeInfoList;
    DWCNodeInfo tempNewNodeInfo;
    DWCNodeInfo tempNewClientInfo;
    int inMeshMaking;
    DWCMatchState lastStateBeforeStopMeshMaking;
    DWCNodeInfo tempNodeInfoBeforeStopMeshMaking;

    u16 qr2Port;
    // 2 bytes padding
    u32 qr2IP;
    int qr2Reservation;

    void* serverBrowser; // TODO header
    int serverBrowserUpdateFlag;
    // 4 bytes padding
    s64 serverBrowserUpdateTick;
    s64 serverBrowserUpdateRequestTick;

    u8 natnegRecvCount;
    u8 natnegFailureCount;
    u16 natnegCookieRand;
    int natnegLastCookie;
    s64 natnegFailedTime;
    s64 natnegFinishTime;
    DWCNNInfo natnegInfo;

    DWCMatchState state;
    u8 resvWaitCount;
    u8 closeState;
    u8 cancelState;
    u8 scResvRetryCount;
    u8 synResendCount;
    u8 cancelSynResendCount;
    u8 clWaitTimeoutCount;
    u8 _727[3]; // padding

    u16 baseLatency;
    u16 cancelBaseLatency;
    // 2 bytes padding
    u32 natnegResvCheckValue;

    int cmdResendFlag;
    s64 cmdResendTick;
    u32 cmdTimeoutTime;
    // 4 bytes padding
    s64 cmdTimeoutStartTick;

    u32 synAckBit;
    u32 cancelSynAckBit;
    s64 lastSynSent;
    s64 lastCancelSynSent;
    s64 closedTime;
    s64 clWaitTime;

    int profileId;
    u32 reqProfileId;
    int cbEventPid;

    const char* gameName;
    const char* secretKey;

    const DWCFriendData* friendList;
    int friendListLength;
    u8 friendIdxList[64];
    int friendIdxListLength;

    DWCMatchCommandControl cmdCnt;
    DWCMatchedCallback matchedCallback;
    void* matchedCallbackParam;
    DWCNewClientCallback newClientCallback;
    void* newClientCallbackParam;
    DWCEvalCallback evalCallback;
    void* evalCallbackParam;

    u32 serverDownBuff;
    u32 serverDownBase;
    u32 groupId;
    u8 searchGroupIdRetryHostCount;
    u8 connectGroupId;
    u8 serverDownQueryCount;
    u8 serverDownNakCount;
    s64 serverPollingTime;

    u8 _8A0;
    u8 _8A1;
    // 2 bytes padding

    DWCConnectAttemptCallback connectAttemptCallback;
    u8 connUserData[4];
    void* connectAttemptParam;

    u32 _8B0;
    // 4 bytes padding
    s64 suspendWaitTime;
} DWCMatchControl;

extern DWCMatchControl* stpMatchCnt;

#ifdef __cplusplus
}
#endif
