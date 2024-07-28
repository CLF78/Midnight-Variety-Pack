#pragma once
#include <common/Common.hpp>
#include <mvp/save/SaveExpansionLicense.hpp>

class SaveExpansion {
public:

    struct Header {

        bool IsValid(u32 fileSize);

        u32 magic;
        u32 revision;
        u32 headerSize;
        u32 checksum;
        u32 licenseCount;
        u32 licenseOffsets[];
    };

    SaveExpansion();
    u32 GetRequiredSpace();
    void Init();
    bool Read(u8* buffer, u32 bufferSize);
    void Write();

    static SaveExpansion* construct(void* buffer) { return new(buffer) SaveExpansion(); }
    SaveExpansionLicense* GetLicense(u8 idx) { return &mLicenses[idx]; }

    SaveExpansionLicense mLicenses[SAVEEX_LICENSE_COUNT];

    u8* mWriteBuffer;
    u32 mWriteBufferSize;
};
