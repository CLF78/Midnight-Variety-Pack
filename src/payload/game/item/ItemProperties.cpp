#include <common/Common.hpp>
#include <game/item/ItemProperties.hpp>

//////////////////////////
// Draggable Blue Shell //
//////////////////////////

// Remove use function so Blue Shells can be dragged
// Credits: MrBean35000vr
kmListHookDefCpp(RaceStartHook) {
    ItemProperties::itemPropertiesTable[ItemProperties::ITEM_BLUE_SHELL].useFunction = nullptr;
}
