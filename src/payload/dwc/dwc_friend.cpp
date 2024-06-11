#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <dwc/dwc_friend.h>
#include <game/net/RKNetStatusData.hpp>
#include <platform/string.h>
#include <wiimmfi/Natify.hpp>

///////////////////////////////////////
// Friend Status Buffer Overflow Fix //
///////////////////////////////////////

// Enforce the status data buffer size to prevent an overflow
// Credits: Wiimmfi
REPLACE DWCFriendStatus DWC_GetFriendStatusData(DWCFriendData* friendData, char* statusData, int* size) {

    // Get the communication status
    char statusString[256];
    DWCFriendStatus ret = DWC_GetFriendStatusSC(friendData, nullptr, nullptr, statusString);
    if (ret == DWC_STATUS_OFFLINE) {
        *size = -1;
        return ret;
    }

    // Get decoded size
    size_t statusLen = strlen(statusString);
    int decodedLen = DWC_Base64Decode(statusString, statusLen, nullptr, 0);
    *size = decodedLen;

    // Try decoding
    if (statusData && decodedLen != -1)
        DWC_Base64Decode(statusString, statusLen, statusData, sizeof(RKNetStatusData[2]));

    return ret;
}

/////////////////////////
// NATify Improvements //
/////////////////////////

// Update custom NATify procedure
// Credits: Wiimmfi
REPLACE void DWCi_FriendProcess() {
    REPLACED();
    Wiimmfi::Natify::Update();
}
