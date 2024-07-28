#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>

class RaceCupSelectArrow : public SheetSelectButton {
public:
    void onLeft(SheetSelectControl* arrowPair, u32 localPlayerId);
    void onRight(SheetSelectControl* arrowPair, u32 localPlayerId);

    void onDeselect(u32 localPlayerId);
};
