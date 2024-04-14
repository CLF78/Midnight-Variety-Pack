#include <common/Common.hpp>
#include <game/net/RKNetController.hpp>
#include <game/net/RKNetPacketHolder.hpp>
#include <game/net/RKNetSelectHandler.hpp>
#include <game/net/RKNetUserHandler.hpp>
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

// Create the expanded class
REPLACE_STATIC void RKNetSELECTHandler::getStaticInstance() {

    // Construct expansion data
    REPLACED();
    if (RKNetSELECTHandler* handler = RKNetSELECTHandler::instance)
        RKNetSELECTHandlerEx::construct(&handler->expansion);

    // Initialize Wiimmfi room stall timer
    Wiimmfi::RoomStall::Init();
}

// Get the course vote from the expansion
REPLACE u16 RKNetSELECTHandler::getPlayerVote(u8 aid) {

    // If it's my AID, get the value from the send packet
    if (RKNetController::instance->isLocalPlayer(aid))
        return expansion.sendPacketEx.courseVote;

    // Else get it from the correct received packet
    return expansion.recvPacketsEx[aid].courseVote;
}

// Get the winning course from the expansion
REPLACE u16 RKNetSELECTHandler::getWinningTrack() {

    // Do not get the track if it hasn't been decided
    if (sendPacket.phase != RKNetSELECTPacket::LOTTERY)
        return CupData::NO_TRACK;

    // Otherwise get it from the expansion
    return expansion.sendPacketEx.winningCourse;
}

// Store the course vote in the correct field
REPLACE void RKNetSELECTHandler::setPlayerData(int characterId, int vehicleId,
               int courseVote, int localPlayerIdx, u8 starRank) {

    RKNetSELECTPlayer* player = &sendPacket.playerData[localPlayerIdx];
    player->character = characterId;
    player->vehicle = vehicleId;
    player->starRank = starRank;
    expansion.sendPacketEx.courseVote = courseVote;
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

// Update the voting phase using the correct data
REPLACE void RKNetSELECTHandler::calcPhase() {

    // Check if there are any unprocessed incoming packets, if not bail
    if (!hasUnprocessedRecvPackets())
        return;

    // Get current sub and outgoing packet expansion
    RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    RKNetSELECTPacketExpansion* sendPacketEx = &expansion.sendPacketEx;

    // Parse each received packet
    for (int aid = 0; aid < 12; aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
            continue;

        // Get incoming packet and its expansion
        u32 aidMask = 1 << aid;
        RKNetSELECTPacket* recvPacket = &recvPackets[aid];
        RKNetSELECTPacketExpansion* revcPacketEx = &expansion.recvPacketsEx[aid];

        // Update phase for host
        if (sub->myAid == sub->hostAid) {
            switch (sendPacket.phase) {

                // In the prepare phase, check that everyone has the updated race settings
                case RKNetSELECTPacket::PREPARE:
                    if (checkUpdatedRaceSettings(aid))
                        aidsWithNewRaceSettings |= aidMask;

                    if (checkUpdatedRaceSettingsAll())
                        sendPacket.phase = RKNetSELECTPacket::VOTING;
                    break;

                // In the voting phase, check that everyone has the vote result
                case RKNetSELECTPacket::VOTING:
                    if (checkUpdatedVoteData(aid))
                        aidsWithVoteData |= aidMask;

                    if (checkUpdatedVoteDataAll())
                        sendPacket.phase = RKNetSELECTPacket::LOTTERY;
                    break;

                // In the lottery phase, don't do anything
                default:
                    break;
            }

        // Update phase for guest
        } else if (aid == sub->hostAid) {
            switch (sendPacket.phase) {

                // In the prepare phase, copy the race settings and wait for host to go to the next phase
                case RKNetSELECTPacket::PREPARE:
                    storeUpdatedRaceSettings(aid);
                    if (recvPacket->phase > RKNetSELECTPacket::PREPARE)
                        sendPacket.phase = RKNetSELECTPacket::VOTING;
                    break;

                // In the voting phase, check that everyone has voted, copy the vote data
                // then wait for host to go to the next phase
                case RKNetSELECTPacket::VOTING:
                    if (checkVoteAll()) {
                        storeVote(aid);
                        storeUpdatedVoteData(aid);
                    }

                    if (recvPacket->phase > RKNetSELECTPacket::VOTING)
                        sendPacket.phase = RKNetSELECTPacket::LOTTERY;
                    break;

                // In the lottery phase, don't do anything
                default:
                    break;
            }
        }

        // Update who has voted
        if (checkVote(aid))
            aidsThatHaveVoted |= aidMask;

        // Something??
        if (aidsWithNewRH1)
            sendPacket.phase = RKNetSELECTPacket::LOTTERY;
    }
}

// Import expanded packets properly
REPLACE void RKNetSELECTHandler::importNewPackets() {
    for (int aid = 0; aid < 12; aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
            continue;

        // Get the received packet buffer and check that it's a SELECT packet using the size
        RKNetPacketHolder* holder = RKNetController::instance->getPacketRecvBuffer(aid, RKNET_SECTION_ROOM_SELECT);
        if (holder->currentPacketSize == sizeof(RKNetSELECTPacketEx)) {

            // Store received time
            lastRecvTimes[aid] = OSGetTime();

            // Update AID map of players that have sent at least one SELECT packet
            aidsWithNewSelect |= (1 << aid);

            // Copy the original packet
            holder->copyData(&recvPackets[aid], sizeof(RKNetSELECTPacket));

            // Copy the expansion data
            memcpy(&expansion.recvPacketsEx[aid], ((u8*)holder->buffer) + sizeof(RKNetSELECTPacket),
                   sizeof(RKNetSELECTPacketExpansion));

            // Handle receive delay
            handleRecvPacketDelay(aid);

            // Clear the buffer
            holder->clear();
        }

        // Check if the AID is already in the race
        if (hasUnprocessedRecvPackets() && RKNetController::instance->getPacketRecvBuffer(aid, RKNET_SECTION_RACEHEADER_1)->currentPacketSize)
            aidsWithNewRH1 |= (1 << aid);
    }
}

// Send the expanded packet
REPLACE void RKNetSELECTHandler::setSendPacket() {

    // Check that at least 170 milliseconds have passed
    s64 currTime = OSGetTime();
    if (OSTicksToMilliseconds(currTime - lastSentTime) < 170)
        return;

    // Find the next AID to send the packet to
    u8 aid = lastSentToAid;
    for (int i = 0; i < 12; i++) {

        // Ensure the next AID does not overflow
        if (++aid >= 12)
            aid = 0;

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
            continue;

        // Schedule USER packet for sending
        RKNetUSERHandler::instance->SetSendUSERPacket(aid);

        // Prepare the SELECT packet
        prepareSendPacket(aid, currTime);

        // Copy the data and the expansion to the holder
        RKNetPacketHolder* holder = RKNetController::instance->getPacketSendBuffer(aid, RKNET_SECTION_ROOM_SELECT);
        holder->copyData(&sendPacket, sizeof(RKNetSELECTPacket));
        holder->appendData(&expansion.sendPacketEx, sizeof(RKNetSELECTPacketExpansion));

        // Update last send info and return
        lastSentTime = currTime;
        lastSentToAid = aid;
        return;
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
