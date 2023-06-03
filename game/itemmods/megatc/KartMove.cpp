#include <kamek.h>
#include <game/kart/KartMove.h>

// Apply Mega Mushroom effect instead of the Lightning one
kmCallDefCpp(0x80798834, void, KartMove* self) {
    self->activateMega();
}
