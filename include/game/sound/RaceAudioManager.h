#include <kamek.h>
#include <nw4r/ut/utList.h>
#include <rvl/mtx/mtx.h>
#include <egg/audio/eggAudioTrack.h>
#include <egg/core/eggDisposer.h>
#include <game/system/Timer.h>

class KartSound;

class RaceAudioManager : public EGG::Disposer {
public:

    enum RaceState {
        INTRO = 2,
        BEFORE_COUNTDOWN,
        NORMAL,
        ENTERING_FINAL_LAP,
        FINAL_LAP,
        FINISHED
    };

    u32 courseId;
    u8 _14[4];

    KartSound* kartSounds[4];
    u8 lastUsedKartSoundSlot;
    u8 totalKartSounds;
    u32 _2C;

    Timer timer;
    u8 maxLap;
    u8 lap;
    u8 position;
    u8 playerIdFirstLocalPlayer;
    u32 raceState;
    u32 gameMode;
    u32 gameType;
    u8 localPlayerCount;
    u8 playerCount;
    u8 realPlayerCount;
    s16 _50;

    EGG::AudioTrack engineVolume;
    u8 _70;
    u8 _71;
    s32 _74;

    nw4r::ut::List actorsList;
    MTX34 playerMats[4];
    VEC3 playerVecs[4];
    u8 _174[4];

    static RaceAudioManager* instance;
};
size_assert(RaceAudioManager, 0x178);
