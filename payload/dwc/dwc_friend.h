#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*DWCBuddyFriendCallback)(int index, void* param);
BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param);

#ifdef __cplusplus
}
#endif
