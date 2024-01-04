#include <common/Common.hpp>
#include <wiimmfi/Status.hpp>

// Hook network shutdown to do some cleanup
kmListHookDefCpp(NetShutdownHook) {

    // Delete the Wiimmfi messaging token
    if (Wiimmfi::Status::token)
        delete Wiimmfi::Status::token;
}
