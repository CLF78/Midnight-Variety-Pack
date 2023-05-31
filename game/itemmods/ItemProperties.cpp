#include <kamek.h>
#include <game/item/ItemProperties.h>

// Do item replacements
kmBranchDefCpp(0x807BD1CC, NULL, void) {

    // Draggable Blue Shell
    ItemProperties::itemPropertiesTable[ItemProperties::ITEM_BLUE_SHELL].useFunction = NULL;

    // TODO Countdown Item Changes:
    // Mushroom -> Mushroom Green
    // Triple Mushroom -> Triple FIB
    // Thundercloud -> Mushroom Bomb
    // NOTE: chance changes required
}
