#pragma once
#include <common/Common.hpp>

namespace Wiimmfi {
namespace Port {

    // The P2P port received from Wiimmfi in the authentication stage
    extern u16 sPort;

    // The P2P port the user wishes to use
    // TODO allow players to change this
    extern u16 sUserPort;

} // namespace Port
} // namespace Wiimmfi
