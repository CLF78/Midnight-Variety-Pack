#include <common/Common.hpp>
#include <game/net/packet/RKNetRoomPacket.hpp>

// Contains all Wiimmfi telemetry
namespace Wiimmfi {
namespace Reporting {

    // Reports the race finish time of the given local player
    void ReportFinishTime(u8 playerIdx);

    // Reports a room start
    void ReportFriendRoomStart(RKNetROOMPacket* packet);

    // Reports the given track SHA1 hash
    void ReportTrackHash(u32* hash);

    // Reports the console signature and certificate
    void ReportSignatureAndCert();

} // namespace Reporting
} // namespace Wiimmfi
