#pragma once
#include <nw4r/snd/BasicSound.hpp>
#include <nw4r/snd/ExternalSoundPlayer.hpp>
#include <nw4r/snd/SoundStartable.hpp>

namespace nw4r {
namespace snd {

class SoundArchivePlayer;

class SoundActor : public SoundStartable {
    public:
        SoundArchivePlayer& soundArchivePlayer;
        detail::ExternalSoundPlayer actorPlayer[4];
        detail::SoundActorParam actorParam;
};

} // namespace snd
} // namespace nw4r
