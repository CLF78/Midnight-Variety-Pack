#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectStage.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>

// Expansion structure for the custom cup system
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
    void setCourseNames(CtrlMenuBattleCupSelectCup* cupHolder, PushButton* button, int unk);

    CtrlMenuBattleCupSelectCup cupHolder;
    CtrlMenuBattleCupSelectStage stageHolder;
    u32 selectedButtonId;

    Page* voteOrRandomPage;
    UIControlTimer* timer;

    static BattleCupSelectPage* getPage() {
        return (BattleCupSelectPage*)SectionManager::instance->curSection->pages[Page::CUP_SELECT_BT];
    }

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
