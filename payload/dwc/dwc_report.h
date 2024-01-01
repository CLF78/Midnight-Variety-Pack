#include <common/Common.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    DWC_REPORT_NONE            = 0x00000000,
    DWC_REPORT_INFO            = 0x00000001,
    DWC_REPORT_ERROR           = 0x00000002,
    DWC_REPORT_DEBUG           = 0x00000004,
    DWC_REPORT_WARNING         = 0x00000008,
    DWC_REPORT_ACHECK          = 0x00000010,
    DWC_REPORT_LOGIN           = 0x00000020,
    DWC_REPORT_MATCH_NN        = 0x00000040,
    DWC_REPORT_MATCH_GT2       = 0x00000080,
    DWC_REPORT_TRANSPORT       = 0x00000100,
    DWC_REPORT_QR2_REQ         = 0x00000200,
    DWC_REPORT_SB_UPDATE       = 0x00000400,
    DWC_REPORT_SEND_INFO       = 0x00008000,
    DWC_REPORT_RECV_INFO       = 0x00010000,
    DWC_REPORT_UPDATE_SV       = 0x00020000,
    DWC_REPORT_CONNECTINET     = 0x00040000,
    DWC_REPORT_AUTH            = 0x01000000,
    DWC_REPORT_AC              = 0x02000000,
    DWC_REPORT_BM              = 0x04000000,
    DWC_REPORT_UTIL            = 0x08000000,
    DWC_REPORT_OPTION_CF       = 0x10000000,
    DWC_REPORT_OPTION_CONNTEST = 0x20000000,
    DWC_REPORT_GAMESPY         = 0x80000000,
    DWC_REPORT_TEST            = 0x00100000,
    DWC_REPORT_ALL             = 0xFFFFFFFF,
} DWCReportFlag;

void DWC_Printf(DWCReportFlag level, const char* fmt, ...);

#ifdef  __cplusplus
}
#endif
