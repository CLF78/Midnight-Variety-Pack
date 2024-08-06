#pragma once
#include <revolution/gx/GXStruct.h>

namespace nw4r {
namespace ut {

struct Color : public GXColor {};
size_assert(Color, sizeof(GXColor));

} // namespace ut
} // namespace nw4r
