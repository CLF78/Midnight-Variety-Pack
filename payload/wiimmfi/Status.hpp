#include <common/Common.hpp>

namespace Wiimmfi {
namespace Status {

    // Decode the token received from the server and scrambles it
    void DecodeToken(const char* encodedToken);

    // Sends a status message to the server
    void SendMessage(const char* key, const char* value, int statusCode = -1);

    // The token received on login
    extern char* token;

} // namespace Port
} // namespace Wiimmfi
