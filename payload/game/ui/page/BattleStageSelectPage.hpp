#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectCup.hpp>
#include <game/ui/ctrl/CtrlMenuBattleStageSelectStage.hpp>

class BattleStageSelectPage : public MenuPage {
public:
    BattleStageSelectPage();
    virtual ~BattleStageSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void afterCalc();

    virtual TypeInfo* getTypeInfo() const;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    UIControlTimer* timer;

    CtrlMenuBattleStageSelectCup cupHolder;
    CtrlMenuBattleStageSelectStage courseHolder;
    bool stageSelected;
};
size_assert(BattleStageSelectPage, 0x1A84);

// Class expansion for the custom cup system
class BattleStageSelectPageEx : public BattleStageSelectPage {
public:
    BattleStageSelectPageEx();
    virtual ~BattleStageSelectPageEx() {}

    void setCourse(CtrlMenuBattleStageSelectStage* courseHolder, PushButton* button, int unk);
    void handleBtnClick(PushButton* button);
    void handleBackPress(int playerId);

    static BattleStageSelectPageEx* getPage() {
        return (BattleStageSelectPageEx*)SectionManager::instance->curSection->pages[Page::COURSE_SELECT_BT];
    }

    static u32 getCupCount() {
        return 2 + ARRAY_SIZE_STATIC(BattleStageSelectPageEx, cups);
    }

    CtrlMenuBattleStageSelectCupSub* getCupButton(u32 idx) {
        if (idx < 2)
            return &cupHolder.cups[idx];
        else if (idx < getCupCount())
            return &cups[idx-2];
        return nullptr;
    }

    CtrlMenuBattleStageSelectCupSub cups[6];
};
