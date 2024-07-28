#pragma once
#include <common/Common.hpp>
#include <game/system/Timer.hpp>
#include <game/system/TimerManager.hpp>
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

        u8 unk[0x08 - 0x04];
        u8 playerIdx;
        // 1 byte padding
        u16 checkpointId;

        u8 unk2[0x26 - 0x0C];
        u8 maxLap;

        u8 unk3[0x40 - 0x27];
        Timer* finishTime;

        u8 unk4[0x54 - 0x44];
    };
    size_assert(Player, 0x54);

    virtual ~RaceManager();

    void calc();

    Random* dynamicRandom;
    Random* staticRandom;
    Player** players;
    RaceMode* gameModeData;
    TimerManager* timerManager;
    u8 unk[0x1E - 0x18];

    s16 introTimer;
    u32 frameCounter;
    u8 unk2[0x28 - 0x24];

    u32 raceStage;
    bool introSkipped;
    bool spectatorMode;
    bool canCountdownStart;
    bool cutsceneMode;
    u8 unk3[0x4C - 0x30];

    static RaceManager* instance;
};
size_assert(RaceManager, 0x4C);
