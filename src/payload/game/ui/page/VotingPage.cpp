#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/system/Identifiers.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <midnight/cup/CupData.hpp>
#include <midnight/cup/CupManager.hpp>

///////////////////////
// Custom Cup System //
///////////////////////

// VotingPage::setPlayerVote() override
// Ensure the correct track name is displayed
REPLACE bool VotingPage::setPlayerVote(u32 playerIdx) {

    // Get VotingBackPage, bail if it doesn't exist
    VotingBackPage* page = VotingBackPage::getPage();
    if (!page)
        return false;

    // If the player index is not valid, bail
    if (playerIdx >= page->playerCount)
        return false;

    // If the vote has already been set, bail
    if (votes[playerIdx] != -1)
        return false;

    // Get aid and local player index
    u8 aid = page->playerInfos[playerIdx].aid;
    u8 localPlayerIdx = page->playerInfos[playerIdx].localPlayerIdx;

    // If the character is invalid, bail
    if (RKNetSELECTHandler::instance->getPlayerCharacter(aid, localPlayerIdx) == CHARACTER_COUNT)
        return false;

    // Get the voted track and the corresponding message
    u16 track = RKNetSELECTHandler::instance->getPlayerVote(aid);
    u16 bmgId = CupManager::getTrackName(track);

    // Set the vote
    voteControls[playerIdx].setData(track == CupData::RANDOM_TRACK_VOTE, bmgId, &page->miiGroup,
                                    playerIdx, RKNetController::instance->isLocalPlayer(aid),
                                    page->playerInfos[playerIdx].team);
    return true;
}

// VotingPage::tryStartRoulette() patch
// Store the correct track name and prevent the game from stalling due to invalid votes
void StoreWinningVote(VotingPage* self) {
    u32 trackIdx = RKNetSELECTHandler::instance->getWinningTrack();
    self->winningTrackBmgId = CupManager::getTrackName(trackIdx);
}

// Glue code
kmBranchDefAsm(0x80644310, 0x8064436C) {
    nofralloc

    // Call C++ code
    mr r3, r27
    bl StoreWinningVote
    blr
}

// VotingPage::tryStartRoulette() patch
// Disable the course cache
kmWrite32(0x80644414, 0x60000000);
