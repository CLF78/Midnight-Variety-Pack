#include <common/Common.hpp>
#include <game/system/SaveManager.hpp>
#include <game/ui/page/YesNoPage.hpp>
#include <game/util/NandUtil.hpp>

///////////////////////////////
// Savegame Expansion System //
///////////////////////////////

// DataCorruptPage::showDeletePrompt() patch
// Display a different message if only the expansion data is corrupted
kmCallDefCpp(0x8061FE70, void, YesNoPage* self, u32 msgId, MessageInfo* msgInfo) {
    self->setWindowMessage((SaveManager::instance->result != NandUtil::ERROR_NONE) ? 2055 : 2056, nullptr);
}
