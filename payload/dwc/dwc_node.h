#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int profileId;
    u32 publicip;
    u32 localIp;
    u16 publicport;
    u16 localPort;
    u32 gt2Ip;
    u16 gt2Port;

    u8 aid;
    u8 hasPrivateAddress;
    int nnTryCount;
    // 4 bytes padding

    s64 nextMeshMakeTryTick;
    u8 connectionUserData[4];
    u32 pad;
} DWCNodeInfo;

typedef struct {
    int nodeCount;
    u32 pad;
    DWCNodeInfo nodeInfos[32];
} DWCNodeInfoList;

#ifdef __cplusplus
}
#endif
