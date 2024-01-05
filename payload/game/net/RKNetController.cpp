#include <common/Common.hpp>
#include <wiimmfi/Status.hpp>

/////////////////////
// Wiimmfi Patches //
/////////////////////

// Hook network shutdown to do some cleanup (not ported)
kmListHookDefCpp(NetShutdownHook) {

    // Delete the Wiimmfi messaging token
    if (Wiimmfi::Status::token)
        delete Wiimmfi::Status::token;
}
