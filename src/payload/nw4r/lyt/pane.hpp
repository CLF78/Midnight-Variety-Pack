#pragma once
#include <common/Common.hpp>
#include <nw4r/lyt/material.hpp>
#include <revolution/mtx.h>

namespace nw4r {
namespace lyt {

class Pane {
public:
    Material* GetMaterial();

    u8 unk[0x28];

    VEC3 translation;
    VEC3 rotation;
    VEC2 scale;
    float width;
    float height;
    u8 unk2[0xB8 - 0x50];

    u8 alpha;
    u8 unk3[0xD8 - 0xB9];
};
size_assert(Pane, 0xD8);

} // namespace lyt
} // namespace nw4r
