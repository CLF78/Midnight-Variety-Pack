#pragma once

namespace nw4r {
namespace db {

struct ConsoleHead {

    enum Attributes {
        CONSOLE_ATTR_TAB_SIZE_2          = BIT_FLAG(-1),
        CONSOLE_ATTR_NO_TURNOVER_LINE    = BIT_FLAG(0), // Do not wrap overflowing lines
        CONSOLE_ATTR_STOP_ON_BUFFER_FULL = BIT_FLAG(1), // Stop updating when buffer is full
        CONSOLE_ATTR_TAB_SIZE_4          = BIT_FLAG(2),
        CONSOLE_ATTR_TAB_SIZE_8          = BIT_FLAG(3),
    };

    u8* textBuffer;
    u16 width; // Max characters per line
    u16 height; // Max character lines
    u16 priority;
    u16 attributes;

    u16 currLineNumber; // Current line to be printed
    u16 currLinePosX; // Current character on current line to be printed

    u16 ringTop;
    PAD(2);
    s32 ringTopLineCnt;
    s32 topLineNumber; // Line number of first line displayed on screen

    s16 displayPosX;
    s16 displayPosY;
    u16 displayLineCount; // Lines to display at once

    bool isVisible;
    PAD(1);

    void* textWriter;
    ConsoleHead* next;
};

s32 Console_GetTotalLines(ConsoleHead* console);
void Console_DrawDirect(ConsoleHead* console);

} // namespace db
} // namespace nw4r
