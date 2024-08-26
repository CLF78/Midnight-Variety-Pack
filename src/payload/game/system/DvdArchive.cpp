#include "DvdArchive.hpp"
#include "RaceConfig.hpp"
#include <egg/core/eggDecomp.hpp>
#include <mvp/cup/CupManager.hpp>
#include <platform/string.h>
#include <revolution/os/OSCache.h>
#include <revolutionex/net/NETDigest.h>
#include <wiimmfi/Reporting.hpp>

///////////////////////////////////////////
// Custom Cup System / Wiimmfi Telemetry //
///////////////////////////////////////////

// Hash track file, store the hash and report it to Wiimmfi if necessary
// Credits: Wiimmfi
REPLACE void DvdArchive::decompress(const char* path, EGG::Heap* heap) {

    // Get decompressed size
    const u32 decompressedSize = EGG::Decomp::getExpandSize(fileBuffer);

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
    if (path && !strstart(path, "Race/Course/")) {
        return;
    }

    // Check if it's a custom track, if not bail
    if (CupManager::IsSystemCourse(RaceConfig::instance->raceScenario.settings.courseId)) {
        return;
    }

    // If the hash hasn't already been computed for this track, do so
    u32* hash = CupData::tracks[CupManager::currentSzs].sha1Hash;
    if (!hash[0] && !hash[1] && !hash[2] && !hash[3] && !hash[4]) {
        NETCalcSHA1(hash, buffer, decompressedSize);
    }

    // Check if we're online, if not bail
    // We cannot use the isOnline variable since it hasn't been initialized yet
    if (!RaceConfig::instance->raceScenario.settings.isOnline()) {
        return;
    }

    // We're online, send the track hash to the server
    // TODO figure out what to do if we remove course slots, probably gotta talk it out with Wiimmfi devs
    Wiimmfi::Reporting::ReportTrackHash(hash, RaceConfig::instance->raceScenario.settings.courseId);
}
