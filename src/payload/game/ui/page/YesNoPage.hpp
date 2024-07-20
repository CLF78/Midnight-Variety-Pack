#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PushButton.hpp>

class YesNoPage : public Page {
public:
    virtual ~YesNoPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    virtual void reset();

    void setWindowMessage(u32 msgId, MessageInfo* msgInfo = nullptr);
    void configureButton(int buttonIdx, u32 msgId, MessageInfo* msgInfo, int animDirection,
                         InputHandler2<Page, void, s32, PushButton*>* handler);

    InputHandler2<Page, void, s32, PushButton*>* inputHandlers[2];
    MultiControlInputManager inputManager;

    int choice;
    int anims[2];
    u32 currSelected;

    InputHandler1<YesNoPage, void, PushButton*> onBtnClick;
    PushButton buttons[2];
    LayoutUIControl messageWindow;
    u32 playerFlags;
    bool _8B0;
};
