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

// Directories holding the VS/Battle cup icons, inside the SZS file.
#define CUP_ICON_DIR_VS_MODERN "/cups/modern"
#define CUP_ICON_DIR_VS_RETRO "/cups/retro"
#define CUP_ICON_DIR_VS_VARIETY "/cups/variety"
#define CUP_ICON_DIR_BT "/cups/battle"

// BRCTR filenames for the custom cup selection arrows.
#define CUP_ARROW_L_BRCTR "CupSelectCupArrowLeft"
#define CUP_ARROW_R_BRCTR "CupSelectCupArrowRight"

// The pitch multiplier for final lap music
#define FINAL_LAP_MUSIC_PITCH_MULTIPLIER 1.2f

///////////////////////
// Exception Handler //
///////////////////////

// Displays debug information when the game crashes.
// Credits: CLF78 (original code), Star (return to menu functionality)

// The distribution's name
#define EXCEPTION_HANDLER_DISTRO_NAME "Mario Kart Midnight 2"

// The point of contact for users who encounter a crash
#define EXCEPTION_HANDLER_SUPPORT_ADDRESS "CLF78#1139 on Discord"

////////////////////////////
// SFX Replacement System //
////////////////////////////

// Allows to replace SFX with external BRSTMS.
// Credits: stebler (original code), CLF78 (porting)

// Bit used to indicate replaced SFX
#define SASR_BIT (1 << 31)

// The directory where custom SFX is stored
#define SASR_SFX_DIR "/sound/sfx"

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// Allows to store additional savegame data outside the regular save.
// Credits: CLF78 (original code)

// The savegame filename
#define SAVEEX_FILENAME "midnight.dat"

// The savegame magic
#define SAVEEX_MAGIC 'NITE'

// The savegame version number
#define SAVEEX_VERSION_NUMBER 1
