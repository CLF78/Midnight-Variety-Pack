#include <kamek.h>

namespace EGG {

class AudioTrack;
struct AudioTrackCallback{
    void (*callbackFunc);
    AudioTrack* ptr;
};
size_assert(AudioTrackCallback, 0x8);

class AudioTrack {
public:
    AudioTrackCallback callback;

    virtual void Reset();
    virtual void Calc();

    u32 delay;
    float maxVolume;
    float curVolume;
    float step;
};
size_assert(AudioTrack, 0x1C);

} // namespace EGG
