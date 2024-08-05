#pragma once
#include "Timer.hpp"
#include "TimerManager.hpp"
#include <game/util/Random.hpp>

class RaceMode;

class RaceManager {
public:

    enum RaceStage {
        STAGE_INTRO,
        STAGE_COUNTDOWN,
        STAGE_RACE,
        STAGE_FINISH,
        STAGE_FINISH_ALL,
    };

    class Player {
    public:

        // TODO add custom reasons maybe?
        enum EndReason {
            REASON_NONE,
            REASON_UNK_1,
            REASON_ALL_FINISHED,
            REASON_TIME_LEADER_FINISH,
            REASON_TIME_LIMIT_REACHED,
        };

        virtual ~Player();

        void endRace(Timer* finishTime, bool isLast, u32 endReason);

        UNK(0x08 - 0x04);
        u8 playerIdx;
        PAD(1);

        u16 checkpointId;
        UNK(0x26 - 0x0C);

        u8 maxLap;
        UNK(0x40 - 0x27);

        Timer* finishTime;
        UNK(0x54 - 0x44);
    };
    size_assert(Player, 0x54);

    virtual ~RaceManager();

    void calc();

    Random* dynamicRandom;
    Random* staticRandom;
    Player** players;
    RaceMode* gameModeData;
    TimerManager* timerManager;
    UNK(0x1E - 0x18);

    s16 introTimer;
    u32 frameCounter;
    UNK(0x28 - 0x24);

    u32 raceStage;
    bool introSkipped;
    bool spectatorMode;
    bool canCountdownStart;
    bool cutsceneMode;
    UNK(0x4C - 0x30);

    static RaceManager* instance;
};
size_assert(RaceManager, 0x4C);
