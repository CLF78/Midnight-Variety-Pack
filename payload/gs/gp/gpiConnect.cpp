#include <common/Common.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Hook gpiSendLogin to send the signature and certificate back to the server
kmBranchDefCpp(0x801007D8, NULL, int, int ret) {
    Wiimmfi::Reporting::ReportSignatureAndCert();
    return ret;
}
