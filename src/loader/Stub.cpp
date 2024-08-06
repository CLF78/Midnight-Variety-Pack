#include "Loader.hpp"
#include <platform/string.h>
#include <revolution/base/PPCReg.h>
#include <revolution/os/OSLoMem.h>

const Loader::Functions functions[] = {
    {(OSReport_t) 0x801A25D0,
    (OSFatal_t) 0x801A4EC4,
    (DVDConvertPathToEntrynum_t) 0x8015DF4C,
    (DVDFastOpen_t) 0x8015E254,
    (DVDReadPrio_t) 0x8015E834,
    (DVDClose_t) 0x8015E568,
    (vsnprintf_t) 0x8001182C,
    (RKSystem*) 0x802A4080,
    'P'},

    {(OSReport_t) 0x801A2530,
    (OSFatal_t) 0x801A4E24,
    (DVDConvertPathToEntrynum_t) 0x8015DEAC,
    (DVDFastOpen_t) 0x8015E1B4,
    (DVDReadPrio_t) 0x8015E794,
    (DVDClose_t) 0x8015E4C8,
    (vsnprintf_t) 0x80010CCC,
    (RKSystem*) 0x8029FD00,
    'E'},

    {(OSReport_t) 0x801A24F0,
    (OSFatal_t) 0x801A4DE4,
    (DVDConvertPathToEntrynum_t) 0x8015DE6C,
    (DVDFastOpen_t) 0x8015E174,
    (DVDReadPrio_t) 0x8015E754,
    (DVDClose_t) 0x8015E488,
    (vsnprintf_t) 0x80011750,
    (RKSystem*) 0x802A3A00,
    'J'},

    {(OSReport_t) 0x801A292C,
    (OSFatal_t) 0x801A5220,
    (DVDConvertPathToEntrynum_t) 0x8015DFC4,
    (DVDFastOpen_t) 0x8015E2CC,
    (DVDReadPrio_t) 0x8015E8AC,
    (DVDClose_t) 0x8015E5E0,
    (vsnprintf_t) 0x80011894,
    (RKSystem*) 0x80292080,
    'K'},
};

char path[] = KAMEK_FILE_PATH "X.bin";

// StrapScene::linkModule() patch
// Load the custom code from disc
kmBranchDefCpp(0x800074D4, NULL, void) {

    // Detect region, stall game on failure
    Region::Value region = Region::Detect();
    if (region == Region::REGION_UNK)
        for (;;);

    // Get the functions and set up logging
    const Loader::Functions* funcs = &functions[region];
    Logger::CreateInstance(funcs->OSReport, funcs->OSFatal, funcs->vsnprintf);

    // Get code file and load it
    path[strlenc(KAMEK_FILE_PATH)] = funcs->identifier;
    Loader::LoadFromDisc(funcs, path);
}

// __start() patch
// Reset the DSP so the SDK can properly initialize it (replaces __init_data)
kmCallDefCpp(0x800060BC, void) {
    __AIRegs[0] = 0;
}

// __start() patch
// Initial hook for patches that need to be applied at game boot (replaces __get_debug_bba)
kmCallDefCpp(0x800061EC, BOOL) {

    // Disable the codehandler by BLRing the first instruction
    // This is not tamper-proof but it should be enough to deter any idiot that would cheat...
    if (__LOG_LEVEL__ != Logger::DEBUG)
        __CodeHandlerStart = 0x4E800020;

    // Return original value
    return FALSE;
}
