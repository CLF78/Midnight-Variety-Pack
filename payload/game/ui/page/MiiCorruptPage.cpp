#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/util/NandUtil.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// MiiCorruptPage::onActivate patch()
// Replace message box message when a save error occurs
kmCallDefCpp(0x806207CC, void, LayoutUIControl* self, u32 msgId, MessageInfo* msgInfo) {

    // If the section is not the corrupted Mii data one, use the save message error depending on the check error
    if (SectionManager::instance->curSection->sectionID != Section::SAVE_CANNOT_READ_RFL) {
        int checkError = SectionManager::instance->saveGhostManager->nandManagerCheckError;
        if (checkError & NandUtil::CHECK_ERROR_BLOCKS)
            msgId = 2053;
        else if (checkError & NandUtil::CHECK_ERROR_INODES)
            msgId = 2071;
        else
            msgId = 2057;
    }

    // Set the text
    self->setText(msgId, nullptr);
}
