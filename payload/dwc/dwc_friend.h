#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DWCFriendStatus {
    DWC_STATUS_OFFLINE,
    DWC_STATUS_ONLINE,
    DWC_STATUS_PLAYING,
    DWC_STATUS_MATCH_ANYBODY,
    DWC_STATUS_MATCH_FRIEND,
    DWC_STATUS_MATCH_SC_CL,
    DWC_STATUS_MATCH_SC_SV,
} DWCFriendStatus;

#ifdef __cplusplus
}
#endif
