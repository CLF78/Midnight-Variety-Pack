#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetPacketHolder.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <platform/string.h>
#include <wiimmfi/RoomStall.hpp>

///////////////////////////////////////////////
// Custom Cup System / Custom Engine Classes //
///////////////////////////////////////////////

// RKNetSELECTHandler::getStaticInstance() patch
// Update size of class
kmCallDefCpp(0x8065FEB0, u32) {
    return sizeof(RKNetSELECTHandler);
}

// RKNetSELECTHandler::getStaticInstance() patch
// Construct the expansion data
kmCallDefCpp(0x8065FF48, void, RKNetSELECTHandler* self) {

    // Store instance
    RKNetSELECTHandler::instance = self;

    // Construct expansion data
    if (self) RKNetSELECTHandlerEx::construct(&self->expansion);
}

// RKNetSELECTHandler::GetWinningTrack() override
// Get the winning course from the expansion
kmBranchDefCpp(0x80660450, NULL, u16, RKNetSELECTHandler* self) {

    // Do not get the track if it hasn't been decided
    if (self->sendPacket.phase != RKNetSELECTPacket::LOTTERY)
        return CupData::NO_TRACK;

    // Otherwise get it from the expansion
    return self->expansion.sendPacketEx.winningCourse;
}

// RKNetSELECTHandler::getPlayerVote() override
// Get the course vote from the expansion
// Original address: 0x80660574
u16 RKNetSELECTHandler::getPlayerVote(u8 aid) {

    // If it's my AID, get the value from the send packet
    if (aid == RKNetController::instance->getCurrentSub()->myAid)
        return expansion.sendPacketEx.courseVote;

    // Else get it from the correct received packet
    return expansion.recvPacketsEx[aid].courseVote;
}

// RKNetSELECTHandler::SetPlayerData() override
// Store the course vote in the correct field
kmBranchDefCpp(0x80660750, NULL, void, RKNetSELECTHandler* self, int characterId, int vehicleId,
               int courseVote, int localPlayerIdx, u8 starRank) {

    RKNetSELECTPlayer* player = &self->sendPacket.playerData[localPlayerIdx];
    player->character = characterId;
    player->vehicle = vehicleId;
    player->starRank = starRank;
    self->expansion.sendPacketEx.courseVote = courseVote;
}

// RKNetSELECTHandler::PrepareAndExportSELECTAndUSER() patch
// Send the expanded packet
kmCallDefCpp(0x80661040, void, RKNetPacketHolder* holder, RKNetSELECTPacket* data, u32 dataSize) {

    // Copy the original packet
    holder->copyData(data, dataSize);

    // Copy the expansion as well
    holder->appendData(&RKNetSELECTHandler::instance->expansion.sendPacketEx,
                       sizeof(RKNetSELECTPacketExpansion));
}

// RKNetSELECTHandler::ImportNewPackets() patch
// Update incoming packet size check
kmWrite16(0x806610FE, sizeof(RKNetSELECTPacketEx));

// RKNetSelectHandler::ImportNewPackets() patch
// Import the expanded packet correctly
kmHookFn void CopyExpandedPacket(RKNetPacketHolder* holder, u32 aid) {

    // Copy the expansion data
    void* dest = &RKNetSELECTHandler::instance->expansion.recvPacketsEx[aid];
    memcpy(dest, ((u8*)holder->buffer) + sizeof(RKNetSELECTPacket), sizeof(RKNetSELECTPacketExpansion));

    // Clear the buffer (original call)
    holder->clear();
}

// Glue code
kmCallDefAsm(0x806611D4) {
    nofralloc

    // Call C++ code
    mr r4, r19
    b CopyExpandedPacket
    blr
}

// TODO add custom settings
void RKNetSELECTHandler::storeUpdatedRaceSettings(u8 aid) {
    sendPacket.battleTeamData.raw = recvPackets[aid].battleTeamData.raw;
    sendPacket.selectId = recvPackets[aid].selectId;
    sendPacket.engineClass.raw = recvPackets[aid].engineClass.raw;
}

// TODO add custom settings
bool RKNetSELECTHandler::checkUpdatedRaceSettings(u8 aid) {
    return raceSettingsDetermined() &&
           (sendPacket.battleTeamData.raw == recvPackets[aid].battleTeamData.raw) &&
           (sendPacket.selectId == recvPackets[aid].selectId) &&
           (sendPacket.engineClass.raw == recvPackets[aid].engineClass.raw);
}

bool RKNetSELECTHandler::checkUpdatedRaceSettingsAll() {
    if (aidsWithNewRaceSettings == 0)
        return false;

    // If i am host and the settings have been determined, add me to the list of updated AIDs
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    if (raceSettingsDetermined())
        aidsWithNewRaceSettings |= 1 << sub->myAid;

    return sub->availableAids == (sub->availableAids & aidsWithNewRaceSettings);
}

void RKNetSELECTHandler::storeVote(u8 aid) {

    // Override the winning AID's voted track with the one determined by the host
    u8 winningVoter = recvPackets[aid].winningVoterAid;
    u16 winningCourse = expansion.recvPacketsEx[aid].winningCourse;
    if ((RKNetController::instance->getCurrentSub()->availableAids & (1 << winningVoter)) &&
        winningCourse != CupData::NO_TRACK)
        expansion.recvPacketsEx[winningVoter].winningCourse = winningCourse;
}

bool RKNetSELECTHandler::checkVote(u8 aid) {
    return expansion.recvPacketsEx[aid].courseVote != CupData::UNDECIDED_TRACK_VOTE;
}

bool RKNetSELECTHandler::checkVoteAll() {
    if (aidsThatHaveVoted == 0)
        return false;

    // If i am host and i have voted, add me to the list
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    if (trackVoted())
        aidsThatHaveVoted |= 1 << sub->myAid;

    return sub->availableAids == (sub->availableAids & aidsThatHaveVoted);
}

void RKNetSELECTHandler::storeUpdatedVoteData(u8 aid) {
    expansion.sendPacketEx.winningCourse = expansion.recvPacketsEx[aid].winningCourse;
    sendPacket.winningVoterAid = recvPackets[aid].winningVoterAid;
    sendPacket.aidPidMap = recvPackets[aid].aidPidMap;
}

bool RKNetSELECTHandler::checkUpdatedVoteData(u8 aid) {
    return voteDetermined() &&
           (expansion.sendPacketEx.winningCourse == expansion.recvPacketsEx[aid].winningCourse) &&
           (sendPacket.winningVoterAid == recvPackets[aid].winningVoterAid) &&
           (sendPacket.aidPidMap == recvPackets[aid].aidPidMap);
}

bool RKNetSELECTHandler::checkUpdatedVoteDataAll() {
    if (aidsWithVoteData == 0)
        return false;

    // If i am host and the vote result has been determined, add me to the list of updated AIDs
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    if (voteDetermined())
        aidsWithVoteData |= 1 << sub->myAid;

    return sub->availableAids == (sub->availableAids & aidsWithVoteData);
}

// RKNetSELECTHandler::calcPhase() override
// Update the voting phase using the correct data
kmBranchDefCpp(0x806614B0, NULL, void, RKNetSELECTHandler* self) {

    // Check if there are any unprocessed incoming packets, if not bail
    if (!self->hasUnprocessedRecvPackets())
        return;

    // Get current sub
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();

    // Get outgoing packet and its expansion
    RKNetSELECTPacket* sendPacket = &self->sendPacket;
    RKNetSELECTPacketExpansion* sendPacketEx = &self->expansion.sendPacketEx;

    // Parse each received packet
    for (int aid = 0; aid < 12; aid++) {

        // Skip my AID
        if (aid == sub->myAid)
            continue;

        // If the AID is not available, skip
        u32 aidMask = 1 << aid;
        if (!(aidMask & sub->availableAids))
            continue;

        // Get incoming packet and its expansion
        RKNetSELECTPacket* recvPacket = &self->recvPackets[aid];
        RKNetSELECTPacketExpansion* revcPacketEx = &self->expansion.recvPacketsEx[aid];

        // Update phase for host
        if (sub->myAid == sub->hostAid) {
            switch (sendPacket->phase) {

                // In the prepare phase, check that everyone has the updated race settings
                case RKNetSELECTPacket::PREPARE:
                    if (self->checkUpdatedRaceSettings(aid))
                        self->aidsWithNewRaceSettings |= aidMask;

                    if (self->checkUpdatedRaceSettingsAll())
                        sendPacket->phase = RKNetSELECTPacket::VOTING;
                    break;

                // In the voting phase, check that everyone has the vote result
                case RKNetSELECTPacket::VOTING:
                    if (self->checkUpdatedVoteData(aid))
                        self->aidsWithVoteData |= aidMask;

                    if (self->checkUpdatedVoteDataAll())
                        sendPacket->phase = RKNetSELECTPacket::LOTTERY;
                    break;

                // In the lottery phase, don't do anything
                default:
                    break;
            }

        // Update phase for guest
        } else if (aid == sub->hostAid) {
            switch (sendPacket->phase) {

                // In the prepare phase, copy the race settings and wait for host to go to the next phase
                case RKNetSELECTPacket::PREPARE:
                    self->storeUpdatedRaceSettings(aid);
                    if (recvPacket->phase > RKNetSELECTPacket::PREPARE)
                        sendPacket->phase = RKNetSELECTPacket::VOTING;
                    break;

                // In the voting phase, check that everyone has voted, copy the vote data
                // then wait for host to go to the next phase
                case RKNetSELECTPacket::VOTING:
                    if (self->checkVoteAll()) {
                        self->storeVote(aid);
                        self->storeUpdatedVoteData(aid);
                    }

                    if (recvPacket->phase > RKNetSELECTPacket::VOTING)
                        sendPacket->phase = RKNetSELECTPacket::LOTTERY;
                    break;

                // In the lottery phase, don't do anything
                default:
                    break;
            }
        }

        // Update who has voted
        if (self->checkVote(aid))
            self->aidsThatHaveVoted |= aidMask;

        // Something??
        if (self->aidsWithNewRH1)
            sendPacket->phase = RKNetSELECTPacket::LOTTERY;
    }
}

//////////////////////////////////
// Online Room Stall Prevention //
//////////////////////////////////

// RKNetSELECTHandler::calc() patch
// Update the timer every frame and kick if necessary
// Credits: Wiimmfi
kmCallDefCpp(0x80660330, bool) {
    Wiimmfi::RoomStall::Update();
    return true; // Not calling the original function here because the result would always be true
}

// RKNetSELECTHandler::getStaticInstance() patch
// Initialize the timer
// Credits: Wiimmfi
kmBranchDefCpp(0x8065FF5C, NULL, void) {
    Wiimmfi::RoomStall::Init();
}
