#include "DvdSoundArchive.hpp"
#include "SoundArchivePlayer.hpp"
#include "SoundStartable.hpp"
#include <mvp/SoundExpansion.hpp>
#include <platform/string.h>
#include <revolution/dvd/dvd.h>

namespace nw4r {
namespace snd {

///////////////////////
// Custom SFX System //
///////////////////////

// Custom function to obtain the custom sound stream array
// Credits: stebler
ut::FileStream** SoundArchivePlayer::soundStreams() {
    u8* end = (u8*)(setupBufferAddress) + setupBufferSize;
    return (ut::FileStream**)(end) - soundArchive->GetSoundCount();
}

// Allocate extra memory for the custom sound stream array
// Credits: stebler
REPLACE ulong SoundArchivePlayer::GetRequiredMemSize(const SoundArchive* archive) {
    ulong size = REPLACED(archive);
    size += archive->GetSoundCount() * sizeof(ut::FileStream*);
    return size;
}

// Clear the custom sound stream array on allocation
// Credits: stebler
REPLACE bool SoundArchivePlayer::SetupMram(const SoundArchive* archive, void* buffer, size_t bufferSize) {
    if (!REPLACED(archive, buffer, bufferSize))
        return false;

    memset(soundStreams(), 0, archive->GetSoundCount() * sizeof(ut::FileStream*));
    return true;
}

// Enable the SASR bit if an external replacement is found
// Credits: stebler
REPLACE SoundStartable::StartResult SoundArchivePlayer::detail_SetupSoundImpl(
                                    SoundHandle* handle, ulong soundId,
                                    detail::BasicSound::AmbientInfo* ambientArgInfo,
                                    SoundActor* actor, bool holdFlag,
                                    const SoundStartable::StartInfo* startInfo) {

    if (soundStreams()[soundId])
        soundId |= SASR_BIT;

    return REPLACED(handle, soundId, ambientArgInfo, actor, holdFlag, startInfo);
}

// Pass the file stream pointer instead of the file ID to PrepareStrmImpl if the sound was replaced
// Credits: stebler
REPLACE SoundStartable::StartResult SoundArchivePlayer::PrepareStrmImpl(
                                    detail::BasicSound* sound,
                                    const SoundArchive::SoundInfo* commonInfo,
                                    const SoundArchive::StrmSoundInfo* info,
                                    SoundStartable::StartInfo::StartOffsetType startOffsetType,
                                    int startOffset) {

    // Remove const from type
    SoundArchive::SoundInfo* sndInfo = (SoundArchive::SoundInfo*)commonInfo;

    // Obtain the sound id from the file id and replace the latter with a pointer to the custom stream
    if (sndInfo->fileId > soundArchive->detail_GetFileCount()) {
        u32 soundId = sndInfo->fileId - soundArchive->detail_GetFileCount();
        sndInfo->fileId = (u32)(soundStreams()[soundId]);
    }

    // Original call
    return REPLACED(sound, commonInfo, info, startOffsetType, startOffset);
}

// Scan for external SFX replacements
// Credits: stebler
REPLACE bool SoundArchivePlayer::LoadGroup(ulong groupId, SoundMemoryAllocatable* allocator,
                                           ulong loadBlockSize) {

    // Try opening the sfx directory
    DVDDir sfxDir;
    if (DVDOpenDir(SASR_SFX_DIR, &sfxDir)) {
        LOG_DEBUG("Scanning for sound effect replacements for group %d...", groupId);

        // Read each folder entry until exhausted
        DVDDirEntry curEntry;
        while (DVDReadDir(&sfxDir, &curEntry)) {

            // If it's a directory, skip it
            if (curEntry.isDir)
                continue;

            // Initialize parsing loop
            u32 soundId = 0;
            char* cs;

            // Parse each character until a non-decimal character or 9 decimal characters are reached
            for (cs = curEntry.name; cs - curEntry.name < 9 && '0' <= *cs && *cs <= '9'; cs++)
                soundId = soundId * 10 + (*cs - '0');

            // If no characters have been parsed, bail
            if (cs == curEntry.name)
                continue;

            // If the file does not end in BRSTM, bail
            if (strcmp(cs, BRSTM_SUFFIX))
                continue;

            // Ignore BRSTM sound type
            switch (soundArchive->GetSoundType(soundId)) {
            case SoundArchive::SOUND_TYPE_SEQ:
            case SoundArchive::SOUND_TYPE_WAVE:
                break;
            default:
                continue;
            }

            // If the custom stream already exists, skip
            if (soundStreams()[soundId])
                continue;

            // Read the sound info to get the file id
            SoundArchive::SoundInfo soundInfo;
            if (!soundArchive->ReadSoundInfo(soundId, &soundInfo))
                continue;

            // Read the file info to get the file positions
            SoundArchive::FileInfo fileInfo;
            if (!soundArchive->detail_ReadFileInfo(soundInfo.fileId, &fileInfo))
                continue;

            // Find the file position belonging to the correct group id
            for (u32 i = 0; i < fileInfo.filePosCount; i++) {
                SoundArchive::FilePos filePos;
                if (!soundArchive->detail_ReadFilePos(soundInfo.fileId, i, &filePos))
                    continue;

                if (filePos.groupId != groupId)
                    continue;

                // Open it and allocate the custom stream
                DVDFileInfo fileInfo;
                if (DVDFastOpen(curEntry.entryNum, &fileInfo)) {
                    void* buffer = allocator->Alloc(sizeof(DvdSoundArchive::DvdFileStream));
                    DvdSoundArchive::DvdFileStream* stream = new (buffer) DvdSoundArchive::DvdFileStream(curEntry.entryNum, 0, 0x7FFFFFFF);
                    soundStreams()[soundId] = stream;

                    LOG_DEBUG("Found music file %s", curEntry.name);
                }

                break;
            }
        }
    }

    // Original call
    return REPLACED(groupId, allocator, loadBlockSize);
}

// Delete the custom sound streams when the data is invalidated
// Credits: stebler
REPLACE void SoundArchivePlayer::InvalidateData(const void* start, const void* end) {
    REPLACED(start, end);

    DvdSoundArchive::DvdFileStream** streams = (DvdSoundArchive::DvdFileStream**)soundStreams();
    for (u32 soundId = 0; soundId < soundArchive->GetSoundCount(); soundId++) {
        if (start <= streams[soundId] && streams[soundId] <= end) {
            streams[soundId]->~DvdFileStream();
            streams[soundId] = nullptr;
        }
    }
}

} // namespace snd
} // namespace nw4r
