#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>

class WifiMemberConfirmPage : public Page {
public:

    enum RatingType {
        RATING_NONE,
        RATING_VS,
        RATING_BT,
    };

    WifiMemberConfirmPage();
    virtual ~WifiMemberConfirmPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    void setPlayerEntry(int entryIdx, int playerIdx, RaceConfig::Player::Team team,
                        RatingType ratingType, bool isLocalPlayer);

    InputHandler1<WifiMemberConfirmPage, void, PushButton*> onBtnPress;
    MultiControlInputManager inputManager;

    CtrlMenuPageTitleText titleText;
    CtrlMenuInstructionText instructionText;
    PushButton okButton;

    LayoutUIControl playerEntries[12];
    UIControlTimer* timer;

    static const char* animNames[12];
};
size_assert(WifiMemberConfirmPage, 0x192C);
