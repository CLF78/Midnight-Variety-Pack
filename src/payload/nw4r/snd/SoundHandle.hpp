#pragma once
#include "BasicSound.hpp"

namespace nw4r {
namespace snd {

class SoundHandle {
public:
    detail::BasicSound* sound;
};
size_assert(SoundHandle, 0x4);

} // namespace snd
} // namespace nw4r
