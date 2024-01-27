#include <common/Common.hpp>
#include <game/net/packet/RKNetRacePacketHeader.hpp>

namespace Wiimmfi {
namespace Security {

    // Validates a match packet to prevent invalid data from being transmitted
    // Returns true if valid
    bool ValidatePacket(u32 aid, RKNetRACEPacketHeader* data, u32 dataLength);

} // namespace Security
} // namespace Wiimmfi
