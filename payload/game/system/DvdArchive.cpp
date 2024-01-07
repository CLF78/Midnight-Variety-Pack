#include <common/Common.hpp>
#include <egg/core/eggDecomp.hpp>
#include <game/system/DvdArchive.hpp>
#include <game/system/RaceConfig.hpp>
#include <midnight/cup/CupManager.hpp>
#include <platform/string.h>
#include <revolution/os/OSCache.h>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Reporting.hpp>

/////////////////////////////////////////////////////////
// Patches for Custom Cup System and Wiimmfi Telemetry //
/////////////////////////////////////////////////////////

// Hash track files, store the hash and report it to Wiimmfi if necessary
void DvdArchive::decompress(const char* path, EGG::Heap* heap) {

    // Get decompressed size
    u32 decompressedSize = EGG::Decomp::getExpandSize(fileBuffer);

    // Allocate the buffer and decode the file in it
    void* buffer = heap->alloc(decompressedSize, 0x20);
    EGG::Decomp::decodeSZS(fileBuffer, buffer);

    // Store info and invalidate cache
    archiveSize = decompressedSize;
    archiveBuffer = buffer;
    archiveHeap = heap;
    DCStoreRange(buffer, decompressedSize);
    state = DvdArchive::DECOMPRESSED;

    // Check if it's a track file, if not bail
    if (strncmp(path, "Race/Course/", sizeof("Race/Course/")-1) != 0)
        return;

    // Check if it's a custom track, if not bail
    if (CupManager::IsSystemCourse(RaceConfig::instance->raceScenario.settings.courseId))
        return;

    // If the hash hasn't already been computed for this track, do so
    u32* hash = CupData::tracks[CupManager::currentSzs].sha1Hash;
    if (!(hash[0] && hash[1] && hash[2] && hash[3] && hash[4]))
        NETCalcSHA1(hash, buffer, decompressedSize);

    // Check if we're online, if not bail
    // We cannot use the isOnline variable since it hasn't been initialized yet
    if (!RaceConfig::instance->raceScenario.settings.isOnline())
        return;

    // We're online, send the track hash to the server
    Wiimmfi::Reporting::ReportTrackHash(hash);
}

// Replace the game function
kmBranchDefCpp(0x80519508, NULL, void, DvdArchive* self, const char* path, EGG::Heap* heap) {
    self->decompress(path, heap);
}
