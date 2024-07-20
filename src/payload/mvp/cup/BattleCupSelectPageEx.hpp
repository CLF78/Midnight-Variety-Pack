#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <game/ui/page/BattleCupSelectPage.hpp>
#include <mvp/cup/BattleCupSelectArrow.hpp>

class BattleCupSelectPageEx : public BattleCupSelectPage {
public:

    enum LayoutIdx {
        LYT_CUP_SELECT_CUP,
        LYT_CUP_SELECT_COURSE,
        LYT_CUP_ARROWS,
    };

    BattleCupSelectPageEx();
    virtual ~BattleCupSelectPageEx() {}

    virtual void onActivate();
    virtual UIControl* loadLayout(u32 layoutIdx);

    void setCourse(CtrlMenuBattleCupSelectCup* cupHolder, PushButton* button, int unk);

    static BattleCupSelectPageEx* getPage() {
        return (BattleCupSelectPageEx*)SectionManager::instance->curSection->pages[Page::CUP_SELECT_BT];
    }

    static u32 getCupCount() {
        return 2 + ARRAY_SIZE_STATIC(BattleCupSelectPageEx, cupButtons);
    }

    PushButton* getCupButton(u32 idx) {
        if (idx < 2)
            return &cupHolder.cupButtons[idx];
        else if (idx < getCupCount())
            return &cupButtons[idx-2];
        return nullptr;
    }

    PushButton cupButtons[6];
    SheetSelectControl arrows;
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32> onLeftHandler;
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32> onRightHandler;
    s32 curPage;
};
