#include <common/Common.hpp>
#include <game/system/Mii.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MessageInfo.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>

class MenuPage : public Page {
public:
    static const int BACK_BUTTON = -100;

    virtual ~MenuPage();

    virtual int getReplacement();
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
    virtual void onStartPressed();
    virtual void onActionsLoaded();

    virtual PushButton* loadButton(int buttonIdx);
    virtual LayoutUIControl* loadLayout() = 0;
    virtual void setupButton(PushButton* button);

    virtual MessageInfo* getMsgInfo();
    virtual void initVideos();

    void startReplace(u32 pageId, PushButton* button);
    void requestSectionChange(u32 sectionId, PushButton* button);
    void forceSectionChange(u32 sectionId, PushButton* button);

    // non-virtual versions of virtual functions
    void callOnInit();
    void callOnActivate();

    void loadMovies(const char** movies, u32 movieCount);

    MiiGroup* miiGroup;

    PushButton** buttons;
    u32 buttonCount;
    PushButton* selectedButton;
    CtrlMenuBackButton backButton;

    LayoutUIControl* titleText;
    CtrlMenuInstructionText* instructionText;

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

    InputHandler0<MenuPage, void> loadPrevMenuAction;
    InputHandler0<MenuPage, void> initThpAction;
    int videoStartFrame;
    int extraChildNumber;
    bool locked;

    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler0<MenuPage, void> onBtnClick;
    InputHandler0<MenuPage, void> onBtnSelect;
    InputHandler0<MenuPage, void> onBtnDeselect;
    InputHandler0<MenuPage, void> onBackPress;
    InputHandler0<MenuPage, void> onStartPress;

    u32 activePlayers;
    u32 activeControllers;
};
size_assert(MenuPage, 0x6C4);
