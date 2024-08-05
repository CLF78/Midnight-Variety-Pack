#pragma once
#include <nw4r/ut/utList.hpp>

namespace nw4r {
namespace snd {

class SoundPlayer {
public:
    ut::LinkListImpl soundList;
    ut::LinkListImpl priorityList;
    ut::LinkListImpl heapList;

    int playableCount;
    int playableLimit;

    float volume;
    float lpfFreq;
    int outputLineFlag;
    float mainOutVolume;
    float remoteOutVolume[4];
    float mainSend;
    float fxSend[3];
};
size_assert(SoundPlayer, 0x5C);

} // namespace snd
} // namespace nw4r
