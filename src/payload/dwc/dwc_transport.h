#pragma once
#include <common/Common.h>

#ifdef __cplusplus
extern "C" {
#endif

void DWCi_ResetLastSomeDataRecvTimeByAid(u8 aid);
BOOL DWC_SetRecvBuffer(u8 aid, void* buffer, int size);

#ifdef __cplusplus
}
#endif
