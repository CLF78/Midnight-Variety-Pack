#pragma once
#include <nw4r/math/types.hpp>
#include <nw4r/ut/rect.hpp>

namespace nw4r {
namespace lyt {

class DrawInfo {
public:
    virtual ~DrawInfo();

    math::MTX34 mViewMtx;
    ut::Rect mViewRect;
    math::VEC2 mLocationAdjustScale;
    f32 mGlobalAlpha;
    u8 mFlag;
    PAD(3);
};
size_assert(DrawInfo, 0x54);

} // namespace lyt
} // namespace nw4r
