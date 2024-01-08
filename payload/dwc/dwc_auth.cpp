#include <common/Common.hpp>
#include <midnight/DolphinDevice.hpp>
#include <revolution/sc.h>

////////////////////////////
// Wiimmfi Authentication //
////////////////////////////

// Report the product code from the imported NAND on Dolphin
kmCallDefCpp(0x800EE098, const char*) {

    // Check if we're on Dolphin and use its IOCTL if so
    if (DolphinDevice::Open())
        return DolphinDevice::GetRealProductCode();

    // Else just call the original function
    return SCGetProductCode();
}
