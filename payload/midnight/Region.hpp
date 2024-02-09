#include <common/Common.hpp>

u16 RegionDetectAddr AT_ADDR(0x8000620A); // Use instruction to detect region

namespace Region {

enum Value {
    REGION_FREE = -1,
    REGION_P,
    REGION_E,
    REGION_J,
    REGION_K,
    REGION_UNK,
    REGION_COUNT,
};

inline Value Detect() {

    switch (RegionDetectAddr) {
        case 0x54A9: return REGION_P;
        case 0x5409: return REGION_E;
        case 0x53CD: return REGION_J;
        case 0x5511: return REGION_K;
        default: return REGION_UNK;
    }
}

inline const char* GetIdentifier() {
    static const char* identifiers[REGION_COUNT] = { "PAL", "USA", "JAP", "KOR", "UNK" };
    return identifiers[Detect()];
}

} // namespace Region
