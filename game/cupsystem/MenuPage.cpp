#include <kamek.h>

// Prevent the game from rewriting the last selected course/stage
kmWrite32(0x80837DB0, 0x4800007C);
