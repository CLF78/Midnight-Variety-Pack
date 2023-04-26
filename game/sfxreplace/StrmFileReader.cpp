#include <kamek.h>
#include <nw4r/snd/StrmFile.h>

namespace nw4r {
namespace snd {
namespace detail {

// Prevent muting on missing extra music channel
kmCallDefCpp(0x800A54F4, bool, StrmFileReader* self, StrmFileReader::StrmTrackInfo* info, int trackIndex) {

    if (self->ReadStrmTrackInfo(info, trackIndex))
        return true;

    return self->ReadStrmTrackInfo(info, 0);
}

} // namespace detail
} // namespace snd
} // namespace nw4r
