#include <kamek.h>

namespace RawSaveExpansion {

struct Header {
    u32 magic;
    u32 fileLength;
    u32 versionNumber;
    u32 licenseOffsets[4];
    u32 checksum;
};

struct LicenseHeader {
    u32 magic;
    u32 licenseLength;
    u16 sectionCount;
    u16 headerLength;
    u32 sectionOffsets[];
};

struct LicenseSection {
    u32 magic;
    u8 data[];
};

struct Cup {
    u32 cupCount;
    u8 data[];

    // array of completions, 1 bit per cup
    // array of ranks, 2 bits per cup
};

} // namespace RawSaveEx
