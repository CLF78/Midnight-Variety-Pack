#pragma once

namespace nw4r {
namespace snd {
namespace detail {

class StrmFileReader {
public:
    struct StrmTrackInfo {
        u8 volume;
        u8 pan;
        PAD(2);
        int channelCount;
        u8 channelIndexTable[32];
    };

    bool ReadStrmTrackInfo(StrmTrackInfo* trackInfo, int trackIndex) const;
};

} // namespace detail
} // namespace snd
} // namespace nw4r
