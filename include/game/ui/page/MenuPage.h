#include <game/ui/Page.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/InputHandler.h>
#include <game/ui/MessageInfo.h>
#include <game/ui/PushButton.h>
#include <game/ui/ctrl/CtrlMenuBackButton.h>

class MiiGroup;

class MenuPage : public Page {
public:
    virtual ~MenuPage();

    virtual int getReplacementPage();
    virtual MenuPage* addPage(int pageId, int animId);

    virtual void onInit();
    virtual void onActivate();
    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void onRefocus();
    virtual TypeInfo* getTypeInfo() const;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers() = 0;
    virtual int getActiveControllers() = 0;
    virtual MenuInputManager* getInputManager() = 0;

    virtual void setSelection(int buttonId);
    virtual void setSelection(PushButton* button);
    virtual void onStartPress();
    virtual void onActionsLoaded();

    virtual void vf_84();
    virtual void vf_88() = 0;
    virtual void vf_8c();

    virtual MessageInfo* getMsgInfo();
    virtual void initVideos();

    MiiGroup* miiGroup;

    PushButton** buttons;
    u32 buttonCount;
    PushButton* selectedButton;
    CtrlMenuBackButton backButton;

    LayoutUIControl* topBar;
    LayoutUIControl* bottomBar;

    u8 _2BC[0x50];
    u32 videoCount;

    MessageInfo text;

    u32 layoutCount;
    bool hasBackButton;
    u8 _3DD;
    u8 _3DE;
    u8 _3DF;
    u32 _3E0;

    u32 titleBmgId;
    int nextPageId;
    int prevPageId;
    int prevSectionId;
    int nextSectionId;
    u32 _3F8;

    InputHandler<MenuPage> loadPrevMenuAction;
    InputHandler<MenuPage> initThpAction;
    int videoStartFrame;
    int extraChildNumber;
    bool locked;
};
size_assert(MenuPage, 0x430);
