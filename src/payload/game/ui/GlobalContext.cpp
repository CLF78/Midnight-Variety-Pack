#include <common/Common.hpp>
#include <game/ui/GlobalContext.hpp>
#include <mvp/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// GlobalContext::GlobalContext() patch
// Insert the custom track/arena order arrays into the class
kmBranchDefCpp(0x805E2FF4, NULL, GlobalContext*, GlobalContext* self) {

    // This does not need a destructor since it's never reached and this field is never reset
    self->trackOrder = CupManager::trackOrder;
    self->arenaOrder = CupManager::arenaOrder;
    return self;
}

// GlobalContext::changeLicense() patches
// Set the default track and arena to -1
kmWrite16(0x805E4218, 0x9123);
kmWrite16(0x805E421C, 0x9123);
