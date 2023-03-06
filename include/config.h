//////////////////////////////////////////////////
// Configuration File for Mario Kart Midnight 2 //
//////////////////////////////////////////////////

///////////
// Guide //
///////////

// Change the settings based on your needs.
// Compile the code.
// The end.

//////////////////////////
// Multi Archive System //
//////////////////////////

// Allows the game to look for resources in additional files, allowing
// for the introduction of asset changes without modifying the original files.
// Credits: stebler, Vabold (original code), CLF78 (porting)

// File suffix for distro-provided assets.
#define MULTI_ARCHIVE_DISTRO_SUFFIX "MKM"

// File suffix for user-provided assets.
#define MULTI_ARCHIVE_USER_SUFFIX "_Dif"

///////////////////////
// Custom Cup System //
///////////////////////

// The extensions required for supporting more/less than 8 cups for VS/Battle.
// Credits: CLF78 (original code), Ismy (cup button to/from cup position algorithm)

// Directories holding the tracks, inside the /Race/Course folder
#define TRACK_DIR_VS "/vs"
#define TRACK_DIR_BT "/bt"

// Directories holding the VS/Battle cup icons, inside the SZS file.
#define CUP_ICON_DIR_VS_MODERN "/cups/modern"
#define CUP_ICON_DIR_VS_RETRO "/cups/retro"
#define CUP_ICON_DIR_VS_VARIETY "/cups/variety"
#define CUP_ICON_DIR_BT "/cups/bt"

// BRCTR filenames for the custom cup selection arrows.
#define CUP_ARROW_L_BRCTR "CupSelectCupArrowLeft"
#define CUP_ARROW_R_BRCTR "CupSelectCupArrowRight"
