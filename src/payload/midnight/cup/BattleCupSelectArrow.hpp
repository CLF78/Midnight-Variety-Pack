#include <common/Common.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>

class BattleCupSelectArrow : public SheetSelectButton {
public:
    void onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
    void onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
};
