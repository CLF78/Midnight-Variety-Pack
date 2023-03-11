#include <kamek.h>

// Region detection address and function
u16 detect:0x8000620A;

enum Region {
    REG_P,
    REG_E,
    REG_J,
    REG_K,
    REG_UNK,
};
