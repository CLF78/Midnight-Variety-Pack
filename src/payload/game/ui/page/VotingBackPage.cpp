#include <common/Common.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <midnight/online/RepickQueue.hpp>

// Copy the repick queue from the SELECT packet
REPLACE u32 VotingBackPage::getInstructionText() {

    // Copy the repick queue
    LOG_DEBUG("Copying repick queue...");
    RepickQueue::instance.queue = RKNetSELECTHandler::instance->expansion.sendPacketEx.repickQueue;

    // Original call
    return REPLACED();
}
