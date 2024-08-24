#pragma once
#include <nw4r/ut/utList.hpp>

namespace nw4r {
namespace snd {
namespace detail {

class DisposeCallback {
public:
    ut::Link disposeLink;
    virtual ~DisposeCallback();
};
size_assert(DisposeCallback, 0xC);

} // namespace detail
} // namespace snd
} // namespace nw4r
