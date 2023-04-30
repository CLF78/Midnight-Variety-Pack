#include <kamek.h>
#include <game/ui/Page.h>

// Change the previous page to be CC Select when exiting TT mode
kmWrite8(0x8083D603, Page::GP_CLASS_SELECT);
