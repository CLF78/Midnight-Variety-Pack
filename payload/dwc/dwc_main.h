#include <common/Common.h>
#include <dwc/dwc_match.h>
#include <gs/gt2/gt2Main.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 unk[0x88];
    DWCMatchedCallback userMatchedCallback;
    void* userMatchedCallbackParam;
    u8 unk2[0x360 - 0x90];
    DWCMatchControl matchControl;
} DWCControl; // TODO finish header

BOOL DWC_IsServerMyself();
int DWC_CloseAllConnectionsHard();
int DWC_CloseConnectionHard(u8 aid);

GT2Connection DWCi_GetGT2Connection(u8 aid);

extern DWCControl* stpDwcCnt;

#ifdef __cplusplus
}
#endif
