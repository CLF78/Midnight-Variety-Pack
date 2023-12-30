#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.hpp>

class BattleStageSelectPageEx {
public:
    CtrlMenuBattleStageSelectCupSub cups[6];
};

class BattleStageSelectPage : public MenuPage {
public:
    UIControlTimer* timer;

    CtrlMenuBattleStageSelectCup cupHolder;
    CtrlMenuBattleStageSelectStage courseHolder;
    bool stageSelected;

    static BattleStageSelectPage* getPage() {
        return (BattleStageSelectPage*)SectionManager::instance->curSection->pages[Page::COURSE_SELECT_BT];
    }

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
