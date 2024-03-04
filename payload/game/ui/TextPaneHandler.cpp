#include <common/Common.hpp>
#include <game/ui/MessageInfo.hpp>
#include <game/ui/TextMgr.hpp>

//////////////////////////////
// Conditional Text Display //
//////////////////////////////

// TextPaneHandler::setText() patch
// Allow using conditional escape sequences in all languages
kmWrite32(0x805CEB08, 0x60000000);

// TextPaneHandler::setText() patch
// Implement automatic space prepending for conditional escape sequences
kmHookFn u32 InsertSpace(bool cond, MessageEscapeSequence* sequence, MessageInfo* msgInfo) {

    // If the condition returned false, use the first message as is
    if (!cond)
        return sequence->settings[1];

    // If the message requires a prepended space, insert it
    if (msgInfo->prependSpace)
        TextMgr::instance->putChar(L' ');

    // Get the second message instead
    return sequence->settings[2];
}

// Glue code
kmBranchDefAsm(0x805CEB3C, 0x805CEB50) {
    nofralloc

    // Call C++ code
    mr r3, r5
    mr r4, r20
    mr r5, r17
    bl InsertSpace

    // Move result to r22 and return
    mr r22, r3
    blr
}
