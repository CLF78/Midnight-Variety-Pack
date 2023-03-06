#include <kamek.h>
#include "cupsystem/CupManager.h"

// LoadCourse hooks
kmCall(0x80540820, CupManager::getTrackFilenameD);
kmCall(0x80540870, CupManager::getTrackFilename);
