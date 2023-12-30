#include <common/Common.hpp>
#include <game/ui/GlobalContext.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////////////////
// Patches for Custom Cup System //
///////////////////////////////////

// Set the default track and arena to -1
kmWrite16(0x805E4218, 0x9123);
kmWrite16(0x805E421C, 0x9123);

// Set the pointers to the tracklists when GlobalContext is constructed
kmBranchDefCpp(0x805E2FF4, NULL, GlobalContext*, GlobalContext* self) {

    // This does not need a destructor since it's never reached and this field is never reset
    self->trackOrder = CupManager::trackOrder;
    self->arenaOrder = CupManager::arenaOrder;
    return self;
}
