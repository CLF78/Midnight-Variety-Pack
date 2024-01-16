#include <common/Common.hpp>
#include <gs/gp/gp.h>
#include <gs/gp/gpiBuddy.h>
#include <platform/string.h>
#include <wiimmfi/Kick.hpp>

//////////////////////////////////////
// Patches for Wiimmfi Kick Support //
//////////////////////////////////////

// Parse Wiimmfi kick messages
kmCallDefCpp(0x800FCEE8, int, GPConnection conn, char* data) {

    // Reproduce skipped code
    if (strstartw(data, "\\bsi\\"))
        return gpiProcessRecvBuddyStatusInfo(conn, data);

    // Call Wiimmfi code
    return Wiimmfi::Kick::ParseKickMessage(conn, data);
}

// Glue code
kmBranch(0x800FCEC8, 0x800FCEE0);
