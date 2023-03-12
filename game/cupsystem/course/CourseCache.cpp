#include <kamek.h>
#include "cupsystem/CupManager.h"

// Load the next track for online and GP mode
kmCall(0x80541BC4, CupManager::getTrackFilename);
