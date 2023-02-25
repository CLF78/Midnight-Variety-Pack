#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.h>
#include <game/ui/ctrl/SheetSelectControl.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>
#include <game/ui/UIControlTimer.h>
#include "cupsystem/CupCounts.h"

// Expansion structures for the custom cup system
class RaceCupSelectArrow : public SheetSelectButton {
public:
    void RaceCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
    void RaceCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
};

class RaceCupSelectPageEx {
public:
    SheetSelectControl arrows;
    InputHandlerEx<SheetSelectButton, SheetSelectControl> leftHandler;
    InputHandlerEx<SheetSelectButton, SheetSelectControl> rightHandler;
    s32 curPage;
};

class RaceCupSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler<RaceCupSelectPage> onBtnClick;
    InputHandler<RaceCupSelectPage> onBtnSelect;
    InputHandler<RaceCupSelectPage> onBtnDeselect;
    InputHandler<RaceCupSelectPage> onBackPress;
    InputHandler<RaceCupSelectPage> onStart;

    u32 activePlayers;
    u32 activeControllers;

    CtrlMenuCupSelectCup cupHolder;
    u32 selectedButtonId;

    Page* voteOrRandomPage;
    UIControlTimer* timer;

    // Custom structures from here onwards
    #if (CUSTOM_CUP_COURSE_SUPPORT && RACE_CUP_ARROWS_ENABLED)
    RaceCupSelectPageEx extension;
    #endif
};

#if (CUSTOM_CUP_COURSE_SUPPORT && RACE_CUP_ARROWS_ENABLED)
size_assert(RaceCupSelectPage, 0x23E0 + sizeof(RaceCupSelectPageEx));
#else
size_assert(RaceCupSelectPage, 0x23E0);
#endif
