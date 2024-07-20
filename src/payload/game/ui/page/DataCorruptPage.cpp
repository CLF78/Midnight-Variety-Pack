#include <common/Common.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/page/YesNoPage.hpp>
#include <game/util/NandUtil.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// DataCorruptPage::showDeletePrompt() patch
// Display a different message if only the expansion data is corrupted
kmCallDefCpp(0x8061FE70, void, YesNoPage* self, u32 msgId, MessageInfo* msgInfo) {
    msgId = (SaveManager::instance->result != NandUtil::ERROR_NONE) ?
            Message::Menu::ERROR_SAVE_CORRUPTED :
            Message::Menu::ERROR_SAVE_EXPANSION_CORRUPTED;
    self->setWindowMessage(msgId);
}
