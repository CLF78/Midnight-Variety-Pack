#include <common/Common.hpp>
#include <wiimmfi/Security.hpp>

namespace Wiimmfi {
namespace Security {

// TODO implementation - see WiiLink24 version of this patch
bool ValidatePacket(u32 aid, RKNetRACEPacketHeader* data, u32 dataLength) {
    return true;
}

} // namespace Security
} // namespace Wiimmfi
