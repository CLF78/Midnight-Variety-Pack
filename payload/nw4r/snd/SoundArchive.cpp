#include <common/Common.hpp>
#include <midnight/SoundExpansion.hpp>
#include <nw4r/snd/DvdSoundArchive.hpp>
#include <revolution/os/OS.h>
#include <platform/new.hpp>

namespace nw4r {
namespace snd {

ulong SoundArchive::GetSoundCount() const {
    return fileReader->GetSoundCount();
}

//////////////////////////////////////
// Patches for SFX Expansion System //
//////////////////////////////////////

// Override sound type for replaced sfx
kmBranchDefCpp(0x8009DF30, NULL, SoundArchive::SoundType, SoundArchive* self, ulong soundId) {
    if (soundId & SASR_BIT)
        return SoundArchive::SOUND_TYPE_STRM;

    return self->fileReader->GetSoundType(soundId);
}

// Ensure the original sfx sound info is read
kmBranchDefCpp(0x8009DF40, NULL, bool, SoundArchive* self, ulong soundId, SoundArchive::SoundInfo* soundInfo) {

    if (!self->fileReader->ReadSoundInfo(soundId & ~SASR_BIT, soundInfo))
        return false;

    if (soundId & SASR_BIT)
        soundInfo->fileId = self->detail_GetFileCount() + (soundId & ~SASR_BIT);

    return true;
}

// Ensure the StrmSoundInfo structure is filled properly
kmBranchDefCpp(0x8009DF60, NULL, bool, SoundArchive* self,
                                       ulong soundId,
                                       SoundArchive::StrmSoundInfo* strmSoundInfo) {

    if (!(soundId & SASR_BIT))
        return self->fileReader->ReadStrmSoundInfo(soundId, strmSoundInfo);

    // Update some flags to avoid broken defaults
    switch (self->GetSoundType(soundId & ~SASR_BIT)) {
    case SoundArchive::SOUND_TYPE_SEQ:
    case SoundArchive::SOUND_TYPE_WAVE:
        strmSoundInfo->startPosition = 0;
        strmSoundInfo->allocChannelCount = 0; // Use the channel count from the BRSTM header
        strmSoundInfo->allocTrackFlag = 0x1;
        return true;
    default:
        return false;
    }
}

// Replace the file stream when opening an sfx
kmHookFn ut::FileStream* OpenFileStreamOverride(SoundArchive* self,
                                                         u32 fileId,
                                                         void* buffer,
                                                         s32 size) {

    if (fileId & SASR_BIT) {

        // Ensure the stream fits the buffer
        if (size < sizeof(DvdSoundArchive::DvdFileStream))
            return nullptr;

        // Get the stream, bail if it does not exist
        DvdSoundArchive::DvdFileStream* stream = (DvdSoundArchive::DvdFileStream*)fileId;
        if (!stream)
            return nullptr;

        // Create a new stream
        stream = new (buffer) DvdSoundArchive::DvdFileStream(&stream->fileInfo.dvdInfo, 0, 0x7FFFFFFF);
        return (ut::FileStream*)stream;
    }

    // If the fileId is valid, proceed to the original call
    return self->detail_OpenFileStream(fileId, buffer, size);
}

// Glue code
kmCall(0x800A002C, OpenFileStreamOverride);
kmCall(0x800A2384, OpenFileStreamOverride);
kmCall(0x800A26C8, OpenFileStreamOverride);

} // namespace snd
} // namespace nw4r
