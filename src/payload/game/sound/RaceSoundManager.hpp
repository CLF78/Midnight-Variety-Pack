#pragma once
#include <egg/audio/eggAudioTrack.hpp>
#include <egg/core/eggDisposer.hpp>
#include <game/system/Timer.hpp>
#include <nw4r/ut/utList.hpp>
#include <revolution/mtx.h>

class KartSound;

class RaceSoundManager : public EGG::Disposer {
public:

    enum SoundType {
        AWARD_CREDITS_BGM,
        GHOST_REPLAY_BGM,
        COURSE_INTRO_BGM,
        COUNTDOWN_JINGLE,
        COURSE_BGM,
        FINAL_LAP_JINGLE,
        COURSE_BGM_FAST,
        RESULTS_BGM,
    };

    void init();

    u32 courseId;
    u8 _14[4];

    KartSound* kartSounds[4];
    u8 lastUsedKartSoundSlot;
    u8 totalKartSounds;
    PAD(2);
    u32 _2C;

    Timer timer;
    u8 maxLap;
    u8 lap;
    u8 position;
    u8 playerIdFirstLocalPlayer;
    u32 currSoundType;
    u32 gameMode;
    u32 gameType;
    u8 localPlayerCount;
    u8 playerCount;
    u8 realPlayerCount;
    PAD(1);
    s16 _50;
    PAD(2);

    EGG::AudioTrack engineVolume;
    u8 _70;
    u8 _71;
    PAD(2);
    s32 _74;

    nw4r::ut::List actorsList;
    MTX34 playerMats[4];
    VEC3 playerVecs[4];
    u8 _174[4];

    static RaceSoundManager* instance;
};
size_assert(RaceSoundManager, 0x178);
