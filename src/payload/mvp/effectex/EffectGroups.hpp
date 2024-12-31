#pragma once
#include <egg/effect/eggEffect.hpp>

class PlayerEffects;

template <int EffectCount>
class CustomEffectGroup {
public:
    CustomEffectGroup() {
        for (int i = 0; i < EffectCount; i++) {
            effects[i] = nullptr;
        }
    }

    ~CustomEffectGroup() {
        for (int i = 0; i < EffectCount; i++) {
            delete effects[i];
        }
    }

    virtual void Create(PlayerEffects* playerEffects) = 0;

    void CheckLoaded() {
        for (int i = 0; i < EffectCount; i++) {
            if (EGG::Effect* eff = effects[i]) {
                loaded[i] = eff->LoadEmitter(1);
                emitRatio[i] = eff->GetEmitRatio();
            }
            else {
                loaded[i] = false;
                emitRatio[i] = 0.0f;
            }
        }
    }

    void ReduceEmitRatios() {
        for (int i = 0; i < EffectCount; i++) {
            if (loaded[i]) {
                effects[i]->setEmitRatio(emitRatio[i] * 0.5f);
            }
        }
    }

    EGG::Effect* effects[EffectCount];
    bool loaded[EffectCount];
    float emitRatio[EffectCount];
};

#define CREATE_EFFECT(index, creator) \
    effects[index] = new EGG::Effect(STRINGIFY(index), creator)

#define CREATE_EFFECT_WITH_SUFFIX(index, suffix, creator) \
    effects[index] = new EGG::Effect(STRINGIFY(index) suffix, creator) // NOLINT(bugprone-macro-parentheses)
