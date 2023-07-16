#include <kamek.h>
#include "kamekLoader.h"

// Defines
#define directWriteBranch(addr, dest, lk) *(addr)=((((u32)(dest)-(u32)(addr)) & 0x3FFFFFC) | 0x48000000 | !!(lk))

// Enums
enum Region {
    REGION_P,
    REGION_E,
    REGION_J,
    REGION_K,
    REGION_COUNT,
};

// Loader data
u16 detect:0x8000620A;
u32 relHook:0x800074D4;
vu32 aiControl:0xCD006C00;

char path[] = "/codeX.bin";

const loaderFunctions functions[REGION_COUNT] = {
    {(OSReport_t) 0x801A25D0,
    (OSFatal_t) 0x801A4EC4,
    (DVDConvertPathToEntrynum_t) 0x8015DF4C,
    (DVDFastOpen_t) 0x8015E254,
    (DVDReadPrio_t) 0x8015E834,
    (DVDClose_t) 0x8015E568,
    (snprintf_t) 0x80011938,
    (RKSystem*) 0x802A4080,
    'P'},

    {(OSReport_t) 0x801A2530,
    (OSFatal_t) 0x801A4E24,
    (DVDConvertPathToEntrynum_t) 0x8015DEAC,
    (DVDFastOpen_t) 0x8015E1B4,
    (DVDReadPrio_t) 0x8015E794,
    (DVDClose_t) 0x8015E4C8,
    (snprintf_t) 0x80010DD8,
    (RKSystem*) 0x8029FD00,
    'E'},

    {(OSReport_t) 0x801A24F0,
    (OSFatal_t) 0x801A4DE4,
    (DVDConvertPathToEntrynum_t) 0x8015DE6C,
    (DVDFastOpen_t) 0x8015E174,
    (DVDReadPrio_t) 0x8015E754,
    (DVDClose_t) 0x8015E488,
    (snprintf_t) 0x8001185C,
    (RKSystem*) 0x802A3A00,
    'J'},

    {(OSReport_t) 0x801A292C,
    (OSFatal_t) 0x801A5220,
    (DVDConvertPathToEntrynum_t) 0x8015DFC4,
    (DVDFastOpen_t) 0x8015E2CC,
    (DVDReadPrio_t) 0x8015E8AC,
    (DVDClose_t) 0x8015E5E0,
    (snprintf_t) 0x800119A0,
    (RKSystem*) 0x80292080,
    'K'},
};

void unknownVersion() {
    // Can't do much here!
    // We can't output a message on screen without a valid OSFatal address
    for (;;);
}

// Region detection function
u32 regionDetect() {

    // Use instruction to detect region
    switch (detect) {
        case 0x54A9: return REGION_P;
        case 0x5409: return REGION_E;
        case 0x53CD: return REGION_J;
        case 0x5511: return REGION_K;
        default: unknownVersion(); return REGION_P;
    }
}

// Loader Stage 2
// Load and apply Kamek binary
u32 loaderStage2(u32 ret) {

    // Detect region and get functions
    const loaderFunctions* funcs = &functions[regionDetect()];

    // Load file
    path[5] = funcs->identifier;
    loadKamekBinaryFromDisc(funcs, path);

    // Return original value
    return ret;
}

// Loader Stage 1
// Run pre-Kamek patches and insert stage 2
void loaderStage1() {

    // Reset the DSP to fix libogc clownery
    aiControl = 0;

    // Insert stage 2 hook
    directWriteBranch(&relHook, &loaderStage2, false);
    cacheInvalidateAddress(&relHook);
}

// Loader Stage 0
// Replace call to __init_data with Stage 1 - this is applied by Riivolution
kmCall(0x800060BC, loaderStage1);
