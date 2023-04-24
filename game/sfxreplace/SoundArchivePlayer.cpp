#include <nw4r/snd/SoundArchivePlayer.h>
#include <nw4r/snd/SoundStartable.h>
#include <stdlib/string.h>
#include "sfxreplace/DVDFileStream.h"

using namespace nw4r;
using namespace nw4r::snd;

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
extern "C" static snd::SoundStartable::StartResult detail_SetupSoundImplOverride(
                                            SoundArchivePlayer* self,
                                            SoundHandle* handle,
                                            ulong soundId,
                                            detail::BasicSound::AmbientInfo* ambientArgInfo,
                                            SoundActor* actor,
                                            bool holdFlag,
                                            const snd::SoundStartable::StartInfo* startInfo) {

    if (self->soundStreams()[soundId])
        soundId |= SASR_BIT;

    return self->detail_SetupSoundImpl(handle, soundId, ambientArgInfo, actor, holdFlag, startInfo);
}

// Glue code
kmBranch(0x8009DD68, detail_SetupSoundImplOverride);
kmBranch(0x8009DD88, detail_SetupSoundImplOverride);
kmBranch(0x800A1810, detail_SetupSoundImplOverride);

// Replace the file id to pass the stream pointer over
kmCallDefCpp(0x800A20C4, snd::SoundStartable::StartResult,
                         SoundArchivePlayer* self,
                         detail::BasicSound* sound,
                         SoundArchive::SoundInfo* commonInfo,
                         const SoundArchive::StrmSoundInfo* info,
                         snd::SoundStartable::StartInfo::StartOffsetType startOffsetType,
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

    // TODO actual scanning
    return self->LoadGroup(groupId, allocator, loadBlockSize);
}

// Glue code
kmCall(0x800A2A70, LoadGroupOverride);
kmCall(0x80210E04, LoadGroupOverride);
kmCall(0x80210ED0, LoadGroupOverride);

// Delete the custom sound streams when the data is invalidated
extern "C" static void InvalidateDataOverride(SoundArchivePlayer* self, const void* start, const void* end) {
    self->InvalidateData(start, end);

    DVDFileStream** streams = (DVDFileStream**)self->soundStreams();
    for (u32 soundId = 0; soundId < self->soundArchive->GetSoundCount(); soundId++) {
        if (start <= streams[soundId] && streams[soundId] <= end) {
            streams[soundId]->~DVDFileStream();
            streams[soundId] = nullptr;
        }
    }
}

// Glue code
kmWritePointer(0x802749EC, InvalidateDataOverride);
kmWritePointer(0x802A292C, InvalidateDataOverride);
