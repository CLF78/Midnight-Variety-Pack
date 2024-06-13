#include <common/Common.hpp>
#include <game/scene/InitScene.hpp>
#include <game/system/SaveManager.hpp>
#include <game/util/NandUtil.hpp>
#include <midnight/save/SaveExpansionManager.hpp>

///////////////////////////
// Save Expansion System //
///////////////////////////

// Update SaveManager size and construct the expansion
REPLACE_STATIC SaveManager* SaveManager::createStaticInstance() {
    if (!SaveManager::instance) {
        if (SaveManager* mgr = new SaveManager()) {
            SaveExpansion::construct(&mgr->expansion);
            SaveManager::instance = mgr;
        }
    }

    return SaveManager::instance;
}

// Initialize the save expansion
REPLACE void SaveManager::init() {

    // Initialize original savegame
    REPLACED();

    // Only read the expansion if there is no error with the original save
    // There is no point in trying to read the expansion if the normal save couldn't be loaded
    if (result == NandUtil::ERROR_NONE) {
        LOG_DEBUG("Read original save successfully!");
        SaveExpansionManager::sError = SaveExpansionManager::Read();
    } else {
        LOG_ERROR("Failed to read original save with error %d", result);
    }

    // Tell SaveManager we are done
    busy = false;
}

// SaveManager::init() patch
// Allow saves to be region-free
kmWrite32(0x80544930, 0x60000000);

// SaveManager::init() patch
// Wait for save expansion before marking as not busy
kmWrite32(0x80544A1C, 0x60000000);

// SaveManager::reset() patch
// Wait for save expansion before marking as not busy
kmWrite32(0x8054A84C, 0x60000000);

// Do not reset the original savegame if the error is expansion-only
REPLACE_STATIC void SaveManager::resetTask() {

    // Only run save reset if the savegame threw an error
    SaveManager* mgr = SaveManager::instance;
    if (mgr->result != NandUtil::ERROR_NONE)
        mgr->reset();

    // Reset the expansion by deleting it and writing it back
    int result = SaveExpansionManager::Delete();
    if (result == NandUtil::ERROR_NONE)
        result = SaveExpansionManager::Write();

    // Store the result
    SaveExpansionManager::sError = result;

    // End busy state
    mgr->busy = false;
}

// Do not reset the original savegame if the error is expansion-only
REPLACE void SaveManager::resetAsync() {

    // Check for SaveManager error, if so reset the raw save (the rest will be done in a separate thread)
    if (result != NandUtil::ERROR_NONE)
        rawSave->Reset();

    // Reset the save expansion data
    expansion.Init();

    // Mark SaveManager as busy and start async process
    busy = true;
    taskThread->request(&resetTask, 0, 0);
}

// Write savegame and expansion to NAND
REPLACE_STATIC void SaveManager::saveLicensesTask() {

    // Get SaveManager
    SaveManager* mgr = SaveManager::instance;

    // Try flushing savegame to NAND
    // Only write the expansion if the original save is written successfully
    mgr->result = mgr->flushSave();
    if (mgr->result == NandUtil::ERROR_NONE)
        SaveExpansionManager::sError = SaveExpansionManager::Write();

    // End busy state
    mgr->busy = false;
}

// Save the expansion along with the original save
REPLACE void SaveManager::saveLicensesAsync() {

    // Write licenses and expansion
    writeLicenses();
    expansion.Write();

    // Mark as busy and start save task
    busy = true;
    taskThread->request(&saveLicensesTask, 0, 0);
}

// Erase the expansion along with the original save
REPLACE void SaveManager::eraseLicense(u32 licenseId) {
    REPLACED(licenseId);
    expansion.mLicenses[licenseId].Init();
}
