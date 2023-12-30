#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/page/YesNoPopupPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>

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
    CtrlMenuCupSelectCup cupHolder;
    CtrlMenuCupSelectCourse courseHolder;
    u32 selectedButtonId;

    YesNoPopupPage* voteOrRandomPage;
    UIControlTimer* timer;

    void updateTextMessages(CtrlMenuCupSelectCup* cupHolder, u32 unk); // overridden

    static RaceCupSelectPage* getPage() {
        return (RaceCupSelectPage*)SectionManager::instance->curSection->pages[Page::CUP_SELECT];
    }

    // Custom structures from here onwards
    RaceCupSelectPageEx extension;
};
size_assert(RaceCupSelectPage, 0x23E0 + sizeof(RaceCupSelectPageEx));
