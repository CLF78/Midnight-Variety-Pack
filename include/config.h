//////////////////////////////////////////////////
// Configuration File for Mario Kart Midnight 2 //
//////////////////////////////////////////////////

// A Handy Guide:
// Change the settings to be true/false (or alter their value) based on your needs.
// Compile the code.
// The end.

// Unlock Everything
// Unlocks every character, vehicle, cup and class without touching the original savegame.
// Required by CUSTOM_CUP_SYSTEM.
#define UNLOCK_SAVEGAME true

// Custom Cup System
// Enables all the extensions required for supporting more/less than 8 cups.
// Requires UNLOCK_SAVEGAME to be true.
#define CUSTOM_CUP_SYSTEM true

// Dependency Checks
// DO NOT TOUCH ANYTHING BEYOND THIS COMMENT
#if (!UNLOCK_SAVEGAME && CUSTOM_CUP_SYSTEM)
#error "UNLOCK_SAVEGAME is required for CUSTOM_CUP_SYSTEM!"
#endif
