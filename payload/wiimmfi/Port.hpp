#include <common/Common.hpp>

namespace Wiimmfi {
namespace Port {

    ///////////////
    // Variables //
    ///////////////

    // The P2P port received from Wiimmfi in the authentication stage
    extern u16 port;

    // The P2P port the user wishes to use
    // TODO allow players to change this
    extern u16 userPort;

} // namespace Port
} // namespace Wiimmfi
