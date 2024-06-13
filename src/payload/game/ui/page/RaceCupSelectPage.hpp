#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuCupSelectCourse.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/page/YesNoPopupPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <midnight/cup/RaceCupSelectArrow.hpp>

class RaceCupSelectPage : public MenuPage {
public:
    RaceCupSelectPage();
    virtual ~RaceCupSelectPage();

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

    CtrlMenuCupSelectCup cupHolder;
    CtrlMenuCupSelectCourse courseHolder;
    u32 selectedButtonId;

    YesNoPopupPage* voteOrRandomPage;
    UIControlTimer* timer;
};
size_assert(RaceCupSelectPage, 0x23E0);

// Class expansion for the custom cup system
class RaceCupSelectPageEx : public RaceCupSelectPage {
public:

    enum LayoutIdx {
        LYT_CUP_SELECT_CUP,
        LYT_CUP_SELECT_COURSE,
        LYT_CUP_ARROWS,
    };

    RaceCupSelectPageEx();
    virtual ~RaceCupSelectPageEx() {}

    virtual void onActivate();
    virtual UIControl* loadLayout(u32 layoutIdx);

    void setCourse(CtrlMenuCupSelectCup* cupHolder, PushButton* button);
    void updateTextMessages(CtrlMenuCupSelectCup* cupHolder, u32 unk);

    static RaceCupSelectPageEx* getPage() {
        return (RaceCupSelectPageEx*)SectionManager::instance->curSection->pages[Page::CUP_SELECT];
    }

    SheetSelectControl arrows;
    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32> leftHandler;
    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32> rightHandler;
    s32 curPage;
};
