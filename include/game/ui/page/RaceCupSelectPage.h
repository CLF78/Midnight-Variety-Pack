#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.h>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.h>
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
    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32> leftHandler;
    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32> rightHandler;
    s32 curPage;
};

class RaceCupSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler0<RaceCupSelectPage, void> onBtnClick;
    InputHandler0<RaceCupSelectPage, void> onBtnSelect;
    InputHandler0<RaceCupSelectPage, void> onBtnDeselect;
    InputHandler0<RaceCupSelectPage, void> onBackPress;
    InputHandler0<RaceCupSelectPage, void> onStart;

    u32 activePlayers;
    u32 activeControllers;

    CtrlMenuCupSelectCup cupHolder;
    CtrlMenuCupSelectCourse courseHolder;
    u32 selectedButtonId;

    Page* voteOrRandomPage;
    UIControlTimer* timer;

    static RaceCupSelectPage* getPage(int pageId);

    // Custom structures from here onwards
    RaceCupSelectPageEx extension;
};
size_assert(RaceCupSelectPage, 0x23E0 + sizeof(RaceCupSelectPageEx));
