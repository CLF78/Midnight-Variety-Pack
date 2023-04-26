#include <kamek.h>
#include <nw4r/snd/DVDSoundArchive.h>
#include <stdlib/new.h>
#include "sfxreplace/DVDFileStream.h"

using namespace nw4r::snd;

// Update buffer size to fit the custom sound stream
kmBranchDefCpp(0x80091380, NULL, ulong) {
    return sizeof(DVDFileStream);
}

// Replace sound stream with custom class
kmBranchDefCpp(0x80091210, NULL, nw4r::ut::FileStream*, DVDSoundArchive* self,
                                                        void* buffer,
                                                        s32 size,
                                                        u32 start,
                                                        u32 length) {

    if (!self->open)
        return nullptr;

    if (size < sizeof(DVDFileStream))
        return nullptr;

    return new (buffer) DVDFileStream(&self->fileInfo, start, length);
}

// Replace sound stream with custom class
kmBranchDefCpp(0x800912C0, NULL, nw4r::ut::FileStream*, DVDSoundArchive* self,
                                                        void* buffer,
                                                        s32 size,
                                                        const char* extFilePath,
                                                        u32 start,
                                                        u32 length) {
    if (size < sizeof(DVDFileStream))
        return nullptr;

    return new (buffer) DVDFileStream(extFilePath);
}
