#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/page/MenuPage.hpp>

class MultiDriftSelectPage : public MenuPage {
public:
    MultiDriftSelectPage();
    virtual ~MultiDriftSelectPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();
    virtual void onRefocus();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, u32 hudSlotId);
    virtual int getActivePlayers();
    virtual int getActiveControllers();
    virtual MenuInputManager* getInputManager();

    virtual void onActionsLoaded();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx);
    virtual void setupButton(PushButton* button);

    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onButtonDeselect(PushButton* button, u32 hudSlotId);
    void onBackPress(u32 hudSlotId);

    static MultiDriftSelectPage* getPage() {
        return (MultiDriftSelectPage*)SectionManager::instance->curSection->getPage(Page::DRIFT_SELECT_MULTI_PLAYER);
    };

    UIControlTimer* timer;
    void* boundingBoxes;
    Section::SectionId nextSectionId;
};
size_assert(MultiDriftSelectPage, 0x6D0);
