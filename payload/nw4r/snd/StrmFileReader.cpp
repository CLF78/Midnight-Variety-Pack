#include <common/Common.hpp>
#include <nw4r/snd/StrmFile.hpp>

namespace nw4r {
namespace snd {
namespace detail {

//////////////////////////////////////
// Patches for SFX Expansion System //
//////////////////////////////////////

// Prevent muting on missing extra music channel
kmCallDefCpp(0x800A54F4, bool, StrmFileReader* self, StrmFileReader::StrmTrackInfo* info, int trackIndex) {

    if (self->ReadStrmTrackInfo(info, trackIndex))
        return true;

    return self->ReadStrmTrackInfo(info, 0);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
