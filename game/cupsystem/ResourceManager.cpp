#include <kamek.h>
#include "cupsystem/CupManager.h"

// Load the track file
kmCall(0x80540820, CupManager::getTrackFilenameD);
kmCall(0x80540870, CupManager::getTrackFilename);
