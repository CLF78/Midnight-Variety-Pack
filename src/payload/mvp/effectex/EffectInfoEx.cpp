#include "EffectInfoEx.hpp"
#include <game/host_system/Scene.hpp>
#include <game/system/ResourceManager.hpp>
#include <platform/stdio.h>

// TODO autogenerate this
const EffectInfoEx::CustomEffect EffectInfoEx::customEffectData[] = {
    { Scene::SCENE_RACE, MultiDvdArchive::COMMON, "Effect/RKRaceEx" },
};

// Initialize effect array
EffectInfoEx::EffectInfoEx() {
    for (u32 i = 0; i < ARRAY_SIZE(customEffects); i++) {
        customEffects[i] = nullptr;
    }
}

// Load custom effects
void EffectInfoEx::loadCustomEffects(Scene::SceneId sceneId) {

    // Set up loop
    ResourceManager* resMgr = ResourceManager::instance;
    u32 customEffectCount = 0;
    for (u32 i = 0; i < ARRAY_SIZE(EffectInfoEx::customEffectData); i++) {

        // If the file does not belong to this scene, ignore it
        const EffectInfoEx::CustomEffect* entry = &EffectInfoEx::customEffectData[i];
        if (entry->sceneId != sceneId) {
            continue;
        }

        // Get the breff and breft name
        char breffName[0x100];
        char breftName[0x100];
        snprintf(breffName, sizeof(breffName), "/%s.breff", entry->path);
        snprintf(breftName, sizeof(breftName), "/%s.breft", entry->path);

        // Get the BREFF
        void* breff = resMgr->getFile(entry->source, breffName, nullptr);
        if (!breff) {
            LOG_FATAL("Could not find file %s. Please make sure it is present!", breffName);
        }

        // Get the BREFT
        void* breft = resMgr->getFile(entry->source, breftName, nullptr);
        if (!breft) {
            LOG_FATAL("Could not find file %s. Please make sure it is present!", breftName);
        }

        // Create the resource
        customEffects[customEffectCount++] = new EGG::EffectResource(breff, breft);
    }
}
