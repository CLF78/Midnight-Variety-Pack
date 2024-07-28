#pragma once
#include <common/Common.hpp>
#include <game/kart/KartObject.hpp>

class AudioHandleHolder {
public:
    static AudioHandleHolder* instance;

    virtual ~AudioHandleHolder();

    virtual void link(void* pointer, u16 objectId);
    virtual void unlink();
    virtual void update();
    virtual void disableConditional();
    virtual void updateUserParam3();
    virtual void* startSoundLimited(u32 soundId, float localVar);
    virtual void* holdSoundLimited(u32 soundId);
    virtual void stopAllSound(int fadeOutFrames);
    virtual void* startNewSoundLimited(u32 soundId, float volume);
    virtual void prepare(u32 soundId, bool unk);
    virtual void prepareSpecial(u32 soundId, bool unk);

    virtual u32 getRoadSoundId() const;
    virtual u32 getBrakingSoundId() const;
    virtual u32 getDriftAndSSMTRoadSoundId() const;
    virtual u32 getLandingSoundId() const;

    bool isChargingSSMT;
    u8 padding[3];
    void* engineMgr;
    u8 _BC[0xdd - 0xbc];
    u8 currentLap;
    KartObject* kartObject;
    bool isLocal;
    bool isGhost;
    u8 wheelCount;
    u8 _E3;
    u32 team;
    bool isColliding;
    u8 driftState;
    u8 _EA[2];
    float speedNorm;
    u16 kclFlag;
    u16 variant;
    u32 outputLineFlag;
    u32 roadSate;
};
