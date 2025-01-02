#pragma once
#include "EffectGroups.hpp"

class KartUmtEffects : public CustomEffectGroup<12> {
public:
    enum EffectIndex {
        rk_driftSpark3L_Spark00,
        rk_driftSpark3L_Spark01,
        rk_driftSpark3R_Spark00,
        rk_driftSpark3R_Spark01,
        rk_driftSpark3L1T_Chip00,
        rk_driftSpark3L1T_Spark00,
        rk_driftSpark3L1T_Spark01,
        rk_driftSpark3R1T_Chip00,
        rk_driftSpark3R1T_Spark00,
        rk_driftSpark3R1T_Spark01,
        rk_driftSpark3L_Chip00,
        rk_driftSpark3R_Chip00,
        EFFECT_COUNT,
    };

    virtual void Create(PlayerEffects* playerEffects);
};

class BikeSmtEffects : public CustomEffectGroup<17> {
public:
    enum EffectIndex {
        rk_driftSpark2LB_Spark00,
        rk_driftSpark2LB_Spark01,
        rk_driftSpark2RB_Spark00,
        rk_driftSpark2RB_Spark01,
        rk_driftSpark2CB_Spark00,
        rk_driftSpark2CB_Spark01,
        rk_driftSpark2LB1T_Chip00,
        rk_driftSpark2LB1T_Spark00,
        rk_driftSpark2LB1T_Spark01,
        rk_driftSpark2RB1T_Chip00,
        rk_driftSpark2RB1T_Spark00,
        rk_driftSpark2RB1T_Spark01,
        rk_driftSpark2CB1T_Chip00,
        rk_driftSpark2CB1T_Spark00,
        rk_driftSpark2CB1T_Spark01,
        rk_driftSpark2LB_Chip00,
        rk_driftSpark2RB_Chip00,
        EFFECT_COUNT,
    };

    virtual void Create(PlayerEffects* playerEffects);
};

class PlayerEffectsEx {
public:
    KartUmtEffects kartUmtEffects;
    BikeSmtEffects bikeSmtEffects;
};
