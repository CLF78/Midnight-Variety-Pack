#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL DWC_IsServerMyself();
int DWC_CloseAllConnectionsHard();
int DWC_CloseConnectionHard(u8 aid);

#ifdef __cplusplus
}
#endif
