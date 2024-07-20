#include <common/Common.hpp>
#include <game/system/Mii.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/MenuInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/UIControlTimer.hpp>
#include <game/ui/ctrl/CountdownTimerControl.hpp>

class VotingBackPage : public Page {
public:

    struct PlayerInfo {
        u8 aid;
        u8 localPlayerIdx;
        // 2 bytes padding
        RaceConfig::Player::Team team;
        u16 vr;
        u16 br;
    };

    enum Status {
        WAITING = 0x2,
        VR_PAGE,
        CUP_SELECT,
        VOTING,
        DISCONNECTED,
        DISCONNECT_NO_MSG,
        DISCONNECT_MSG = 0x8,
    };

    virtual ~VotingBackPage();
    virtual int vf_14();

    virtual void onInit();
    virtual void onDeinit();
    virtual void onActivate();
    virtual void onDeactivate();

    virtual void beforeCalc();
    virtual void afterCalc();
    virtual void onRefocus();

    RUNTIME_TYPE_INFO_NODECL;

    int getVoteIdx(u32 aid, u32 localPlayerIdx);
    u32 getInstructionText();
    u32 getInstructionText(MessageInfo* info);

    void setupRace();

    static bool teamsEnabled();

    static VotingBackPage* getPage() {
        return (VotingBackPage*)SectionManager::instance->curSection->pages[Page::TIMER];
    }

    InputHandler1<VotingBackPage, void, Page*> onDisconnectMessageClickHandler;
    MenuInputManager inputManager;
    int status;
    UIControlTimer timer;
    CountdownTimerControl timerControl;
    PlayerInfo playerInfos[12];
    u32 playerCount;
    u32 engineClass;
    u32 battleType;
    MiiGroup miiGroup;
};
size_assert(VotingBackPage, 0x328);
