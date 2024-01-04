#include <common/Common.hpp>
#include <wiimmfi/Reporting.hpp>

// Hook gpiSendLogin to send the signature and certificate back to the server (ported from Wiimmfi payload)
kmBranchDefCpp(0x801007D8, NULL, int, int ret) {
    Wiimmfi::Reporting::ReportSignatureAndCert();
    return ret;
}
