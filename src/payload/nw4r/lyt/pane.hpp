#pragma once
#include "material.hpp"
#include <revolution/mtx.h>

namespace nw4r {
namespace lyt {

class Pane {
public:
    Material* GetMaterial();

    UNK(0x28);

    VEC3 translation;
    VEC3 rotation;
    VEC2 scale;
    float width;
    float height;
    UNK(0xB8 - 0x50);

    u8 alpha;
    UNK(0xD8 - 0xB9);
};
size_assert(Pane, 0xD8);

} // namespace lyt
} // namespace nw4r
