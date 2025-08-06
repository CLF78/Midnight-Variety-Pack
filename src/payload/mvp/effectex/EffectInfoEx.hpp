#pragma once
#include <egg/effect/eggEffectResource.hpp>
#include <game/host_system/Scene.hpp>
#include <game/system/MultiDvdArchive.hpp>

class EffectInfoEx {
public:
    struct CustomEffect {
        Scene::SceneId sceneId;
        MultiDvdArchive::Type source;
        const char* path;
    };

    static const int EFFECT_FILE_COUNT = 1;

    EffectInfoEx();
    void loadCustomEffects(Scene::SceneId sceneId);

    EGG::EffectResource* customEffects[EFFECT_FILE_COUNT];
    static const CustomEffect customEffectData[EFFECT_FILE_COUNT];
};
