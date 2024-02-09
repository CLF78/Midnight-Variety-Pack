#include <common/Common.hpp>
#include <revolution/base/PPCReg.h>
#include <revolution/os/OSLoMem.h>
#include <midnight/Region.hpp>
#include <Loader.hpp>

const Loader::Functions functions[] = {
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

char path[] = KAMEK_FILE_PATH "X.bin";

// Loader insertion hook
kmBranchDefCpp(0x800074D4, NULL, void) {

    // Detect region, stall game on failure
    Region::Value region = Region::Detect();
    if (region == Region::REGION_UNK)
        for (;;);

    // Get the functions
    const Loader::Functions* funcs = &functions[region];

    // Get code file and load it
    path[sizeof(KAMEK_FILE_PATH)-1] = funcs->identifier;
    Loader::LoadFromDisc(funcs, path);
}

// Reset the DSP so the SDK can properly initialize it
kmCallDefCpp(0x800060BC, void) {
    __AIRegs[0] = 0;
}

// Initial hook for patches that need to be applied at game boot
// Replaces the __get_debug_bba call
kmCallDefCpp(0x800061EC, u32) {

    // Disable the codehandler by BLRing the first instruction
    // This is not tamper-proof but it should be enough to deter any idiot that would cheat...
    __CodeHandlerStart = 0x4E800020;

    // Return 0 to match original call
    return 0;
}
