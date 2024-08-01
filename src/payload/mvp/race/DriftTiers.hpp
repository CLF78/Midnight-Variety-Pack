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

    static const int SmtEffectsCount = 8;
    static const int UmtEffectsCount = 8;
    explicit ExpPlayerEffects(KartObject* kartObject) : PlayerEffects(kartObject) { };
    virtual ~ExpPlayerEffects() {
        if (isBike) {
            EGG::Effect** array = rk_orangeMT;
            if (array != nullptr) {
                for(int i = 0; i < SmtEffectsCount; ++i) delete(array[i]);
                delete[] array;
            }
        }
        else {
            EGG::Effect** array = rk_purpleMT;
            if (array != nullptr) {
                for(int i = 0; i < UmtEffectsCount; ++i) delete(array[i]);
                delete[] array;
            }
        }
    }
    EGG::Effect** rk_purpleMT;
    EGG::Effect** rk_orangeMT;
    static const char* UMTNames[8];
    static const char* SMTNames[8];
};
