#pragma once
#include "dwc_account.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DWC_STATUS_OFFLINE,
    DWC_STATUS_ONLINE,
    DWC_STATUS_PLAYING,
    DWC_STATUS_MATCH_ANYBODY,
    DWC_STATUS_MATCH_FRIEND,
    DWC_STATUS_MATCH_SC_CL,
    DWC_STATUS_MATCH_SC_SV,
    DWC_STATUS_NUM,
} DWCFriendStatus;

typedef void (*DWCBuddyFriendCallback)(int index, void* param);
BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param);
DWCFriendStatus DWC_GetFriendStatusSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry,
                                      char* statusString);

void DWCi_FriendProcess();

#ifdef __cplusplus
}
#endif
