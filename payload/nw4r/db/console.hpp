#include <common/Common.hpp>

namespace nw4r {
namespace db {

struct ConsoleHead {

    enum Attributes {
        CONSOLE_ATTR_NO_TURNOVER_LINE = 0x1, // Do not wrap overflowing lines
        CONSOLE_ATTR_STOP_ON_BUFFER_FULL = 0x2, // Stop updating when buffer is full
        CONSOLE_ATTR_TAB_SIZE_2 = 0x0,
        CONSOLE_ATTR_TAB_SIZE_4 = 0x4,
        CONSOLE_ATTR_TAB_SIZE_8 = 0x8,
    };

    u8* textBuffer;
    u16 width; // Max characters per line
    u16 height; // Max character lines
    u16 priority;
    u16 attributes;

    u16 currLineNumber; // Current line to be printed
    u16 currLinePosX; // Current character on current line to be printed

    u16 ringTop;
    s32 ringTopLineCnt;
    s32 topLineNumber; // Line number of first line displayed on screen

    s16 displayPosX;
    s16 displayPosY;
    u16 displayLineCount; // Lines to display at once
    
    bool isVisible;
    u8 pad;
    
    void* textWriter;
    ConsoleHead* next;
};

s32 Console_GetTotalLines(ConsoleHead* console);

void Console_DrawDirect(ConsoleHead* console);

} // namespace db
} // namespace nw4r
