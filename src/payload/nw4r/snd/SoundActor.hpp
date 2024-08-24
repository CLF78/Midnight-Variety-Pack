#pragma once
#include "BasicSound.hpp"
#include "ExternalSoundPlayer.hpp"
#include "SoundStartable.hpp"

namespace nw4r {
namespace snd {

class SoundArchivePlayer;

class SoundActor : public SoundStartable {
public:
    SoundArchivePlayer* soundArchivePlayer;
    detail::ExternalSoundPlayer actorPlayer[4];
    detail::SoundActorParam actorParam;
};
size_assert(SoundActor, 0x54);

} // namespace snd
} // namespace nw4r
