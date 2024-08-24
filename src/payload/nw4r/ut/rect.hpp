#pragma once

namespace nw4r {
namespace ut {

struct Rect {
    f32 left;
    f32 top;
    f32 right;
    f32 bottom;
};
size_assert(Rect, 0x10);

} // namespace ut
} // namespace nw4r
