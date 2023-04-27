#include <kamek.h>

// Prevent BRSTMs from cutting off when looping
kmCallDefAsm(0x800912D4) {
    nofralloc

    lis r31, 0x7FFF
    ori r31, r31, 0xFFFF
    blr
}
