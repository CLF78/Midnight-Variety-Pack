#include <mvp/Region.hpp>

namespace Region {

const char* GetIdentifier() {
    static const char* identifiers[REGION_COUNT] = { "PAL", "USA", "JAP", "KOR", "UNK" };
    return identifiers[Detect()];
}

} // namespace Region
