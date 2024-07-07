#include <common/Common.hpp>
#include <game/ui/InputHandler.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/MenuInputManager.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/ctrl/CtrlMenuInstructionText.hpp>
#include <game/ui/ctrl/CtrlMenuPageTitleText.hpp>
#include <game/ui/ctrl/VoteControl.hpp>

class VotingPage : public Page {
public:
    virtual ~VotingPage();
    virtual int getReplacement();

    virtual void onInit();
    virtual void onDeinit();

    virtual void beforeInAnim();
    virtual void beforeOutAnim();
    virtual void beforeCalc();

    RUNTIME_TYPE_INFO_NODECL;

    bool setPlayerVote(u32 playerIdx);
    void submitVote(u32 courseId);

    static VotingPage* getPage() {
        return (VotingPage*)SectionManager::instance->curSection->pages[Page::WIFI_VOTING];
    }

    InputHandler1<VotingPage, void, LayoutUIControl*> onDisconnectMsgBoxClick;
    MenuInputManager inputManager;
    int votedCourse;

    CtrlMenuPageTitleText titleText;
    CtrlMenuInstructionText instructionText;
    LayoutUIControl voteCounter;
    VoteControl voteControls[12];

    int votes[12];
    int voteCount;
    int isBattle;
    int state;
    int localPlayerCount;
    int curSelectedVote;
    int winningVote;
    int winningTrackBmgId;

    float curRouletteSpeed;
    float curAnimPos;
    int transitionDelayTimer;
};
size_assert(VotingPage, 0x16C0);
