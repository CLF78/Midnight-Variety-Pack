#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <game/net/packet/RKNetRoomPacket.hpp>
#include <game/race/RaceGlobals.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <nw4r/ut/Lock.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/es/es.h>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Reporting {

void ReportFinishTime(u8 playerIdx) {

    // Check if the race is online
    if (!RaceGlobals::isOnlineRace)
        return;

    // Check if the player is a local player
    bool isLocal = false;
    for (int i = 0; i < RaceConfig::instance->raceScenario.localPlayerCount; i++) {
        if (RaceConfig::instance->raceScenario.settings.hudPlayerIds[i] == playerIdx) {
            isLocal = true;
            break;
        }
    }

    if (!isLocal)
        return;

    // Get the finish time
    char buffer[32];
    u32 timer;
    u32 finishTime = RaceManager::instance->players[playerIdx]->finishTime->getTimeMs();
    snprintf(buffer, sizeof(buffer), "slot=%d|time=%d", playerIdx, finishTime);

    // Get the RaceManager timer
    // Use interrupts to get a more accurate value (??)
    {
        nw4r::ut::AutoInterruptLock lock;
        timer = RaceManager::instance->frameCounter;
    }

    // Send message
    Status::SendMessage("finish", buffer, timer);
}

void ReportFriendRoomStart(RKNetROOMPacket* packet) {
    if (packet->msgType == RKNetROOMPacket::MSG_ROOM_START)
        Status::SendMessage("friend_event_start", "3", packet->param1);
}

void ReportSignatureAndCert() {

    ALIGN(32) char signature[60]; // system-defined
    ALIGN(32) char b64Signature[80]; // pre-computed length
    ALIGN(32) char cert[0x180]; // system-defined
    int tokenLength = Status::token ? strlen(Status::token) : 0;

    // Get the certificate
    // Removed useless memset call here
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
