#include "WifiDisconnectPage.hpp"
#include <game/net/RKNetController.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>

/////////////////////////////
// Improved Error Messages //
/////////////////////////////

// Inline helper
inline bool IsErrorInRange(u32 errorCode, u32 min, u32 max) {
    return errorCode >= min && errorCode < max;
}

// Customize the error message based on the error code
u32 GetErrorMessage(u32 errorCode, MessageInfo* extraInfo) {

    // 20XXX - Nintendo NASWII error codes
    if (IsErrorInRange(errorCode, 20000, 21000)) {
        switch(errorCode) {

            // Can't connect to the internet
            case 20100:
                return Message::Menu::ERROR_NO_CONNECTION;

            // WFC shutdown message, can only appear if domain patches were applied incorrectly
            case 20110:
                return Message::Menu::ERROR_INCORRECT_WIFI_PATCHES;

            // Let other errors fall back to a generic message
            default:
                return Message::Menu::ERROR_CONNECTION_GENERIC;
        }

    // 23XXX - Wiimmfi NASWII error codes
    } else if (IsErrorInRange(errorCode, 23000, 24000)) {
        switch(errorCode) {

            // 1 hour before activation
            case 23801:
                return Message::Menu::ERROR_ACTIVATION_1_HOUR;

            // XX hours before activation
            case 23802 ... 23879:
                extraInfo->intVals[1] = errorCode - 23800;
                return Message::Menu::ERROR_ACTIVATION_X_HOURS;

            // 1 day before activation
            case 23881:
                return Message::Menu::ERROR_ACTIVATION_1_DAY;

            // X days before activation
            case 23882 ... 23887:
                extraInfo->intVals[1] = errorCode - 23880;
                return Message::Menu::ERROR_ACTIVATION_X_DAYS;

            // Console registered but needs activation
            case 23899:
                return Message::Menu::ERROR_CONSOLE_REGISTERED;

            // Server under maintenance
            case 23901:
                return Message::Menu::ERROR_MAINTENANCE;

            // Server under heavy traffic
            case 23902:
                return Message::Menu::ERROR_HIGH_TRAFFIC;

            // Profile creation disabled
            case 23903:
                return Message::Menu::ERROR_PROFILE_CREATION_DISABLED;

            // Outdated patches (may happen in the future)
            case 23904 ... 23905:
            case 23951 ... 23953:
                return Message::Menu::ERROR_OUTDATED_WIFI_PATCHES;

            // Banned profile (imported savegame most likely)
            case 23910:
            case 23912:
                return Message::Menu::ERROR_PROFILE_BANNED;

            // Profile creation denied
            case 23911:
                return Message::Menu::ERROR_PROFILE_CREATION_DENIED;

            // Console creation denied
            case 23913:
                return Message::Menu::ERROR_CONSOLE_CREATION_DENIED;

            // Console banned
            case 23914:
                return Message::Menu::ERROR_CONSOLE_BANNED;

            // NAND not imported
            case 23915:
            case 23918:
                return Message::Menu::ERROR_DEFAULT_NAND;

            // Proxies not allowed
            case 23916:
                return Message::Menu::ERROR_PROXY;

            // User banned
            case 23917:
                return Message::Menu::ERROR_BANNED;

            // Profile not created on Wiimmfi
            case 23919:
                return Message::Menu::ERROR_INVALID_PROFILE;

            // Offensive or invalid Mii
            case 23927 ... 23928:
                return Message::Menu::ERROR_OFFENSIVE_MII;

            // Various patching errors
            case 23931 ... 23935:
            case 23971 ... 23973:
                return Message::Menu::ERROR_INCORRECT_WIFI_PATCHES;

            // Let other errors fall back to a generic message
            default:
                return Message::Menu::ERROR_CONNECTION_GENERIC;
        }

    // 29XXX - NAND Errors
    } else if (IsErrorInRange(errorCode, 29000, 30000)) {
        switch(errorCode) {

            // NAND full
            case 29000:
                return Message::Menu::ERROR_CONNECTION_NOT_ENOUGH_MEMORY;

            // NAND damaged
            case 29001:
                return Message::Menu::ERROR_CONNECTION_NAND_DAMAGED;

            // Let other errors fall back to a generic message
            default:
                return Message::Menu::ERROR_CONNECTION_NAND_ACCESS;
        }

    // 31XXX - DLS1 Errors
    } else if (IsErrorInRange(errorCode, 31000, 32000))
        return Message::Menu::ERROR_DOWNLOAD;

    // 33XXX - Profanity Check Errors
    else if (IsErrorInRange(errorCode, 33000, 34000))
        return Message::Menu::ERROR_MII_NAME_CHECK;

    // 5XXXX - Network Connection Errors
    else if (IsErrorInRange(errorCode, 50000, 60000)) {
        switch (errorCode) {

            // No internet connection configured
            case 50200 ... 50299:
                return Message::Menu::ERROR_NO_CONNECTION_CONFIGURED;

            // Cannot connect to LAN
            case 50400 ... 50499:
            case 51400 ... 51499:
                return Message::Menu::ERROR_UNSTABLE_LAN_ADAPTER;

            // Cannot connect to access point
            case 51000 ... 51099:
            case 51300 ... 51399:
                return Message::Menu::ERROR_AP_CONFIG;

            // DHCP error or IP address conflict
            case 52000 ... 52099:
            case 52700 ... 52799:
                return Message::Menu::ERROR_IP_SETTINGS;

            // DNS error
            case 52100 ... 52199:
                return Message::Menu::ERROR_DNS_SETTINGS;

            // Proxy configuration error
            case 52400 ... 52599:
                return Message::Menu::ERROR_PROXY_SETTINGS;

            // Unstable connection
            case 54000 ... 54099:
                return Message::Menu::ERROR_UNSTABLE_CONNECTION;

            // Fall back to a generic error message
            default:
                return Message::Menu::ERROR_NO_CONNECTION;
        }

    // 6XXXX - Network Errors (Login State)
    } else if (IsErrorInRange(errorCode, 60000, 70000)) {
        switch (errorCode) {

            // Error 60000 cannot appear unless the patch was removed
            case 60000:
                return Message::Menu::ERROR_INCORRECT_WIFI_PATCHES;

            // Fall back to a generic error message
            default:
                return Message::Menu::ERROR_DURING_LOGIN;
        }

    // 7XXXX - Network Errors (Friend Roster Synchronization State)
    } else if (IsErrorInRange(errorCode, 70000, 80000)) {
        switch (errorCode) {

            // Possible kick or ban
            case 71010:
                return Message::Menu::ERROR_KICK_BAN;

            // Fall back to a generic error message
            default:
                return Message::Menu::ERROR_DURING_FRIEND_PROCESS;
        }

    // 8XXXX - Network Errors (Matchmaking State)
    } else if (IsErrorInRange(errorCode, 80000, 90000)) {
        switch (errorCode) {

            // Possible kick or ban
            case 81010:
                return Message::Menu::ERROR_KICK_BAN;

            // Bad connection
            case 86420:
                return Message::Menu::ERROR_86420;

            // Fall back to a generic error message
            default:
                return Message::Menu::ERROR_DURING_MATCHMAKING;
        }

    // 9XXXX - Network Errors (Other Stage)
    } else if (IsErrorInRange(errorCode, 90000, 100000)) {
        switch (errorCode) {

            // Possible kick or ban
            case 91010:
                return Message::Menu::ERROR_KICK_BAN;

            // Fall back to a generic error message
            default:
                return Message::Menu::ERROR_DURING_GAMEPLAY;
        }

    // 1XXXXX - WiiConnect24 Errors
    } else if (errorCode >= 100000)
        return Message::Menu::ERROR_COMMUNICATION;

    // Fallback to all the above
    return Message::Menu::ERROR_CONNECTION_UNKNOWN;
}

// Display the error code in more cases and make the error messages more descriptive
REPLACE void WifiDisconnectPage::onActivate() {

    // Display the OK button by default
    okButton.hidden = false;

    // Get the section
    Section* section = SectionManager::instance->curSection;

    // If the disconnection was triggered by the game, there are no error codes to display
    if (section->sectionId == Section::DC_WITHOUT_ERROR_CODE) {
        section->shutdownNet();
        messageBox.setText(Message::Menu::ERROR_DISCONNECTED_BY_GAME);
        return;
    }

    // Get the disconnection info
    if (section->sectionId == Section::DC_WITH_ERROR_CODE || section->sectionId == Section::UNK_98) {
        disconnectInfo = SectionManager::instance->globalContext->disconnectInfo;
        SectionManager::instance->globalContext->disconnectInfo.reset();
    } else {
        disconnectInfo = RKNetController::instance->getWifiDisconnectInfo();
    }

    // Setup message info with category and error code (not used by every message)
    const u32 disconnectCategory = disconnectInfo.errorCategory;
    MessageInfo msgInfo;
    msgInfo.intVals[0] = disconnectInfo.errorCode;

    // Various errors, display a different message depending on the error code
    if (disconnectCategory == WifiDisconnectInfo::ERROR_WITH_CODE) {
        const u32 errorMsg = GetErrorMessage(disconnectInfo.errorCode, &msgInfo);
        messageBox.setText(errorMsg, &msgInfo);
        section->shutdownNet();

    // Mii name detected as offensive, use the dedicated message (no error codes available)
    } else if (disconnectCategory == WifiDisconnectInfo::ERROR_OFFENSIVE_MII) {
        messageBox.setText(Message::Menu::ERROR_OFFENSIVE_MII);
        section->shutdownNet();

    // Generic error, unsure of use cases
    } else if (disconnectCategory == WifiDisconnectInfo::ERROR_GENERIC) {
        messageBox.setText(Message::Menu::ERROR_DISCONNECT_GENERIC);
        section->shutdownNet();

    // There was a fatal error, prevent returning to the main menu and display the error code
    // TODO allow restarting the game instead of disabling the button?
    } else if (disconnectCategory == WifiDisconnectInfo::ERROR_UNRECOVERABLE) {
        messageBox.setText(Message::Menu::ERROR_UNRECOVERABLE, &msgInfo);
        okButton.hidden = true;
        inputManager.playerEnabledFlags = 0;
    }
}
