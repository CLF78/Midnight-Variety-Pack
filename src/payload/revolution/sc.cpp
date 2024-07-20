#include <common/Common.hpp>
#include <mvp/util/DolphinDevice.hpp>
#include <revolution/sc.h>

////////////////////////////
// Wiimmfi Authentication //
////////////////////////////

// Report the product code from the imported NAND on Dolphin
// Credits: Wiimmfi
REPLACE const char* SCGetProductCode() {

    // Check if we're on Dolphin and use the dedicated IOCTL if so
    if (DolphinDevice::IsOpen()) {
        if (const char* prodCode = DolphinDevice::GetRealProductCode())
            return prodCode;
    }

    // Else use the original function
    return REPLACED();
}
