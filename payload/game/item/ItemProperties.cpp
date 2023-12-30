#include <common/Common.hpp>
#include <game/item/ItemProperties.hpp>

//////////////////////////////////////
// Patches for Draggable Blue Shell //
//////////////////////////////////////

// Remove use function so Blue Shells can be dragged
kmListHookDefCpp(RaceStartHook) {
    ItemProperties::itemPropertiesTable[ItemProperties::ITEM_BLUE_SHELL].useFunction = nullptr;
}
