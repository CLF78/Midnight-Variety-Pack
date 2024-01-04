#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <platform/string.h>
#include <revolution/es/es.h>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Reporting {

void ReportSignatureAndCert() {

    ALIGN(32) char signature[60]; // system-defined
    ALIGN(32) char b64Signature[80]; // pre-computed length
    ALIGN(32) char cert[0x180]; // system-defined
    int tokenLength = Status::token ? strlen(Status::token) : 0;

    // Get the certificate
    s32 ret = ES_Sign((u8*)Status::token, tokenLength, (u8*)signature, (u8*)cert);
    if (ret == ES_ERR_OK) {

        // Encode and send the signature
        int len = DWC_Base64Encode(signature, sizeof(signature), b64Signature, sizeof(b64Signature));
        b64Signature[len] = '\0';
        Status::SendMessage("xy_sg", b64Signature, ret);

        // Encode and send the certificate
        len = DWC_Base64Encode(cert, sizeof(cert), Auth::sConsoleCert, sizeof(Auth::sConsoleCert)-1);
        Auth::sConsoleCert[len] = '\0';
        Status::SendMessage("xy_ct", Auth::sConsoleCert, ret);

    } else {

        // Something failed, send invalid data
        Status::SendMessage("xy_sg", "invalid", ret);
        Status::SendMessage("xy_ct", "invalid", ret);
    }
}

} // namespace Reporting
} // namespace Wiimmfi
