#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/PageInputManager.hpp>
#include <game/ui/Page.hpp>

class AwardPage : public Page {
public:

    enum CreditsType {
        CREDITS_NONE,
        CREDITS_NORMAL,
        CREDITS_FULL,
    };

    virtual ~AwardPage();

    virtual void onInit();
    virtual void onActivate();

    virtual void beforeInAnim();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    void initRank();
    void initType();

    static CreditsType getCreditsType(u32 cupId, u32 engineClass, bool isMirror, u32 rank);

    InputHandler0<AwardPage, void> onFront;
    u32 _58;

    PageInputManager inputManager;
    LayoutUIControl cupDisplay;
    LayoutUIControl rankDisplay;
    LayoutUIControl congratulations; // AwardDemoCongratulations
    LayoutUIControl items[12]; // AwardDemoResultItem

    bool isTeams;
    u8 bestRank;
    u8 localPlayerCount;
    u8 playerCount;
};
size_assert(AwardPage, 0x1770);
