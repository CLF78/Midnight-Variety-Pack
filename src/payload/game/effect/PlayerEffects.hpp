#pragma once
#include <egg/effect/eggEffect.hpp>
#include <game/kart/KartObject.hpp>
#include <game/system/RaceConfig.hpp>
#include <mvp/effectex/PlayerEffectsEx.hpp>

class PlayerEffects {
public:
    enum KartDriftEffect {
        rk_driftSmoke,
        rk_driftSmoke_1,
        rk_driftSpark1L_Spark00,
        rk_driftSpark1L_Spark01,
        rk_driftSpark1R_Spark00,
        rk_driftSpark1R_Spark01,
        rk_driftSpark2L_Spark00,
        rk_driftSpark2L_Spark01,
        rk_driftSpark2R_Spark00,
        rk_driftSpark2R_Spark01,
        rk_driftSpark1L1T_Chip00,
        rk_driftSpark1L1T_Spark00,
        rk_driftSpark1L1T_Spark01,
        rk_driftSpark1R1T_Chip00,
        rk_driftSpark1R1T_Spark00,
        rk_driftSpark1R1T_Spark01,
        rk_driftSpark2L1T_Chip00,
        rk_driftSpark2L1T_Spark00,
        rk_driftSpark2L1T_Spark01,
        rk_driftSpark2R1T_Chip00,
        rk_driftSpark2R1T_Spark00,
        rk_driftSpark2R1T_Spark01,
        rk_driftSpark1L_Chip00,
        rk_driftSpark1R_Chip00,
        rk_driftSpark2L_Chip00,
        rk_driftSpark2R_Chip00,
        rk_start0,
        rk_start1,
        rk_start2,
        rk_start0_1,
        rk_start1_1,
        rk_start2_1,
        rk_autoDriftL,
        rk_autoDriftR,
        rk_brakeSmk,
        rk_brakeSmk_1,
    };

    enum BikeDriftEffects {
        rk_driftSmokeB,
        rk_driftSpark1LB_Spark00,
        rk_driftSpark1LB_Spark01,
        rk_driftSpark1RB_Spark00,
        rk_driftSpark1RB_Spark01,
        rk_driftSpark1CB_Spark00,
        rk_driftSpark1CB_Spark01,
        rk_driftSpark1LB1T_Chip00,
        rk_driftSpark1LB1T_Spark00,
        rk_driftSpark1LB1T_Spark01,
        rk_driftSpark1RB1T_Chip00,
        rk_driftSpark1RB1T_Spark00,
        rk_driftSpark1RB1T_Spark01,
        rk_driftSpark1CB1T_Chip00,
        rk_driftSpark1CB1T_Spark00,
        rk_driftSpark1CB1T_Spark01,
        rk_driftSpark1LB_Chip00,
        rk_driftSpark1RB_Chip00,
        rk_hangOnL,
        rk_hangOnR,
        rk_hangOnLTail,
        rk_hangOnRTail,
        rk_start0B,
        rk_start1B,
        rk_start2B,
        rk_brakeSmkB,
        rk_autoDriftLB,
        rk_autoDriftRB,
    };

    enum WheelType {
        REAR_WHEEL_L,
        REAR_WHEEL_R,
        REAR_WHEEL_BIKE,
    };

    explicit PlayerEffects(KartObject* kartObject);
    virtual ~PlayerEffects();

    void init();
    void loadEffects();
    void reduceEmitRatios();

    void updateKartDriftEffects();

    void createKartDriftBlueExplodingSparks();   // 806a26a0
    void createKartDriftOrangeExplodingSparks(); // 806a290c
    void createKartDriftPurpleExplodingSparks(); // custom

    void updateKartDriftSmoke();                 // 806a1820
    void updateKartDriftBlueChips();             // 806a2118
    void updateKartDriftBlueExplodingSparks();   // 806a2780
    void updateKartDriftBlueSparks();            // 806a1d70
    void updateKartDriftOrangeChips();           // 806a2348
    void updateKartDriftOrangeExplodingSparks(); // 806a29ec
    void updateKartDriftOrangeSparks();          // 806a1fd8
    void updateKartDriftPurpleChips();           // custom
    void updateKartDriftPurpleExplodingSparks(); // custom
    void updateKartDriftPurpleSparks();          // custom
    void updateKartBrakeSmoke();                 // 806a1cc8

    void fadeKartDriftSmoke();        // 806a1938
    void fadeKartDriftBlueChips();    // 806a21f4
    void fadeKartDriftBlueSparks();   // 806a1e4c
    void fadeKartDriftOrangeChips();  // 806a2424
    void fadeKartDriftOrangeSparks(); // 806a20b4
    void fadeKartDriftPurpleChips();  // custom
    void fadeKartDriftPurpleSparks(); // custom
    void fadeKartBrakeSmoke();        // 806a1d1c

    void killKartDriftBlueChips();             // 806a2258
    void killKartDriftBlueExplodingSparks();   // 806a27e4
    void killKartDriftBlueSparks();            // 806a1eb0
    void killKartDriftOrangeExplodingSparks(); // 806a2a50
    void killKartDriftOrangeChips();           // 806a2488
    void killKartDriftOrangeSparks();          // custom
    void killKartDriftPurpleChips();           // custom
    void killKartDriftPurpleExplodingSparks(); // custom
    void killKartDriftPurpleSparks();          // custom

    bool IsWheelOnGround(int wheelIdx) const {
        return (wheelsOnFloor[wheelIdx] || wheelNotOnFloorTimers[wheelIdx] < wheelNotOnFloorThreshold);
    }

    void createEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex,
                            u32 lastEffectIndex); // 8069797c

    void createAndUpdateEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
                                     MTX34* playerMat, VEC3* wheelPos, bool updateScale);

    void updateEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
                            MTX34* playerMat, VEC3* wheelPos, bool updateScale);

    void followFadeEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex,
                                MTX34* playerMat, VEC3* wheelPos, bool updateScale);

    void killEffectsByIdx(EGG::Effect** effectsArray, u32 firstEffectIndex, u32 lastEffectIndex); // 806a25ec

    UNK(0xD - 0x4);
    bool isInsideDrift;
    bool isAutoDrift;
    bool isHumanPlayer;
    bool isRemoteCPU;
    bool isHumanPlayer2P;
    bool isRemoteCPU2P;
    bool isHumanPlayer3P4P;
    bool isCPU3P4P;
    UNK(0x34 - 0x15);

    u32 mtCharge;
    UNK(0x80 - 0x38);

    u32 wheelNotOnFloorTimers[2];
    UNK(0x118 - 0x88);

    KartObject* kartObject;
    UNK(0x124 - 0x11C);

    u32 isBike;
    bool hasThreeWheels;
    bool isRealPlayer;
    bool isRemoteCPUPlayer;
    bool isGhost;
    bool isTTReplay;
    bool isRealOrGhost;
    u8 playerId;
    UNK(0x134 - 0x12F);

    int playerIdPlus2;
    UNK(0x52C - 0x138);

    EGG::Effect** kartEffects;
    EGG::Effect** kartDriftEffects;
    bool kartEffectsLoaded[8];
    bool kartDriftEffectsLoaded[36];
    float kartEffectEmitRatios[8];
    float kartDriftEffectEmitRatios[36];

    EGG::Effect** bikeEffects;
    EGG::Effect** bikeDriftEffects;
    bool bikeEffectsLoaded[8];
    bool bikeDriftEffectsLoaded[28];
    float bikeEffectEmitRatios[8];
    float bikeDriftEffectEmitRatios[28];
    UNK(0x6F4 - 0x6CC);

    MTX34 playerMat;
    u32 gameScreenIdx;
    float internalVelocity;
    UNK(0x7A0 - 0x72C);

    s16 driftState;
    UNK(0x7B9 - 0x7A2);

    bool wheelsOnFloor[2];
    PAD(1);

    VEC3 wheelPositions[2];
    VEC3 prevWheelPositions[2];
    UNK(0x988 - 0x7EC);

    s16 blueChipAppearThreshold;
    s16 orangeChipAppearThreshold;
    s16 loopedSparkAppearThreshold;
    UNK(0xAE0 - 0x98E);

    u8 wheelNotOnFloorThreshold;
    UNK(0xAE8 - 0xAE1);

    // Expansion structure
    PlayerEffectsEx expansion;
};
size_assert(PlayerEffects, 0xAE8 + sizeof(PlayerEffectsEx));
