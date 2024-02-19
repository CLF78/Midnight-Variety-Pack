#include <common/Common.hpp>
#include <dwc/dwc_base64.h>
#include <game/net/packet/RKNetRoomPacket.hpp>
#include <game/system/CourseMap.hpp>
#include <game/system/MultiDvdArchive.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/system/RaceManager.hpp>
#include <game/system/ResourceManager.hpp>
#include <nw4r/ut/Lock.hpp>
#include <platform/stdio.h>
#include <platform/string.h>
#include <revolution/es/es.h>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Reporting.hpp>
#include <wiimmfi/Status.hpp>

namespace Wiimmfi {
namespace Reporting {

void* GetSubfileHash(const char* path, int src, char* hash) {

    // Get the file and its size
    size_t fileSize;
    void* file = ResourceManager::instance->getFile(src, path, &fileSize);

    // If it doesn't exist, set the hash to zero
    if (!file) {
        hash[0] = '0';
        hash[1] = '\0';

    // Else hash it and print it in the buffer
    } else {
        u32 digest[5];
        NETCalcSHA1(digest, file, fileSize);
        sprintf(hash, HASH_STRING_FMT, digest[0], digest[1], digest[2], digest[3], digest[4]);
    }

    // Return the file pointer anyway
    return file;
}

void ReportConnectionMatrix(u32 aidsConnectedToMe) {
    Status::SendMessage("connected", "", aidsConnectedToMe);
}

void ReportCommonSubfiles() {

    // Initialize buffer for each message
    char buffers[4][HASH_STRING_SIZE];
    char statusMsgBuffer[200];

    // Send KartParam, DriverParam and ItemSlot hashes
    GetSubfileHash("kartParam.bin", MultiDvdArchive::COMMON, buffers[0]);
    GetSubfileHash("driverParam.bin", MultiDvdArchive::COMMON, buffers[1]);
    GetSubfileHash("ItemSlot.bin", MultiDvdArchive::COMMON, buffers[2]);
    sprintf(statusMsgBuffer, "kart=%s|driver=%s|item=%s", buffers[0], buffers[1], buffers[2]);
    Status::SendMessage("common_subfile_sha1", statusMsgBuffer);

    // Send GeoHit hashes
    GetSubfileHash("GeoHitTableItem.bin", MultiDvdArchive::COMMON, buffers[0]);
    GetSubfileHash("GeoHitTableItemObj.bin", MultiDvdArchive::COMMON, buffers[1]);
    GetSubfileHash("GeoHitTableKart.bin", MultiDvdArchive::COMMON, buffers[2]);
    GetSubfileHash("GeoHitTableKartObj.bin", MultiDvdArchive::COMMON, buffers[3]);
    sprintf(statusMsgBuffer, "ghti=%s|ghtio=%s|ghtk=%s|ghtko=%s", buffers[0], buffers[1],
            buffers[2], buffers[3]);
    Status::SendMessage("common_subfile_sha1", statusMsgBuffer);

    // Send minigame.kmg and ObjFlow hashes
    GetSubfileHash("minigame.kmg", MultiDvdArchive::COMMON, buffers[0]);
    GetSubfileHash("ObjFlow.bin", MultiDvdArchive::COMMON, buffers[1]);
    sprintf(statusMsgBuffer, "minigame=%s|objflow=%s", buffers[0], buffers[1]);
    Status::SendMessage("common_subfile_sha1", statusMsgBuffer);
}

void ReportCourseSubfiles() {

    // Setup hash buffers
    char buffers[3][HASH_STRING_SIZE];
    char statusMsgBuffer[160];

    // Get the KMP and hash it
    MapdataFileAccessor::SData* kmp = (MapdataFileAccessor::SData*)GetSubfileHash("course.kmp",
                                                                                  MultiDvdArchive::COURSE,
                                                                                  buffers[0]);

    // If KMP is invalid bail, the game will crash later anyway
    if (!kmp || kmp->numSections == 0)
        return;

    // Copy the cleaned kmp hash over as a failsafe for missing STGI
    *buffers[1] = *buffers[0];

    // Locate STGI section to overwrite lap/speed modifier
    for (int i = 0; i < kmp->numSections; i++) {

        // Get KMP section
        KmpSectionHeader* header = (KmpSectionHeader*)((u8*)kmp + kmp->headerSize + kmp->offsets[i]);

        // Check for magic
        if (header->sectionMagic != 'STGI')
            continue;

        // Get section data and check for lap/speed modifications
        // If the data is unchanged, do not hash the KMP again
        MapdataStage::SData* section = (MapdataStage::SData*)(header++);
        if (section->mLapCount == 3 && section->mSpeedMod == 0)
            break;

        // Else reset the fields to the default values and hash the file again
        u8 lapCount = section->mLapCount;
        u16 speedMod = section->mSpeedMod;
        section->mLapCount = 3;
        section->mSpeedMod = 0;
        GetSubfileHash("course.kmp", MultiDvdArchive::COURSE, buffers[1]);

        // Set values back and exit the loop
        section->mLapCount = lapCount;
        section->mSpeedMod = speedMod;
        break;
    }

    // Get KCL hash and send message
    GetSubfileHash("course.kcl", MultiDvdArchive::COURSE, buffers[2]);
    sprintf(statusMsgBuffer, "kmp=%s|clean-kmp=%s|kcl=%s", buffers[0], buffers[1], buffers[2]);
    Status::SendMessage("track_subfile_sha1", statusMsgBuffer);

    // Send LEX file hash, if the file exists
    void* lexFile = GetSubfileHash("course.lex", MultiDvdArchive::COURSE, buffers[0]);
    if (lexFile) {
        sprintf(statusMsgBuffer, "lex=%s", buffers[0]);
        Status::SendMessage("track_subfile_sha1", statusMsgBuffer);
    }

    // Send ItemSlotTable.slt hash, if the file exists
    void* itemSlotFile = GetSubfileHash("ItemSlotTable/ItemSlotTable.slt", MultiDvdArchive::COURSE, buffers[0]);
    if (itemSlotFile) {
        sprintf(statusMsgBuffer, "islot=%s", buffers[0]);
        Status::SendMessage("track_subfile_sha1", statusMsgBuffer);
    }

    // Get course_model hash (or the _d version if it's not available)
    void* courseModelFile = GetSubfileHash("course_model.brres", MultiDvdArchive::COURSE, buffers[0]);
    if (!courseModelFile)
        GetSubfileHash("course_d_model.brres", MultiDvdArchive::COURSE, buffers[0]);

    // Get vrcorn hash (or the _d version if it's not available)
    void* vrcornFile = GetSubfileHash("vrcorn_model.brres", MultiDvdArchive::COURSE, buffers[1]);
    if (!vrcornFile)
        GetSubfileHash("vrcorn_d_model.brres", MultiDvdArchive::COURSE, buffers[1]);

    // Get the minimap model and send message
    GetSubfileHash("map_model.brres", MultiDvdArchive::COURSE, buffers[2]);
    sprintf(statusMsgBuffer, "course%d=%s|vrcorn%d=%s|map=%s", courseModelFile == nullptr,
            buffers[0], vrcornFile == nullptr, buffers[1], buffers[2]);
    Status::SendMessage("track_subfile_sha1", statusMsgBuffer);
}

void ReportFinishTime(u8 playerIdx) {

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

    ALIGN(32) char cert[IOSECCCertSize];
    ALIGN(32) char signature[IOSECCSigSize];
    ALIGN(32) char b64Signature[DWC_Base64GetEncodedSize(sizeof(signature))+1];
    int tokenLength = Status::sToken ? strlen(Status::sToken) : 0;

    // Get the certificate
    // Removed useless memset call here
    s32 ret = ES_Sign((u8*)Status::sToken, tokenLength, (u8*)signature, (u8*)cert);
    if (ret == ES_ERR_OK) {

        // Encode and send the signature
        int len = DWC_Base64Encode(signature, sizeof(signature), b64Signature, sizeof(b64Signature));
        b64Signature[len] = '\0';
        Status::SendMessage("xy_sg", b64Signature, ret);

        // Encode and send the certificate
        len = DWC_Base64Encode(cert, sizeof(cert), Auth::sConsoleCert, sizeof(Auth::sConsoleCert));
        Auth::sConsoleCert[len] = '\0';
        Status::SendMessage("xy_ct", Auth::sConsoleCert, ret);

    } else {

        // Something failed, send invalid data
        Status::SendMessage("xy_sg", "invalid", ret);
        Status::SendMessage("xy_ct", "invalid", ret);
    }
}

void ReportTrackHash(u32* hash, u8 courseId) {

    // Convert the hash to a string
    // Using sprintf since the output is fixed size and the buffer is big enough
    char buffer[HASH_STRING_SIZE];
    sprintf(buffer, HASH_STRING_FMT, hash[0], hash[1], hash[2], hash[3], hash[4]);

    // Send the hash over with the course id
    Status::SendMessage("track_sha1", buffer, courseId);
}

} // namespace Reporting
} // namespace Wiimmfi
