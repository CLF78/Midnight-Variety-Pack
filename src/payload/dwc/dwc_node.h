#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 playersAtConsole;
    PAD(3);
} DWCConnectionUserData;
size_cassert(DWCConnectionUserData, 0x4);

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
    PAD(4);

    s64 nextMeshMakeTryTick;
    DWCConnectionUserData connectionUserData;
    PAD(4);
} DWCNodeInfo;
size_cassert(DWCNodeInfo, 0x30);

typedef struct {
    u32 nodeCount;
    PAD(4);
    DWCNodeInfo nodeInfos[32];
} DWCNodeInfoList;
size_cassert(DWCNodeInfoList, 0x608);

DWCNodeInfo* DWCi_NodeInfoList_GetServerNodeInfo();
DWCNodeInfo* DWCi_NodeInfoList_GetNodeInfoForProfileId(int pid);
DWCNodeInfo* DWCi_NodeInfoList_GetNodeInfoForAid(u8 aid);

#ifdef __cplusplus
}
#endif
