#include <kamek.h>
#include "cupsystem/CupManager.h"
#if CUSTOM_CUP_SYSTEM

// LoadCourse hooks
kmCall(0x80540820, CupManager::getTrackFilenameD);
kmCall(0x80540870, CupManager::getTrackFilename);

#endif
