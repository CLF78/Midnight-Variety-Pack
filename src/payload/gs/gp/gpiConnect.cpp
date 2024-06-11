#include <common/Common.hpp>
#include <gs/gp/gp.h>
#include <gs/gp/gpiOperation.h>
#include <wiimmfi/Reporting.hpp>

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Report the signature and certificate to the server
// Credits: Wiimmfi
REPLACE GPResult gpiSendLogin(GPConnection connection, GPIConnectData* data) {
    GPResult ret = REPLACED(connection, data);
    Wiimmfi::Reporting::ReportSignatureAndCert();
    return ret;
}
