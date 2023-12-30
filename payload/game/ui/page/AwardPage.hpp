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
        CREDITS_FULL
    };

    InputHandler0<AwardPage, void> onFront;
    u32 _58;

    PageInputManager inputManager;
    LayoutUIControl cupDisplay;
    LayoutUIControl _314;
    LayoutUIControl congratulations;
    LayoutUIControl items[12];

    bool isTeams;
    u8 bestRank;
    u8 localPlayerCount;
    u8 playerCount;
};
size_assert(AwardPage, 0x1770);
