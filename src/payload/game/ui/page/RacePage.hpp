#pragma once
#include "Page.hpp"
#include <game/system/Timer.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/input/PageInputManager.hpp>

class RacePage : public Page {
public:
    enum Controls {
        TIMER = BIT_FLAG(1),
        COUNTDOWN = BIT_FLAG(2),
        MINIMAP = BIT_FLAG(3),
        POSITION = BIT_FLAG(4),
        LAP = BIT_FLAG(5),
        ITEM_ROULETTE = BIT_FLAG(6),
        BATTLE_POINTS = BIT_FLAG(7),
        GHOST_TIME_DIFF = BIT_FLAG(8),
        LIVE = BIT_FLAG(9),
        SCORE = BIT_FLAG(10),
        ITEM_BALLOON = BIT_FLAG(11),
        GHOST_MESSAGE = BIT_FLAG(12),
        BATTLE_POINT_ADD = BIT_FLAG(13),

        // Custom values
        MESSAGE_QUEUE = BIT_FLAG(14),
    };

    virtual ~RacePage();

    virtual int getReplacement();
    virtual void setNextSection(int sectionId, int animId, f32 delay);

    virtual void onInit();
    virtual void onDeinit();
    virtual void onDeactivate();
    virtual void beforeCalc();
    virtual void afterCalc();

    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const = 0;
    virtual int getControls() const = 0;
    virtual int getNameBalloonCount() const;

    virtual void onPause(u32 hudSlotId);
    virtual bool hasPlayerRaceEnded(u8 id);
    virtual bool hasRaceEnded();
    virtual bool hasRaceEndedReplay();

    u8 getControlCount(Controls controls);
    void initControls(Controls controls);

    int replacementPage;

    Timer timer;
    u8 countdownSeconds;
    PAD(3);

    void* raceCounts; // TODO header
    LayoutUIControl* ghostMessage;

    bool hasHudPlayer[4];
    u32 spectatedPlayerId;
    bool spectatedPlayerIdChangeRequested;
    PAD(3);

    u32 _6C;                   // something with ghost races
    void** wifiStartMessages;  // TODO header
    void** wifiFinishMessages; // TODO header
    u32 framesAfterFinish;

    Page* pausePage; // TODO header
    PageInputManager inputManager;

    InputHandler1<RacePage, void, u32>* onPauseHandler;
    InputHandler1<RacePage, void, u32>* onNextPlayerSwitchHandler;
    InputHandler1<RacePage, void, u32>* onPrevPlayerSwitchHandler;

    void* nameBalloons;     // TODO header
    void** balloonManagers; // TODO header
    void* itemBalloons;     // TODO header
};
size_assert(RacePage, 0x1DC);

class GpRacePage : public RacePage {
public:
    virtual ~GpRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class GpReplayRacePage : public RacePage {
public:
    virtual ~GpReplayRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class TtRacePage : public RacePage {
public:
    virtual ~TtRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class GhostReplayRacePage : public RacePage {
public:
    virtual ~GhostReplayRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class SingleVsRacePage : public RacePage {
public:
    virtual ~SingleVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class WifiSingleVsRacePage : public RacePage {
public:
    virtual ~WifiSingleVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class WifiFriendVsRacePage : public RacePage {
public:
    virtual ~WifiFriendVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class WifiFriendTeamVsRacePage : public RacePage {
public:
    virtual ~WifiFriendTeamVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class MultiVsRacePage : public RacePage {
public:
    virtual ~MultiVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class WifiMultiVsRacePage : public RacePage {
public:
    virtual ~WifiMultiVsRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class LiveViewPage : public RacePage {
public:
    virtual ~LiveViewPage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class SingleBtRacePage : public RacePage {
public:
    virtual ~SingleBtRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class MultiBtRacePage : public RacePage {
public:
    virtual ~MultiBtRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class LiveViewBtPage : public RacePage {
public:
    virtual ~LiveViewBtPage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};

class ToRacePage : public RacePage {
public:
    virtual ~ToRacePage();
    RUNTIME_TYPE_INFO_NODECL;

    virtual int getPausePage() const;
    virtual int getControls() const;
    virtual int getNameBalloonCount() const;
};
