#include <common/Common.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleCupSelectStage.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <midnight/online/YesNoPopupPageEx.hpp>

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

    RUNTIME_TYPE_INFO_NODECL;

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

    YesNoPopupPageEx* voteOrRandomPage;
    UIControlTimer* timer;
};
size_assert(BattleCupSelectPage, 0x175C);
