#include "SectionManager.hpp"
#include <game/system/SaveGhostManager.hpp>
#include <game/system/SystemManager.hpp>
#include <game/util/NandUtil.hpp>
#include <mvp/save/SaveExpansionManager.hpp>

///////////////////////////
// Page Expansion System //
///////////////////////////

// Update section class size
REPLACE void SectionManager::createSection() {

    if (firstLoad && SystemManager::instance->launchType == 1) {
        pad.setControllers();
    }

    Section::SectionId sectionId = nextSectionId;
    currentAnimDirection = nextAnimDirection;
    nextSectionId = Section::NONE;
    animDelay = 0;
    nextAnimDirection = Page::ANIM_NEXT;
    someFrameCounter = -1;
    state = IDLE;

    const Scene::SceneId sceneId = Section::getSceneId(sectionId);
    pad.resetStatus(sceneId != Scene::SCENE_RACE);

    Section* section = new Section();
    curSection = section;
    section->init(sectionId);
}
