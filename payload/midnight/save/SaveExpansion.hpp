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

    SaveExpansion();
    u32 GetRequiredSpace();
    void Init();
    bool Read();
    bool Write();

    static SaveExpansion* construct(void* buffer) { return new(buffer) SaveExpansion(); }
    SaveExpansionLicense* GetLicense(u8 idx) { return &mLicenses[idx]; }

    void DeleteReadBuffer() {
        if (mReadBuffer) {
            delete mReadBuffer;
            mReadBufferSize = 0;
        }
    }

    SaveExpansionLicense mLicenses[SAVEEX_LICENSE_COUNT];

    u8* mWriteBuffer; // permanent
    u32 mWriteBufferSize;

    u8* mReadBuffer; // temporary
    u32 mReadBufferSize;
};
