#include <common/Common.hpp>
#include <game/net/packet/RKNetRoomPacket.hpp>

// Contains all Wiimmfi telemetry
namespace Wiimmfi {
namespace Reporting {

    // Reports the race finish time of the given local player
    void ReportFinishTime(u8 playerIdx);

    // Reports a room start
    void ReportFriendRoomStart(RKNetROOMPacket* packet);

    // Sends the signature and the certificate to the server
    void ReportSignatureAndCert();

} // namespace Reporting
} // namespace Wiimmfi
