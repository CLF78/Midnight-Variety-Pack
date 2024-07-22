#include <common/Common.hpp>
#include <game/system/Mii.hpp>
#include <game/ui/page/Page.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MessageInfo.hpp>
#include <game/ui/input/MultiControlInputManager.hpp>
#include <game/ui/ctrl/PushButton.hpp>
#include <game/ui/ctrl/CtrlMenuBackButton.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>

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
    RUNTIME_TYPE_INFO_NODECL;

    virtual void onSelectChange(PushButton* button, int playerId);
    virtual int getActivePlayers() = 0;
    virtual int getActiveControllers() = 0;
    virtual MenuInputManager* getInputManager() = 0;

    virtual void setSelection(int buttonId);
    virtual void setSelection(PushButton* button);
    virtual void onStartPressed(int playerId);
    virtual void onActionsLoaded();

    virtual PushButton* loadButton(int buttonIdx);
    virtual UIControl* loadLayout(u32 layoutIdx) = 0;
    virtual void setupButton(PushButton* button);

    virtual MessageInfo* getMsgInfo();
    virtual void initMovies();

    void requestSectionChange(u32 sectionId, PushButton* button);
    void forceSectionChange(u32 sectionId, PushButton* button);
    void loadPrevPageById(u32 pageId, PushButton& button);
    void loadNextPageById(u32 pageId, PushButton* button);
    void loadNextPageWithDelayById(u32 pageId, float delay);
    void loadPrevPage(PushButton& button);
    void loadPrevPageWithDelay(float delay);
    void pushMessage(u32 bmgId, MessageInfo* text = nullptr);
    void loadMovies(const char** movies, u32 movieCount);
    bool checkAllPlayersActive();

    MiiGroup* miiGroup;

    PushButton** buttons;
    u32 buttonCount;
    PushButton* selectedButton;
    CtrlMenuBackButton backButton;

    CtrlMenuPageTitleText* titleText;
    CtrlMenuInstructionText* instructionText;

    u8 _2BC[0x50];
    u32 movieCount;

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

    InputHandler1<MenuPage, void, Page*> onMessageBoxClickHandler;
    InputHandler1<MenuPage, void, u32> onMoviePreparedHandler;
    int movieStartFrame;
    int extraChildNumber;
    bool locked;

    MultiControlInputManager multiControlInputManager;
    u32 curChildCount;

    InputHandler2<MenuPage, void, PushButton*, u32> onButtonClickHandler;
    InputHandler2<MenuPage, void, PushButton*, u32> onButtonSelectHandler;
    InputHandler2<MenuPage, void, PushButton*, u32> onButtonDeselectHandler;
    InputHandler1<MenuPage, void, u32> onBackPressHandler;
    InputHandler1<MenuPage, void, u32> onStartPressHandler;

    u32 activePlayers;
    u32 activeControllers;
};
size_assert(MenuPage, 0x6C4);
