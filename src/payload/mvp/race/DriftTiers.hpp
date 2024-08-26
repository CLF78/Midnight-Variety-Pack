#pragma once
#include <egg/effect/eggEffect.hpp>
#include <egg/effect/eggEffectResource.hpp>
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
        if (isBike) {
            EGG::Effect** array = rk_orangeMT;
            if (array != nullptr) {
                for (int i = 0; i < smtEffectsCount; ++i) delete(array[i]);
                delete[] array;
            }
        }
        else {
            EGG::Effect** array = rk_purpleMT;
            if (array != nullptr) {
                for (int i = 0; i < umtEffectsCount; ++i) delete(array[i]);
                delete[] array;
            }
        }
    }

    static EGG::Effect** rk_purpleMT;
    static EGG::Effect** rk_orangeMT;

    static const int smtEffectsCount = 8;
    static const int umtEffectsCount = 8;
    static const char* umtNames[umtEffectsCount];
    static const char* smtNames[smtEffectsCount];
    static EGG::EffectResource* umtEffects;
    static EGG::EffectResource* smtEffects;
};
