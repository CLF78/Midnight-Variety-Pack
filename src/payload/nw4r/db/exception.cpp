#include <common/Common.hpp>
#include <nw4r/db/exception.hpp>
#include <midnight/Region.hpp>
#include <revolution/base/PPCArch.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSError.h>
#include <revolution/os/OSLoMem.h>

///////////////////////
// Exception Handler //
///////////////////////

namespace nw4r {
namespace db {

bool IsValidStackAddr(u32 addr) {

    // Ignore misaligned addresses
    if (addr & 3)
        return false;

    // Use the secure boundaries from low memory to support expanded memory
    return ((addr >= 0x80000000 && addr <= __OSBootInfo2.MEM1SecureBoundary) ||
            (addr >= 0x90000000 && addr <= __OSBootInfo2.MEM2SecureBoundary));
}

bool IsPayloadAddr(u32 addr) {
    return (addr >= (u32)__PayloadStart && addr <= (u32)__PayloadEnd);
}

void ShowMainInfo_(OSError error, const OSContext* context, u32 dsisr, u32 dar) {

    switch(error) {
        case OS_ERROR_DSI:
            Exception_Printf_("Invalid access at address %08X", context->srr0);
            break;
    
        case OS_ERROR_ISI:
            Exception_Printf_("Jump to invalid address %08X", context->srr0);
            break;

        case OS_ERROR_ALIGNMENT:
            Exception_Printf_("Unaligned access at address %08X", context->srr0);
            break;

        case OS_ERROR_PROGRAM:
            Exception_Printf_("Incorrect instruction at address %08X", context->srr0);
            break;

        case OS_ERROR_FPE: {
            // Initial print
            Exception_Printf_("Float error (");

            // Mask disabled exceptions
            u32 fpscr = context->fpscr;
            u32 mask = (fpscr & (FPSCR_XE | FPSCR_ZE | FPSCR_UE | FPSCR_OE | FPSCR_VE)) << 16;

            // Mask all remaining unrelated bits and apply the result
            mask |= (FPSCR_VXCVI | FPSCR_VXSQRT | FPSCR_VXSOFT | FPSCR_VXVC | FPSCR_VXIMZ | FPSCR_VXZDZ |
                     FPSCR_VXIDI | FPSCR_VXISI | FPSCR_VXSNAN);
            fpscr &= mask;

            // Check for invalid operations
            if (fpscr & FPSCR_VX) {
                if (fpscr & FPSCR_VXSNAN)
                    Exception_Printf_("sNaN");
                else if (fpscr & FPSCR_VXISI)
                    Exception_Printf_("Infinity - Infinity");
                else if (fpscr & FPSCR_VXIDI)
                    Exception_Printf_("Infinity / Infinity");
                else if (fpscr & FPSCR_VXZDZ)
                    Exception_Printf_("0 / 0");
                else if (fpscr & FPSCR_VXIMZ)
                    Exception_Printf_("Infinity * 0");
                else if (fpscr & FPSCR_VXVC)
                    Exception_Printf_("Invalid compare");
                else if (fpscr & FPSCR_VXSOFT)
                    Exception_Printf_("Software request");
                else if (fpscr & FPSCR_VXSQRT)
                    Exception_Printf_("Invalid square root");
                else if (fpscr & FPSCR_VXCVI)
                    Exception_Printf_("Invalid integer convert");

            // Check for other errors
            } else if (fpscr & FPSCR_OX)
                Exception_Printf_("Overflow");
            else if (fpscr & FPSCR_UX)
                Exception_Printf_("Underflow"); 
            else if (fpscr & FPSCR_ZX)
                Exception_Printf_("Division by zero"); 
            else if (fpscr & FPSCR_XX)
                Exception_Printf_("Inexact result");

            // Print address
            Exception_Printf_(") at %08X", context->srr0); }
            break;

        default:
            static const char* sExceptionNames[] = {
                "System Reset",
                "Machine Check",
                "", // DSI, replaced
                "", // ISI, replaced
                "External Interrupt",
                "", // Alignment, replaced
                "", // Program, replaced
                "FPU Unavailable",
                "Decrementer",
                "System Call",
                "Trace",
                "Performance Monitor",
                "Breakpoint",
                "System Management Interrupt",
                "Thermal Interrupt",
                "Protection",
                "" // Floating Point, replaced
            };

            Exception_Printf_("%s exception at %08X", sExceptionNames[error], context->srr0);
    }

    if (IsPayloadAddr(context->srr0))
        Exception_Printf_(" (P)\n");
    else
        Exception_Printf_("\n");
}

void ShowGPR_(const OSContext* context) {

    // Print heading
    Exception_Printf_("Registers:\n");

    // Print 3 registers per line
    for (int reg = 0; reg < 10; reg++)
        Exception_Printf_("r%d  %08X  r%d %08X  r%d %08X\n", reg, context->gpr[reg],
                                                             reg + 11, context->gpr[reg + 11],
                                                             reg + 22, context->gpr[reg + 22]);

    // Print the final line
    Exception_Printf_("r%d %08X  r%d %08X\n", 10, context->gpr[10], 21, context->gpr[21]);
}

void ShowStackTrace_(u32* sp) {

    // Print heading
    Exception_Printf_("Last function calls:\n");

    // Print 4 addresses per line to save space
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

// Make the exception handler more user friendly
REPLACE void PrintContext_(OSError error, const OSContext* context, u32 dsisr, u32 dar) {
    static const char SEP_LINES[] = "--------------------------------\n";

    // Print heading
    Exception_Printf_(SEP_LINES);
    Exception_Printf_(EXCEPTION_HANDLER_DISTRO_NAME " Exception Handler (%s)\n", Region::GetIdentifier());
    Exception_Printf_("Please take a picture of the information below and send it to " EXCEPTION_HANDLER_SUPPORT_ADDRESS ".");
    Exception_Printf_(" Use the D-Pad/Circle Pad to scroll through the data or press HOME/START to return to the System Menu.\n");

    // Print main information
    Exception_Printf_(SEP_LINES);
    ShowMainInfo_(error, context, dsisr, dar);

    // Print registers
    Exception_Printf_(SEP_LINES);
    ShowGPR_(context);

    // Print stack trace
    Exception_Printf_(SEP_LINES);
    ShowStackTrace_((u32*)context->gpr[1]);

    // Print a dashed line to end the display
    Exception_Printf_(SEP_LINES);
}

} // namespace db
} // namespace nw4r
