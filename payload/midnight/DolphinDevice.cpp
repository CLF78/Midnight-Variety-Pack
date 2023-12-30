#include <common/Common.hpp>
#include <midnight/DolphinDevice.hpp>

namespace DolphinDevice {

s32 sDevDolphin = -1;
ALIGN(32) char sVersionBuffer[64]; // Must be aligned by 32 and multiple of 32 bytes 

bool IsOpen() {
    return sDevDolphin >= 0;
}

bool Open() {
    if (sDevDolphin == -1)
        sDevDolphin = IOS_Open("/dev/dolphin", 0);
    return IsOpen();
}

void Close() {
    if (IsOpen()) {
        IOS_Close(sDevDolphin);
        sDevDolphin = -1;
    }
}

const char* GetVersion() {

    // If the version was already obtained, return it directly
    static bool sVersionObtained = false;
    if (sVersionObtained)
        return sVersionBuffer;

    // Check if device is open
    if (!IsOpen())
        return nullptr;

    // Do IOS call
    ALIGN(32) IOSIoVector vec = {sVersionBuffer, sizeof(sVersionBuffer)};
    s32 result = IOS_Ioctlv(sDevDolphin, GET_VERSION, 0, 1, &vec);

    // Bail on failure
    if (result != IPC_OK)
        return nullptr;

    // Ensure the string is null-terminated
    sVersionBuffer[sizeof(sVersionBuffer)-1] = '\0';

    // Return the buffer
    sVersionObtained = true;
    return sVersionBuffer;
}

} // namespace DolphinDevice
