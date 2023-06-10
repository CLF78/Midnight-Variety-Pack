#include <kamek.h>
#include <game/ui/page/MenuPage.h>
#include <game/ui/MultiControlInputManager.h>
#include <game/ui/UIControlTimer.h>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.h>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.h>

class BattleStageSelectPageEx {
public:
    CtrlMenuBattleStageSelectCupSub cups[6];
};

class BattleStageSelectPage : public MenuPage {
public:
    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler0<BattleStageSelectPage, void> onBtnClick;
    InputHandler0<BattleStageSelectPage, void> onBtnSelect;
    InputHandler0<BattleStageSelectPage, void> onBtnDeselect;
    InputHandler0<BattleStageSelectPage, void> onBackPress;
    InputHandler0<BattleStageSelectPage, void> onStartPress;

    u32 activePlayers;
    u32 activeControllers;

    UIControlTimer* timer;

    CtrlMenuBattleStageSelectCup cupHolder;
    CtrlMenuBattleStageSelectStage courseHolder;
    bool _1A80;

    static BattleStageSelectPage* getPage(int pageId);

    // Custom structures from here onwards
    BattleStageSelectPageEx extension;

    CtrlMenuBattleStageSelectCupSub* getCupButton(u32 idx) {
        if (idx < 2)
            return &cupHolder.cups[idx];
        else if (idx < 8)
            return &extension.cups[idx-2];
        return nullptr;
    }

};
size_assert(BattleStageSelectPage, 0x1A84 + sizeof(BattleStageSelectPageEx));
