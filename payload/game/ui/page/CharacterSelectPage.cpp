#include <common/Common.hpp>
#include <game/ui/Page.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// CharacterSelectPage::onInit() patch
// Go to CC Select when exiting TT mode
kmWrite8(0x8083D603, Page::GP_CLASS_SELECT);
