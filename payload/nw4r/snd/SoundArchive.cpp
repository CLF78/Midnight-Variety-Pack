#include <common/Common.hpp>
#include <midnight/SoundExpansion.hpp>
#include <nw4r/snd/DvdSoundArchive.hpp>

namespace nw4r {
namespace snd {

ulong SoundArchive::GetSoundCount() const {
    return fileReader->GetSoundCount();
}

///////////////////////
// Custom SFX System //
///////////////////////

// Force BRSTM sound type for replaced sound IDs
// Credits: stebler
REPLACE SoundArchive::SoundType SoundArchive::GetSoundType(ulong soundId) const {
    if (soundId & SASR_BIT)
        return SoundArchive::SOUND_TYPE_STRM;

    return fileReader->GetSoundType(soundId);
}

// Replace the file ID with the replacement BRSTM's ID
// Credits: stebler
REPLACE bool SoundArchive::ReadSoundInfo(ulong soundId, SoundInfo* soundInfo) const {
    if (!fileReader->ReadSoundInfo(soundId & ~SASR_BIT, soundInfo))
        return false;

    if (soundId & SASR_BIT)
        soundInfo->fileId = detail_GetFileCount() + (soundId & ~SASR_BIT);

    return true;
}

// Ensure the StrmSoundInfo structure is filled properly for replaced sounds
// Credits: stebler
REPLACE bool SoundArchive::detail_ReadStrmSoundInfo(ulong soundId, StrmSoundInfo* strmSoundInfo) const {

    // If the sound was not replaced, do the original call
    if (!(soundId & SASR_BIT))
        return fileReader->ReadStrmSoundInfo(soundId, strmSoundInfo);

    // Update some flags to avoid broken defaults
    switch (GetSoundType(soundId & ~SASR_BIT)) {
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

// Replace the file stream when opening a sound file
// Credits: stebler
REPLACE ut::FileStream* SoundArchive::detail_OpenFileStream(ulong fileId, void* buffer, s32 size) const {

    // Check if the file ID is a faked pointer
    if (fileId & 0x80000000) {

        // Ensure the stream fits the buffer
        if (size < sizeof(DvdSoundArchive::DvdFileStream))
            return nullptr;

        // Get the stream, faked as the file ID
        DvdSoundArchive::DvdFileStream* stream = (DvdSoundArchive::DvdFileStream*)fileId;
        if (!stream)
            return nullptr;

        // Construct the stream
        stream = new (buffer) DvdSoundArchive::DvdFileStream(&stream->fileInfo.dvdInfo, 0, 0x7FFFFFFF);
        return (ut::FileStream*)stream;
    }

    // If the fileId is valid, proceed to the original call
    return REPLACED(fileId, buffer, size);
}

} // namespace snd
} // namespace nw4r
