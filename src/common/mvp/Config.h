#pragma once

////////////////////////////////////////////////////
// Configuration File for Midnight x Variety Pack //
////////////////////////////////////////////////////

///////////
// Guide //
///////////

// Change the settings based on your needs.
// Compile the code.
// The end.

//////////////////////
// Main Information //
//////////////////////

// The distribution's name.
#define DISTRO_NAME "Midnight x Variety Pack"

// The distribution's version.
#define DISTRO_VERSION "WIP"

//////////////////
// Kamek Loader //
//////////////////

// Loads the custom code in the game.
// Credits: Ninji (original code), Melg (ported to MKW), CLF78 (various updates)

// Path and filename of the code files.
// The region letter and the ".bin" extension are appended automatically.
#define KAMEK_FILE_PATH "/code"

// Root directory of the distribution files (used for error messages and debugging)
#define KAMEK_ROOT_DIR "/mvp"

//////////////////////////
// Multi Archive System //
//////////////////////////

// Allows the game to look for resources in additional files, allowing
// for the introduction of asset changes without modifying the original files.
// Credits: stebler, Vabold (original code), CLF78 (various updates)

// File suffix for distro-provided assets.
#define MULTI_ARCHIVE_DISTRO_SUFFIX "MVP"

// File suffix for user-provided assets.
#define MULTI_ARCHIVE_USER_SUFFIX "_Dif"

///////////////////////
// Custom Cup System //
///////////////////////

// The extensions required for supporting more/less than 8 cups for VS/Battle.
// Credits: CLF78 (original code), Ismy (cup button <=> cup position algorithm)

// Directories holding the VS/Battle cup icons, inside the SZS file.
#define CUP_ICON_DIR_VS_MIDNIGHT "/cups/midnight"
#define CUP_ICON_DIR_VS_NOSTALGIA "/cups/nostalgia"
#define CUP_ICON_DIR_VS_FUSION "/cups/fusion"
#define CUP_ICON_DIR_BT "/cups/battle"

// Online regions for each tracklist
#define ONLINE_REGION_VS_MIDNIGHT 609
#define ONLINE_REGION_VS_NOSTALGIA 753
#define ONLINE_REGION_VS_FUSION 891
#define ONLINE_REGION_BT ONLINE_REGION_VS_MIDNIGHT
#define LOGIN_REGION ONLINE_REGION_VS_MIDNIGHT

// BRCTR filenames for the custom cup selection arrows.
#define CUP_ARROW_L_BRCTR "CupSelectCupArrowLeft"
#define CUP_ARROW_R_BRCTR "CupSelectCupArrowRight"

// The pitch multiplier for final lap music.
#define FINAL_LAP_MUSIC_PITCH_MULTIPLIER 1.2f

// The scores required for reaching 3/2/1 stars.
#define GP_SCORE_3_STARS 60
#define GP_SCORE_2_STARS 57
#define GP_SCORE_1_STAR 55

///////////////////////
// Exception Handler //
///////////////////////

// Displays debug information when the game crashes.
// Note: The printed text is wrapped automatically, so adding newlines is not required.
// Credits: Star, Vabold (original code), CLF78 (various updates)

// The point of contact for users who encounter a crash.
#define EXCEPTION_HANDLER_SUPPORT_ADDRESS "CLF78 on Discord"

////////////////////////////
// SFX Replacement System //
////////////////////////////

// Allows to replace SFX with external BRSTMS.
// Credits: stebler (original code), CLF78 (various updates)

// The directory where custom SFX is stored
#define SASR_SFX_DIR "/sound/sfx"

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// Allows to store additional save data outside the regular save.
// Credits: CLF78 (original code)

// The savegame filename
#define SAVEEX_FILENAME "mvp.dat"

// The savegame magic
#define SAVEEX_MAGIC 'NITE'

// The savegame version number
#define SAVEEX_VERSION_NUMBER 1

// The savegame license magic
#define SAVEEX_LICENSE_MAGIC 'LCNS'

// The savegame license count
#define SAVEEX_LICENSE_COUNT 4

///////////////////////////
// Custom Engine Classes //
///////////////////////////

// Allows to modify and add class engines
// Credits: MrBean35000vr, stebler, _tZ (original code), CLF78 (various updates)

// The battle speed modifiers
#define CC_BATTLE_SPEED_MODIFIER 0.7f
#define CC_50_SPEED_MODIFIER 0.8f
#define CC_100_SPEED_MODIFIER 0.9f
#define CC_150_SPEED_MODIFIER 1.0f
#define CC_200_SPEED_MODIFIER 1.5f
#define CC_500_SPEED_MODIFIER 2.0f

/////////////////////////
// Time Limit Modifier //
/////////////////////////

// Allows to modify the default time limit
// Credits: Chadderz (original code), CLF78 (various updates)

// The default time limit
#define DEFAULT_ONLINE_TIME_LIMIT 5 * 60 * 1000
