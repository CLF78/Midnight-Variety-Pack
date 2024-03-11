#include <common/Common.hpp>
#include <midnight/save/SaveExpansionLicense.hpp>
#include <platform/new.hpp>

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

    static SaveExpansion* construct(void* buffer) {
        return new(buffer) SaveExpansion();
    }

    SaveExpansion() : mLicenses() {
        Init();
        mWriteBuffer = new u8[GetRequiredSpace()];
    }

    SaveExpansionLicense* GetLicense(u8 idx) { return &mLicenses[idx]; }

    u32 GetRequiredSpace();
    void Init();
    bool Read(u8* buffer, u32 bufferSize);
    bool Write(u8* buffer, u32 bufferSize);

    SaveExpansionLicense mLicenses[SAVEEX_LICENSE_COUNT];
    u8* mWriteBuffer;
};
