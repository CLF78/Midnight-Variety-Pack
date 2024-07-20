#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/sound/SoundEffect.hpp>
#include <game/system/Identifiers.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/page/VotingBackPage.hpp>
#include <game/ui/page/VotingPage.hpp>
#include <mvp/cup/CupData.hpp>
#include <mvp/cup/CupManager.hpp>

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
    LOG_DEBUG("Setting vote for player %d to %d...", playerIdx, track);
    voteControls[voteCount].setData(track == CupData::RANDOM_TRACK_VOTE, bmgId, &page->miiGroup,
                                    playerIdx, RKNetController::instance->isLocalPlayer(aid),
                                    page->playerInfos[playerIdx].team);

    // Update the vote index and count
    votes[playerIdx] = voteCount++;
    return true;
}

// VotingPage::tryStartRoulette() patch
// Store the correct track name, prevent the game from stalling due to invalid votes
// and disable the course cache
void StoreWinningVote(VotingPage* self) {

    // Get the winning track and bail if we have not yet obtained it
    u32 trackIdx = RKNetSELECTHandler::instance->getWinningTrack();
    if (trackIdx == CupData::NO_TRACK) {
        LOG_DEBUG("Not yet in phase 2. Retrying later...");
        return;
    }

    // Set the winning track text
    self->winningTrackBmgId = CupManager::getTrackName(trackIdx);
    LOG_DEBUG("Winning track: %d, BMG: %d", trackIdx, self->winningTrackBmgId);

    // Set the winning vote control
    u8 winningVoter = RKNetSELECTHandler::instance->getWinningVoter();
    self->winningVote = self->votes[VotingBackPage::getPage()->getVoteIdx(winningVoter, 0)];

    // Start the roulette
    self->curRouletteSpeed = 0.3f;
    self->curAnimPos = 0.0f;
    self->curSelectedVote = 0;
    self->voteControls[0].animator.getGroup(4)->setAnimation(1, 0.0f);
    self->voteControls[0].playSound(SE_UI_CRS_ROULETTE, -1);
    self->state = 1;
    self->instructionText.setText(Message::Menu::VOTE_ROULETTE_DECIDING);
}

// Glue code
kmBranchDefAsm(0x80644310, 0x80644418) {
    nofralloc

    // Call C++ code
    mr r3, r27
    bl StoreWinningVote
    blr
}
