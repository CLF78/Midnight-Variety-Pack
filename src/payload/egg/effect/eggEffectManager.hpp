#pragma once

namespace EGG {

class EffectCreator;

class EffectManager {
public:
    struct EffectArg {
        EGG::Heap* heap;
        int maxEffects;
        int maxEmitters;
        int maxParticleManagers;
        int maxParticles;
    };
    size_assert(EffectArg, 0x14);

    EffectManager(u32 maxGroupCount, EGG::Heap* heap);
    void CreateMemoryManager(EffectArg* args);
    static EffectManager* Create(u32, EGG::Heap* heap);

    u32 maxGroupID;
    u32 maxEffectsCount;
    UNK(0x28 - 0x8);
    EffectCreator** creators;
    void* memoryBlock;

    static EffectManager* sInstance;
};
size_assert(EffectManager, 0x30);

} // namespace EGG
