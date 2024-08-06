#pragma once
#include "dwc_match.h"
#include <gs/gt2/gt2Main.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GT2Socket gt2Socket;
    GT2Callbacks gt2Callbacks;
    int gt2SendBufSize;
    int gt2RecvBufSize;
    UNK(0x88 - 0x1C);

    DWCMatchedCallback userMatchedCallback;
    void* userMatchedCallbackParam;
    UNK(0x360 - 0x90);

    DWCMatchControl matchControl;
    UNK(0x1438 - 0xC20);
} DWCControl; // TODO finish header
size_cassert(DWCControl, 0x1438);

typedef struct {
    u8 index;
    u8 aid;
    PAD(2);
    u32 profileId;
} DWCConnectionInfo;
size_cassert(DWCConnectionInfo, 0x8);

BOOL DWC_IsServerMyself();
int DWC_CloseAllConnectionsHard();
int DWC_CloseConnectionHard(u8 aid);

int DWCi_GT2GetConnectionListIdx();
GT2Connection* DWCi_GetGT2ConnectionByIdx(int idx);
GT2Connection DWCi_GetGT2Connection(u8 aid);
DWCConnectionInfo* DWCi_GetConnectionInfoByIdx(int idx);

void DWCi_GT2SocketErrorCallback(GT2Socket socket);
BOOL DWCi_GT2UnrecognizedMessageCallback(GT2Socket socket, u32 ip, u16 port, u8* message, int len);
GT2Result DWCi_HandleGT2Error(GT2Result result);

u8 DWC_GetMyAID();
u8 DWC_GetServerAID();
u32 DWC_GetAIDBitmap();

extern DWCControl* stpDwcCnt;

#ifdef __cplusplus
}
#endif
