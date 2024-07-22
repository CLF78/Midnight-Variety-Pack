
#include <common/Common.hpp>
#include <game/ui/page/MenuPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>

class DriftSelectPage : public MenuPage {
public:
    DriftSelectPage();
    virtual ~DriftSelectPage();
    virtual void onInit();
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void afterCalc();
    RUNTIME_TYPE_INFO_NODECL;
    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();
    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* pushButton);

    void onButtonClick(PushButton& button, u32 hudSlotId);
    void onButtonDeselect(PushButton& button, u32 hudSlotId);
    void onBckPress(u32 hudSlotId);
    static DriftSelectPage* getPage(){
        return (DriftSelectPage*)SectionManager::instance->curSection->getPage(Page::DRIFT_SELECT);
    };

    UIControlTimer* timer;
    Section::SectionId sectionId;
};
size_assert(DriftSelectPage, 0x6CC);
