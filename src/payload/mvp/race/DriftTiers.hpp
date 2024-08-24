#pragma once
#include <egg/effect/eggEffect.hpp>
#include <game/effect/PlayerEffects.hpp>
#include <game/kart/KartObject.hpp>

class ExpPlayerEffects : public PlayerEffects {
public:
    enum UMTeffects {
        rk_driftSpark3L_Spark00,
        rk_driftSpark3L_Spark01,
        rk_driftSpark3R_Spark00,
        rk_driftSpark3R_Spark01,
        rk_purpleBoost,
        rk_purpleBoost_2,
        rk_purpleBoost_3,
        rk_purpleBoost_4
    };

    explicit ExpPlayerEffects(KartObject* kartObject) : PlayerEffects(kartObject) {}
    virtual ~ExpPlayerEffects() {

        EGG::Effect** array = isBike ? rk_orangeMT : rk_purpleMT;
        const u32 effCount = isBike ? SmtEffectsCount : UmtEffectsCount;

        if (array != nullptr) {
            for (u32 i = 0; i < effCount; ++i) {
                delete (array[i]);
            }
            delete[] array;
        }
    }

    EGG::Effect** rk_purpleMT;
    EGG::Effect** rk_orangeMT;

    static const u32 SmtEffectsCount = 8;
    static const u32 UmtEffectsCount = 8;
    static const char* UMTNames[SmtEffectsCount];
    static const char* SMTNames[SmtEffectsCount];
};
