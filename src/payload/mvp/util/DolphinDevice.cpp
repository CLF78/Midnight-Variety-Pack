#include <common/Common.hpp>
#include <mvp/util/DolphinDevice.hpp>
#include <platform/string.h>

namespace DolphinDevice {

s32 sDevDolphin = IOS_Open("/dev/dolphin", IPC_OPEN_NONE);
char sVersionBuffer[64];
char sProdCodeBuffer[6];

bool IsOpen() {
    return sDevDolphin >= 0;
}

u32 GetElapsedTime() {

    // Check if device is open
    if (!IsOpen())
        return 0;

    // Do IOS call
    u32 milliseconds = 0;
    IOSIoVector vec = {&milliseconds, sizeof(milliseconds)};
    s32 result = IOS_Ioctlv(sDevDolphin, GET_ELAPSED_TIME, 0, 1, &vec);

    // Bail on failure
    if (result != IPC_OK)
        return 0;

    // Return correct value
    return milliseconds;
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
    IOSIoVector vec = {sVersionBuffer, sizeof(sVersionBuffer)};
    s32 result = IOS_Ioctlv(sDevDolphin, GET_VERSION, 0, 1, &vec);

    // Bail on failure
    if (result != IPC_OK)
        return nullptr;

    // Ensure the string is null-terminated
    sVersionBuffer[strlenc(sVersionBuffer)] = '\0';

    // Return the buffer
    sVersionObtained = true;
    return sVersionBuffer;
}

const char* GetRealProductCode() {

    // If the code was already obtained, return it directly
    static bool sProdCodeObtained = false;
    if (sProdCodeObtained)
        return sProdCodeBuffer;

    // Check if device is open
    if (!IsOpen())
        return nullptr;

    // Do IOS call
    IOSIoVector vec = {sProdCodeBuffer, sizeof(sProdCodeBuffer)};
    s32 result = IOS_Ioctlv(sDevDolphin, GET_REAL_PRODUCT_CODE, 0, 1, &vec);

    // Bail on failure
    if (result != IPC_OK)
        return nullptr;

    // Ensure the string is null-terminated
    sProdCodeBuffer[strlenc(sProdCodeBuffer)] = '\0';

    // Return the buffer
    sProdCodeObtained = true;
    return sProdCodeBuffer;
}

} // namespace DolphinDevice
