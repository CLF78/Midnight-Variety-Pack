#include <kamek.h>
#include <nw4r/snd/SoundArchiveFile.h>
#include <rvl/dvd/dvd.h>

namespace nw4r {
namespace snd {

class DVDSoundArchive : public SoundArchive {
public:
    detail::SoundArchiveFileReader reader;
    DVDFileInfo fileInfo;
    bool open;
};
size_assert(DVDSoundArchive, 0x18C);

} // namespace snd
} // namespace nw4r
