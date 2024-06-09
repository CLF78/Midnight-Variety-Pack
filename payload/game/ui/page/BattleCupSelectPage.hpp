#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectStage.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <midnight/cup/BattleCupSelectArrow.hpp>
#include <midnight/cup/CupManager.hpp>

class BattleCupSelectPage : public MenuPage {
public:
    BattleCupSelectPage();
    virtual ~BattleCupSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void afterCalc();
    virtual void onRefocus();

    virtual TypeInfo* getTypeInfo() const;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    void setCourseNames(CtrlMenuBattleCupSelectCup* cupHolder, PushButton* button, int unk);

    CtrlMenuBattleCupSelectCup cupHolder;
    CtrlMenuBattleCupSelectStage stageHolder;
    u32 selectedButtonId;

    Page* voteOrRandomPage;
    UIControlTimer* timer;
};
size_assert(BattleCupSelectPage, 0x175C);

// Class expansion for the custom cup system
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
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32> leftHandler;
    InputHandler2<BattleCupSelectArrow, void, SheetSelectControl*, u32> rightHandler;
    s32 curPage;
};
