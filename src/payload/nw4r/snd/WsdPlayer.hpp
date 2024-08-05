#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class WsdPlayer {
public:

    class WsdCallback {
    public:
        virtual ~WsdCallback();
    };
    size_assert(WsdCallback, 0x4);
};

} // namespace detail
} // namespace snd
} // namespace nw4r
