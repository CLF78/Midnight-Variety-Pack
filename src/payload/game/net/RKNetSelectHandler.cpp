#include "RKNetController.hpp"
#include "RKNetPacketHolder.hpp"
#include "RKNetSelectHandler.hpp"
#include "RKNetUserHandler.hpp"
#include <game/util/Random.hpp>
#include <mvp/cup/CupManager.hpp>
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

// Clear the repick queue when opening a room
// We don't need to clear it at any other time since it's for online only and the host
// shares their list with us
kmListHookDefCpp(RoomOpenHook) {
    LOG_DEBUG("Clearing repick queue...");
    RepickQueue::instance.Clear();
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
    expansion.sendPacketEx.repickQueue = expansion.recvPacketsEx[aid].repickQueue;
}

// TODO add custom settings
bool RKNetSELECTHandler::checkUpdatedRaceSettings(u8 aid) {
    return raceSettingsDetermined() &&
           (sendPacket.battleTeamData.raw == recvPackets[aid].battleTeamData.raw) &&
           (sendPacket.selectId == recvPackets[aid].selectId) &&
           (sendPacket.engineClass.raw == recvPackets[aid].engineClass.raw) &&
           (expansion.sendPacketEx.repickQueue == expansion.recvPacketsEx[aid].repickQueue);
}

bool RKNetSELECTHandler::checkUpdatedRaceSettingsAll() {
    if (aidsWithNewRaceSettings == 0)
        return false;

    // If i am host and the settings have been determined, add me to the list of updated AIDs
    const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    if (raceSettingsDetermined())
        aidsWithNewRaceSettings |= 1 << sub->myAid;

    return sub->availableAids == (sub->availableAids & aidsWithNewRaceSettings);
}

void RKNetSELECTHandler::storeVote(u8 aid) {

    // Override the winning AID's voted track with the one determined by the host
    const u8 winningVoter = recvPackets[aid].winningVoterAid;
    const u16 winningCourse = expansion.recvPacketsEx[aid].winningCourse;
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
    const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
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
    const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
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
    const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    RKNetSELECTPacketExpansion* sendPacketEx = &expansion.sendPacketEx;

    // Parse each received packet
    for (int aid = 0; aid < 12; aid++) {

        // Skip invalid AIDs
        if (!RKNetController::instance->isConnectedPlayer(aid))
            continue;

        // Get incoming packet
        const u32 aidMask = 1 << aid;
        RKNetSELECTPacket* recvPacket = &recvPackets[aid];

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

                    // When the host goes to the next phase, add the winning course to the repick
                    // queue as well
                    if (recvPacket->phase > RKNetSELECTPacket::VOTING) {
                        RepickQueue::instance.Push(expansion.sendPacketEx.winningCourse);
                        sendPacket.phase = RKNetSELECTPacket::LOTTERY;
                    }

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
            memcpy(&recvPackets[aid], holder->buffer, sizeof(RKNetSELECTPacket));

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
    const s64 currTime = OSGetTime();
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

// Replace the engine class picking algorithm
REPLACE void RKNetSELECTHandler::decideEngineClass() {

    // Create randomizer
    Random rnd;

    // Default to the battle settings
    int engineClass = RKNetEngineClassData::CLASS_BATTLE;
    bool mirror = false;
    u32 randNum;

    // Decide the engine class based on the tracklist for VS
    // TODO make the chances configurable instead of hardcoding them here
    // TODO prevent overriding the engine class if explicitly set in the settings
    if (mode == MODE_PUBLIC_VS || mode == MODE_PRIVATE_VS) {
        switch (CupManager::currentCupList) {

            // Chances: 100% 150cc
            case CupManager::TRACKS_MIDNIGHT:
                engineClass = RKNetEngineClassData::CLASS_150CC;
                break;

            // Chances: 99% 150cc, 1% 200cc
            case CupManager::TRACKS_NOSTALGIA:
                randNum = rnd.nextU32(100);
                engineClass = (randNum == 99) ?
                              RKNetEngineClassData::CLASS_200CC :
                              RKNetEngineClassData::CLASS_150CC;
                break;

            // Chances: game-mode dependent
            // - Regular VS: 40% 150cc, 40% 200cc, 20% 500cc
            // TODO add support for other game modes
            case CupManager::TRACKS_FUSION:
                randNum = rnd.nextU32(100);
                if (randNum > 79) {
                    engineClass = RKNetEngineClassData::CLASS_500CC;
                } else if (randNum > 39) {
                    engineClass = RKNetEngineClassData::CLASS_200CC;
                } else {
                    engineClass = RKNetEngineClassData::CLASS_150CC;
                }
                break;
        }

        // Decide whether mirror mode should be enabled for VS
        // Chances: 90% Normal, 10% Mirror
        // TODO make the chances configurable instead of hardcoding them here
        // TODO prevent overriding the flag if explicitly set in the settings
        randNum = rnd.nextU32(100);
        if (randNum > 89) {
            mirror = true;
        }
    }

    // Set the data
    sendPacket.engineClass.engineClass = engineClass;
    sendPacket.engineClass.isMirror = mirror;
}

// Replace the track vote picking algorithm
REPLACE void RKNetSELECTHandler::decideTrack() {

    // Check if battle
    const bool isBattle = (mode == MODE_PRIVATE_BATTLE || mode == MODE_PUBLIC_BATTLE);

    // Bail if no one is connected
    const RKNetController::Sub* sub = RKNetController::instance->getCurrentSub();
    if (sub->playerCount <= sub->localPlayerCount) return;

    // Collect every player's vote
    for (int i = 0; i < 12; i++) {

        // Check if AID is available
        if (sub->availableAids & (1 << i)) {

            // Get the vote and ensure it is valid
            u16 track = getPlayerVote(i);
            const u16 trackCount = CupManager::GetTrackCount(isBattle);
            track = (track < trackCount) ? track : CupData::RANDOM_TRACK_VOTE;

            // Add it to the repick manager
            RepickQueue::instance.AddVote(i, track);
        }
    }

    // Get the winning vote
    const RepickQueue::Vote vote = RepickQueue::instance.GetWinningVote();

    // Get the track, and if it's random then pick a random track
    u16 track = vote.track;
    if (track == CupData::RANDOM_TRACK_VOTE) {

        // Initialize loop
        Random randomizer;
        const CupData::CupList* cupList = CupManager::GetCurrentCupList(isBattle);
        LOG_DEBUG("Player voted random, picking a track...");

        // Ensure the random track isn't in the repick queue
        do {
            const CupData::Cup* cup = &cupList->cups[randomizer.nextU32(cupList->cupCount)];
            track = cup->entryId[randomizer.nextU32(4)];
        } while (RepickQueue::instance.GetQueuePosition(track) != RepickQueue::NOT_IN_QUEUE);

        LOG_DEBUG("Got a random track, picked %d.", track);
    }

    // Set the data and add the track to the queue
    sendPacket.winningVoterAid = vote.aid;
    expansion.sendPacketEx.winningCourse = track;
    RepickQueue::instance.Push(track);

    // Clear the votes as well
    RepickQueue::instance.ClearVotes();
}

// Send the expanded packet information
REPLACE void RKNetSELECTHandler::prepareSendPacket(u8 aid, s64 sendTime) {

    // If there are no unprocessed packets, bail
    if (!hasUnprocessedRecvPackets())
        return;

    // Update the timers (replicated code)
    sendPacket.timeSender = OSTicksToMilliseconds(sendTime);
    const s64 timeSent = recvPackets[aid].timeSender;
    if (timeSent == 0) {
        sendPacket.timeReceived = 0;
    } else {
        sendPacket.timeReceived = OSTicksToMilliseconds(sendTime - lastRecvTimes[aid]) + timeSent;
    }

    // If we are not host, bail
    if (!RKNetController::instance->isPlayerHost())
        return;

    // If the settings have not been decided, do so
    if (!raceSettingsDetermined()) {
        LOG_DEBUG("Deciding settings...");
        decideBattleAndTeams();
        decideSelectId();
        decideEngineClass();
        copyRepickQueue();
    }

    // If everyone has voted and the track has not been decided, do so
    if (checkVoteAll() && expansion.sendPacketEx.winningCourse == CupData::NO_TRACK) {
        decideTrack();
        decidePidToAidMap();
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
