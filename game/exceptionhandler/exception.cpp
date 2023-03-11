#include <kamek.h>
#include <nw4r/db/exception.h>
#include <rvl/ax/ax.h>
#include <rvl/os/OSContext.h>
#include <rvl/os/OSError.h>
#include <rvl/os/OSThread.h>
#include <game/system/SystemManager.h>
#include "exceptionhandler/exception.h"

// Use these to determine if the exception was triggered by the payload
#define PAYLOAD_START 0x808DD400
#define PAYLOAD_END 0x809A0000

// Strings
extern char dashes;
const char* regStrings[] = {"PAL", "USA", "JAP", "KOR", "UNK"};

// Helper functions
const char* getRegionString() {

    // Use instruction to detect region
    u32 region;
    switch (detect) {
        case 0x54A9: region = REG_P; break;
        case 0x5409: region = REG_E; break;
        case 0x53CD: region = REG_J; break;
        case 0x5511: region = REG_K; break;
        default: region = REG_UNK; break;
    }

    return regStrings[region];
}

bool IsValidStackAddr(u32 addr) {
    return ((addr >= 0x80000000 && addr <= 0x817FFFFF) || (addr >= 0x90000000 && addr <= 0x935E0000));
}

bool IsPayloadAddr(u32 addr) {
    return (addr >= PAYLOAD_START && addr <= PAYLOAD_END);
}

void ReturnToMenu() {
    if (OSGetCurrentThread())
        SystemManager::returnToMenu();
}

extern "C" static void printStackTrace(u32* sp) {
    for (u32 i = 0; i < 16; i++) {

        // Check if stack address is valid, if not break
        // Print a newline character if the current line isn't finished
        if (!IsValidStackAddr((u32)sp)) {
            if (i % 4 != 0)
                nw4r::db::Exception_Printf_("\n");
            break;
        }

        // Get the address
        u32 addr = sp[1] - 4;

        // Add an arrow between addresses
        if (i % 4 != 0)
            nw4r::db::Exception_Printf_(" -> ");

        // Print the address and if it belongs to the payload
        nw4r::db::Exception_Printf_("%08X", addr);
        if (IsPayloadAddr(addr))
            nw4r::db::Exception_Printf_(" (P)");

        // Go to a new line if 4 addresses have been printed
        if (i % 4 == 3)
            nw4r::db::Exception_Printf_("\n");

        // Get caller sp
        sp = (u32*)*sp;
    }
}

// Force display the exception handler
kmWrite16(0x802264E8, 0x4800);

// Replace initial text
kmBranchDefCpp(0x800236B0, 0x800236E8, void, u16 error, OSContext* context, u32 dsisr, u32 dar) {
    nw4r::db::Exception_Printf_(EXCEPTION_HANDLER_DISTRO_NAME " Exception Handler [%s]\n", getRegionString());
    nw4r::db::Exception_Printf_("Please take a picture of the following information\nand send it to " EXCEPTION_HANDLER_SUPPORT_ADDRESS ". Use the\n");
    nw4r::db::Exception_Printf_("D-pad to scroll through the data, or press HOME to\n");
    nw4r::db::Exception_Printf_("return to the Wii Menu.\n");
}

// Replace heading with simple dashes
kmCallDefCpp(0x800236FC, const char*) {
    return &dashes;
}

// Override main info
kmCallDefCpp(0x80023718, void, u16 error, OSContext* context, u32 dsisr, u32 dar) {

    // For DSI and ISI exceptions, use the SRR0 and determine if the address is in the payload
    // For any other exception, fall back to original call
    u32 srr0 = context->srr0;
    const char* p = (IsPayloadAddr(srr0)) ? " (P)" : "";
    if (error == OS_ERROR_DSI)
        nw4r::db::Exception_Printf_("Invalid load/store operation at %08X%s\n", srr0, p);
    else if (error == OS_ERROR_ISI)
        nw4r::db::Exception_Printf_("Invalid branch to %08X%s\n", srr0, p);
    else
        nw4r::db::ShowMainInfo_(error, context, dsisr, dar);
}

// Force register print
kmWrite32(0x8002372C, 0x60000000);

// Replace register head
kmBranchDefCpp(0x80023730, 0x80023740, const char*) {
    nw4r::db::Exception_Printf_(&dashes);
    return "Registers:\n";
}

// Replace register format string
kmCallDefCpp(0x8002375C, const char*) {
    return " r%d %08X  r%d %08X  r%d %08X\n";
}

// Replace ending register format string
kmCallDefCpp(0x8002379C, void, const char*, u32 reg1, u32 regVal1, u32 reg2, u32 regVal2) {
    nw4r::db::Exception_Printf_("r%d %08X  r%d %08X\n", reg1, regVal1, reg2, regVal2);
    nw4r::db::Exception_Printf_(&dashes);
}

// Replace stack trace head
kmCallDefCpp(0x8002387C, const char*) {
    return "Last function calls:\n";
}

// Replace stack trace (wrapper)
kmBranchDefAsm(0x8002388C, 0x80023950) {
    nofralloc

    // Call C++ function
    mr r3, r29
    bl printStackTrace
    blr
}

// Mute audio on crash
kmCallDefAsm(0x80023268) {
    nofralloc

    // Original instruction
    mr r28, r3

    // Mute audio
    li r3, 0
    b AXSetMasterVolume
}

// Modify existing call to return to menu on HOME button press
kmWrite32(0x80226648, 0x48000018);
kmWritePointer(0x80386E4C, ReturnToMenu);

// Disable scrolling to the sides
kmWrite32(0x802266CC, 0x48000030);

// Hardcode total line count
kmCallDefCpp(0x80226588, u32) {
    return 9;
}

// Disable initial automatic scroll
kmWrite8(0x8022659F, 0x48);
