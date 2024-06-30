#include <common/Common.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/MultiControlInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>

class WifiPlayerListPage : public Page {
public:

    enum RatingType {
        RATING_NONE,
        RATING_VS,
        RATING_BT,
    };

    virtual ~WifiPlayerListPage();

    virtual void onInit();
    virtual void onActivate();
    virtual void afterCalc();

    virtual TypeInfo* getTypeInfo() const;

    void setPlayerEntry(int entryIdx, int playerIdx, RaceConfig::Player::Team team,
                        RatingType ratingType, bool isLocalPlayer);

    InputHandler1<WifiPlayerListPage, void, PushButton*> onBtnPress;
    MultiControlInputManager inputManager;

    LayoutUIControl titleText;
    CtrlMenuInstructionText instructionText;
    PushButton okButton;

    LayoutUIControl playerEntries[12];
    UIControlTimer* timer;
};
size_assert(WifiPlayerListPage, 0x192C);
