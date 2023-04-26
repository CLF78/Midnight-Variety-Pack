#include <kamek.h>
#include <nw4r/snd/StrmFile.h>

using namespace nw4r::snd::detail;

// Prevent muting on missing alternate track slot
kmCallDefCpp(0x800A54F4, bool, StrmFileReader* self, StrmFileReader::StrmTrackInfo* info, int trackIndex) {

    if (self->ReadStrmTrackInfo(info, trackIndex))
        return true;

    return self->ReadStrmTrackInfo(info, 0);
}
