#include <common/Common.hpp>
#include <nw4r/snd/StrmFile.hpp>

namespace nw4r {
namespace snd {
namespace detail {

//////////////////////
// BRSTM Cutoff Fix //
//////////////////////

// Prevent muting on missing extra music channel(s)
// Credits: stebler
REPLACE bool StrmFileReader::ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo* info, int trackIndex) const {
    if (REPLACED(info, trackIndex))
        return true;

    return REPLACED(info, 0);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
