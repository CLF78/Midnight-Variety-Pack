#include <common/Common.hpp>
#include <game/ui/page/BattleStageSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <mvp/online/YesNoPopupPageEx.hpp>

class BattleStageSelectPageEx : public BattleStageSelectPage {
public:
    BattleStageSelectPageEx();
    virtual ~BattleStageSelectPageEx() {}

    virtual void onActivate();
    virtual void afterCalc();

    void onRepickPromptPress(s32 choice, PushButton* button);
    void setCourse(CtrlMenuBattleStageSelectStage* courseHolder, PushButton* button, int unk);
    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onBackPress(u32 hudSlotId);

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
    InputHandler2<BattleStageSelectPageEx, void, s32, PushButton*> onRepickPromptPressHandler;
    YesNoPopupPageEx* repickPrompt;
};
