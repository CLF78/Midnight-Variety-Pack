#include <common/Common.hpp>
#include <game/net/packet/RKNetRoomPacket.hpp>

// Contains all Wiimmfi telemetry
namespace Wiimmfi {
namespace Reporting {

    // Reports a room start
    void ReportFriendRoomStart(RKNetROOMPacket* packet);

    // Sends the signature and the certificate to the server
    void ReportSignatureAndCert();

} // namespace Reporting
} // namespace Wiimmfi
