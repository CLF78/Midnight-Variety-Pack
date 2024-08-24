#pragma once
#include <nw4r/ut/utList.hpp>

namespace nw4r {
namespace snd {
namespace detail {

class ExternalSoundPlayer {
public:
    ut::LinkListImpl soundList;
    int playableCount;
};
size_assert(ExternalSoundPlayer, 0x10);

} // namespace detail
} // namespace snd
} // namespace nw4r
