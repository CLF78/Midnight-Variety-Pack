#include <kamek.h>
#include <nw4r/snd/DvdSoundArchive.h>
#include <nw4r/snd/SoundArchivePlayer.h>
#include <nw4r/snd/SoundStartable.h>
#include <rvl/dvd/dvd.h>
#include <rvl/os/OS.h>
#include <stdlib/new.h>
#include <stdlib/string.h>

namespace nw4r {
namespace snd {

// Custom function to obtain the custom sound streams array
ut::FileStream** SoundArchivePlayer::soundStreams() {
    u8* end = (u8*)(setupBufferAddress) + setupBufferSize;
    return (ut::FileStream**)(end) - soundArchive->GetSoundCount();
}

// Allocate additional memory to store the custom sound streams array
extern "C" static ulong GetRequiredMemSizeOverride(SoundArchivePlayer* player,
                                                   const SoundArchive* archive) {
    ulong size = player->GetRequiredMemSize(archive);
    size += archive->GetSoundCount() * sizeof(ut::FileStream*);
    return size;
}

// Glue code
kmCall(0x802107EC, GetRequiredMemSizeOverride);
kmCall(0x8021095C, GetRequiredMemSizeOverride);
kmCall(0x80210B04, GetRequiredMemSizeOverride);
kmCall(0x80210C38, GetRequiredMemSizeOverride);

// Clear the custom sound streams array on allocation
kmCallDefCpp(0x800A0960, bool, SoundArchivePlayer* self,
                               const SoundArchive* archive,
                               void* buffer,
                               size_t bufferSize) {

    if (!self->SetupMram(archive, buffer, bufferSize))
        return false;

    memset(self->soundStreams(), 0, archive->GetSoundCount() * sizeof(ut::FileStream*));
    return true;
}

// Enable the SASR bit when an external replacement is found
extern "C" static SoundStartable::StartResult detail_SetupSoundImplOverride(
                                              SoundArchivePlayer* self,
                                              SoundHandle* handle,
                                              ulong soundId,
                                              detail::BasicSound::AmbientInfo* ambientArgInfo,
                                              SoundActor* actor,
                                              bool holdFlag,
                                              const SoundStartable::StartInfo* startInfo) {

    if (self->soundStreams()[soundId])
        soundId |= SASR_BIT;

    return self->detail_SetupSoundImpl(handle, soundId, ambientArgInfo, actor, holdFlag, startInfo);
}

// Glue code
kmBranch(0x8009DD68, detail_SetupSoundImplOverride);
kmBranch(0x8009DD88, detail_SetupSoundImplOverride);
kmBranch(0x800A1810, detail_SetupSoundImplOverride);

// Replace the file id to pass the stream pointer over
kmCallDefCpp(0x800A20C4, SoundStartable::StartResult,
                         SoundArchivePlayer* self,
                         detail::BasicSound* sound,
                         SoundArchive::SoundInfo* commonInfo,
                         const SoundArchive::StrmSoundInfo* info,
                         SoundStartable::StartInfo::StartOffsetType startOffsetType,
                         int startOffset) {

    // Obtain the sound id from the file id and replace the latter with a pointer to the custom stream
    if (commonInfo->fileId > self->soundArchive->detail_GetFileCount()) {
        u32 soundId = commonInfo->fileId - self->soundArchive->detail_GetFileCount();
        commonInfo->fileId = (u32)(self->soundStreams()[soundId]);
    }

    // Original call
    return self->PrepareStrmImpl(sound, commonInfo, info, startOffsetType, startOffset);
}

// Scan for external SFX replacements
extern "C" static bool LoadGroupOverride(SoundArchivePlayer* self,
                                        ulong groupId,
                                        SoundMemoryAllocatable* allocator,
                                        ulong loadBlockSize) {

    // Try opening the sfx directory
    DVDDir sfxDir;
    if (DVDOpenDir(SASR_SFX_DIR, &sfxDir)) {

        // Read each folder entry
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
            if (strcmp(cs, ".brstm"))
                continue;

            // Ignore BRSTM sound type
            switch (self->soundArchive->GetSoundType(soundId)) {
            case SoundArchive::SOUND_TYPE_SEQ:
            case SoundArchive::SOUND_TYPE_WAVE:
                break;
            default:
                continue;
            }

            // If the custom stream already exists, skip
            if (self->soundStreams()[soundId])
                continue;

            // Read the sound info to get the file id
            SoundArchive::SoundInfo soundInfo;
            if (!self->soundArchive->ReadSoundInfo(soundId, &soundInfo))
                continue;

            // Read the file info to get the file positions
            SoundArchive::FileInfo fileInfo;
            if (!self->soundArchive->detail_ReadFileInfo(soundInfo.fileId, &fileInfo))
                continue;

            // Find the file position belonging to the correct group id
            for (u32 i = 0; i < fileInfo.filePosCount; i++) {
                SoundArchive::FilePos filePos;
                if (!self->soundArchive->detail_ReadFilePos(soundInfo.fileId, i, &filePos))
                    continue;

                if (filePos.groupId != groupId)
                    continue;

                // Open it and allocate the custom stream
                DVDFileInfo fileInfo;
                if (DVDFastOpen(curEntry.entryNum, &fileInfo)) {
                    void* buffer = allocator->Alloc(sizeof(DvdSoundArchive::DvdFileStream));
                    DvdSoundArchive::DvdFileStream* stream = new (buffer) DvdSoundArchive::DvdFileStream(curEntry.entryNum, 0, 0x7FFFFFFF);
                    self->soundStreams()[soundId] = stream;
                }

                break;
            }
        }
    }

    // Original call
    return self->LoadGroup(groupId, allocator, loadBlockSize);
}

// Glue code
kmCall(0x800A2A70, LoadGroupOverride);
kmCall(0x80210E04, LoadGroupOverride);
kmCall(0x80210ED0, LoadGroupOverride);

// Delete the custom sound streams when the data is invalidated
extern "C" static void InvalidateDataOverride(SoundArchivePlayer* self, const void* start, const void* end) {
    self->InvalidateData(start, end);

    DvdSoundArchive::DvdFileStream** streams = (DvdSoundArchive::DvdFileStream**)self->soundStreams();
    for (u32 soundId = 0; soundId < self->soundArchive->GetSoundCount(); soundId++) {
        if (start <= streams[soundId] && streams[soundId] <= end) {
            streams[soundId]->~DvdFileStream();
            streams[soundId] = nullptr;
        }
    }
}

// Glue code
kmWritePointer(0x802749EC, InvalidateDataOverride);
kmWritePointer(0x802A292C, InvalidateDataOverride);

} // namespace snd
} // namespace nw4r
