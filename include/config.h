//////////////////////////////////////////////////
// Configuration File for Mario Kart Midnight 2 //
//////////////////////////////////////////////////

///////////
// Guide //
///////////

// Change the settings to be ON/OFF (or alter their value) based on your needs.
// Compile the code.
// The end.

//////////////////
// DO NOT TOUCH //
//////////////////
#define ON true
#define OFF false

///////////////////////
// Unlock Everything //
///////////////////////

// Unlocks every character, vehicle, cup and class without touching the original savegame.
// Unlock screens are also disabled.
#define UNLOCK_SAVEGAME ON

//////////////////////////
// Multi Archive System //
//////////////////////////

// Allows the game to look for resources in additional files, allowing
// for the introduction of asset changes without modifying the original files.
#define MULTI_ARCHIVE_SYSTEM ON

// Add support for user-provided assets.
#define MULTI_ARCHIVE_ALLOW_USER_FILES ON

// File suffix for distro-provided assets.
#define MULTI_ARCHIVE_DISTRO_SUFFIX "MKM"

// File suffix for user-provided assets.
#define MULTI_ARCHIVE_USER_SUFFIX "_Dif"

///////////////////////
// Custom Cup System //
///////////////////////

// Enables the extensions required for supporting more/less than 8 cups.
// Required dependency: UNLOCK_SAVEGAME.
#define CUSTOM_CUP_SYSTEM ON

// Enables the cup system for race tracks.
#define CUSTOM_CUP_COURSE_SUPPORT ON

// Enables the cup system for battle arenas.
#define CUSTOM_CUP_BATTLE_SUPPORT ON

// Enables custom music slot support.
#define EXTENDED_MUSIC_SLOTS ON

// Enables random variant support.
#define RANDOM_TRACKS ON

// Enables defining random variant chances.
#define RANDOM_TRACKS_CHANCES ON

// Directory holding the tracks, inside the /Race/Course folder
#define TRACK_DIR_VS "/vs"
#define TRACK_DIR_BT "/bt"

// Directory holding the VS/Battle cup icons, inside the SZS file.
#define CUP_ICON_DIR_VS "/cups/vs"
#define CUP_ICON_DIR_BT "/cups/bt"

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
