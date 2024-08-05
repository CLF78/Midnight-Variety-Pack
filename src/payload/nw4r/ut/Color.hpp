#pragma once
#include <revolution/gx/GXStruct.h>

namespace nw4r {
namespace ut {

struct Color : public GXColor {};
size_assert(Color, 0x4);

} // namespace ut
} // namespace nw4r
