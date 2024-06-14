#include <common/Common.hpp>
#include <game/net/WifiDisconnectInfo.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PageInputManager.hpp>
#include <game/ui/PushButton.hpp>

class WifiDisconnectPage : public Page {
public:
    virtual ~WifiDisconnectPage();

    virtual int vf_14();

    virtual void onInit();
    virtual void onActivate();
    virtual void beforeInAnim();
    virtual void afterCalc();

    virtual TypeInfo* getTypeInfo() const;

    PageInputManager inputManager;
    WifiDisconnectInfo disconnectInfo;
    LayoutUIControl messageBox;
    LayoutUIControl okButton;
    InputHandler1<WifiDisconnectPage, void, PushButton*> onBtnSelect;
};
size_assert(WifiDisconnectPage, 0x48C);
