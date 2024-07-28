#pragma once
#include <common/Common.hpp>
#include <game/kart/KartObjectProxy.hpp>
#include <game/kart/KartBoost.hpp>
#include <game/system/RaceConfig.hpp>
#include <revolution/mtx.h>

class KartMove : public KartObjectProxy {
public:

    enum DriftState{
        NOT_DRIFTING,
        CHARGING,
        MT_CHARGED,
        SMT_CHARGED,
        UMT_CHARGED,

        DRIFT_STATE_COUNT
    };

    KartMove();
    virtual ~KartMove();
    virtual void createComponents();
    virtual void setTurnParams();
    virtual void init(bool unk0, bool unk1);
    virtual void activateStar();
    virtual void activateMega();
    virtual void activateInk(bool unk);
    virtual void clear();
    virtual void initOob();
    virtual void vf_2c(); // stuff with wheelie
    virtual void vf_30(); // stuff with wheelie
    virtual void cancelWheelieTurn();
    virtual float getLeanRot() const;
    virtual bool canWheelie() const;
    virtual void calcTurn();
    virtual void calcVehicleSpeed();
    virtual void calcAirtimeTop();
    virtual void calcRotation(float turn);
    virtual float getWheelieSoftSpeedLimitBonus() const;
    virtual void calcWheelie();
    virtual void vf_58(); // stuff with wheelie
    virtual bool canHop() const;
    virtual void hop();
    virtual void calcMtCharge();
    virtual void cancelWheelie();

    bool addMtCharge(u32 currentDriftState, short *mtCharge, short baseMtCharge, u32 maxMtCharge);
    void calcManualDrift();
    void clearDrift();
    void releaseMt(bool unk0, u32 unk1);
    void activateBoost(u32 type, u32 duration);

    float speedMultiplier;
    float baseSpeed;
    float softSpeedLimit;
    float _1C;

    float speed;
    float lastSpeed;
    float _28;

    float hardSpeedLimit;

    float acceleration;
    float draftMultiplier;

    u8 unk[0xCA-0x38];

    s16 umtCharge; // Custom field (using padding)
    int hopStickX;

    u8 unk2[0xFC - 0xD0];

    s16 driftState;
    s16 mtCharge;
    s16 smtCharge;
    s16 mtBoostTimer;
    float outsideDriftBonus;

    KartBoost boost;

    u8 unk3[0x1F4-0x12C];

    VEC3 cannonExitDir;

    u8 unk4[0x2C4 - 0x200];

    static const float speedModifiers[RaceConfig::Settings::CC_COUNT];
    static float kartSpeedLimit;
    static float bulletSpeedLimit;

    static float minimumDriftThreshold;
};
size_assert(KartMove, 0x2C4);

class KartMoveBike : public KartMove {
public:
    KartMoveBike();
    virtual ~KartMoveBike();
    virtual void calcMtCharge();
};
