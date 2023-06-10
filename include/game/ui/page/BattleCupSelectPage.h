#include <kamek.h>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectCup.h>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectStage.h>
#include <game/ui/ctrl/SheetSelectControl.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>
#include <game/ui/UIControlTimer.h>

// Expansion structures for the custom cup system
class BattleCupSelectArrow : public SheetSelectButton {
public:
    void BattleCupSelectArrow::onLeftArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
    void BattleCupSelectArrow::onRightArrowPress(SheetSelectControl* arrowPair, u32 localPlayerId);
};

class BattleCupSelectPageEx {
public:
    PushButton cupButtons[6];
    SheetSelectControl arrows;
    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32> leftHandler;
    InputHandler2<SheetSelectButton, void, SheetSelectControl*, u32> rightHandler;
    s32 curPage;
};

class BattleCupSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler0<BattleCupSelectPage, void> onBtnClick;
    InputHandler0<BattleCupSelectPage, void> onBtnSelect;
    InputHandler0<BattleCupSelectPage, void> onBtnDeselect;
    InputHandler0<BattleCupSelectPage, void> onBackPress;
    InputHandler0<BattleCupSelectPage, void> onStart;

    u32 activePlayers;
    u32 activeControllers;

    CtrlMenuBattleCupSelectCup cupHolder;
    CtrlMenuBattleCupSelectStage stageHolder;
    u32 selectedButtonId;

    Page* voteOrRandomPage;
    UIControlTimer* timer;

    static BattleCupSelectPage* getPage(int pageId);

    // Custom structures from here onwards
    BattleCupSelectPageEx extension;

    PushButton* getCupButton(u32 idx) {
        if (idx < 2)
            return &cupHolder.cupButtons[idx];
        else if (idx < 8)
            return &extension.cupButtons[idx-2];
        return nullptr;
    }
};
size_assert(BattleCupSelectPage, 0x175C + sizeof(BattleCupSelectPageEx));
