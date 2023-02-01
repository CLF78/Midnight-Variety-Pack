//////////////////////////////////////////////////
// Configuration File for Mario Kart Midnight 2 //
//////////////////////////////////////////////////

///////////
// Guide //
///////////

// Change the settings to be true/false (or alter their value) based on your needs.
// Compile the code.
// The end.

///////////////////////
// Unlock Everything //
///////////////////////

// Unlocks every character, vehicle, cup and class without touching the original savegame.
#define UNLOCK_SAVEGAME true

//////////////////////////
// Multi Archive System //
//////////////////////////

// Allows the game to look for resources in additional files, allowing both mod authors and
// the playerbase to introduce asset changes without modifying the original files.
#define MULTI_ARCHIVE_SYSTEM true

 // File suffix for distro-provided assets.
#define MULTI_ARCHIVE_DISTRO_SUFFIX "MKM"

 // File suffix for user-provided assets.
#define MULTI_ARCHIVE_USER_SUFFIX "_Dif"

///////////////////////
// Custom Cup System //
///////////////////////

// Enables all the extensions required for supporting more/less than 8 cups.
// Required dependencies: UNLOCK_SAVEGAME, MULTI_ARCHIVE_SYSTEM.
#define CUSTOM_CUP_SYSTEM true

// BRCTR filenames for the custom cup selection arrows.
#define CUP_ARROW_L_BRCTR "CupSelectCupArrowLeft"
#define CUP_ARROW_R_BRCTR "CupSelectCupArrowRight"

///////////////////////
// Dependency Checks //
///////////////////////

// DO NOT TOUCH ANYTHING BEYOND THIS COMMENT
#if (!UNLOCK_SAVEGAME && CUSTOM_CUP_SYSTEM)
#error "UNLOCK_SAVEGAME is required for CUSTOM_CUP_SYSTEM!"
#endif

#if (!MULTI_ARCHIVE_SYSTEM && CUSTOM_CUP_SYSTEM)
#error "MULTI_ARCHIVE_SYSTEM is required for CUSTOM_CUP_SYSTEM!"
#endif
