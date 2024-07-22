#include <common/Common.hpp>
#include <game/system/SaveGhostManager.hpp>
#include <game/system/SystemManager.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/util/NandUtil.hpp>
#include <mvp/save/SaveExpansionManager.hpp>

///////////////////////////
// Page Expansion System //
///////////////////////////

// Update Section class size
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

    Scene::SceneId sceneId = Section::getSceneId(sectionId);
    pad.resetStatus(sceneId != Scene::SCENE_RACE);

    Section* section = new Section();
    curSection = section;
    section->init(sectionId);
}

///////////////////////////
// Save Expansion System //
///////////////////////////

// SectionManager::init() patch
// Redirect the game if the save expansion is missing or there was an error while reading it
kmCallDefCpp(0x80634EC8, int, SaveGhostManager* self) {

    // Original call
    int section = self->getErrorSection();

    // If there was an error with the original save, just go to the indicated section
    if (section != Section::NONE)
        return section;

    // Otherwise copy the savegame error from the expansion and do our own error check
    self->saveManagerError = SaveExpansionManager::sError;
    switch (SaveExpansionManager::sError) {
        case NandUtil::ERROR_NONE:
        case NandUtil::ERROR_REGION:
            return Section::NONE;

        case NandUtil::ERROR_FILE_CORRUPT:
            return Section::SAVE_INVALID;

        case NandUtil::ERROR_SPACE:
            self->nandManagerCheckError = SaveExpansionManager::sCheckError;
            return Section::SAVE_CANNOT_FLUSH;

        default:
            return Section::SAVE_CANNOT_READ_SYSMEM;
    }
}
