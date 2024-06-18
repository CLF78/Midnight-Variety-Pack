#include <common/Common.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/online/RepickQueue.hpp>

RepickQueue RepickQueue::instance;

void RepickQueue::RawQueue::Clear() {
    for (int i = 0; i < ARRAY_SIZE(lastPicks); i++) {
        lastPicks[i] = NO_PICK;
    }
}

void RepickQueue::RawQueue::Push(u16 track) {

    // Only add the track if it's not already there
    if (GetQueuePosition(track) == NOT_IN_QUEUE) {
        LOG_DEBUG("Pushing track %d to the queue...", track);
        memmove(&lastPicks[1], &lastPicks, sizeof(lastPicks) - sizeof(lastPicks[0]));
        lastPicks[0] = track;
    } else {
        LOG_DEBUG("Track %d already in queue, not pushing.", track);
    }
}

u8 RepickQueue::RawQueue::GetQueuePosition(u16 track) {

    // Treat random track votes as a track not in the queue
    if (track == CupData::RANDOM_TRACK_VOTE) return NOT_IN_QUEUE;

    // Get the queue position
    for (int i = 0; i < ARRAY_SIZE(lastPicks); i++) {
        if (lastPicks[i] == track) return i;
    }

    // Track isn't in the queue
    return NOT_IN_QUEUE;
}

void RepickQueue::ClearVotes() {
    for (int i = 0; i < ARRAY_SIZE(votes); i++) {
        votes[i].Clear();
    }

    voteCount = 0;
}

void RepickQueue::AddVote(u8 aid, u16 vote) {
    if (voteCount < 12) {
        LOG_DEBUG("Adding vote %d by AID %d...", vote, aid);
        votes[voteCount++].Set(vote, aid);
    }
}

RepickQueue::Vote RepickQueue::GetWinningVote() {

    // Get the maximum queue position and the amount of votes with said queue position
    u8 maxPos = 0;
    u8 maxPosCount = 0;
    for (int i = 0; i < voteCount; i++) {

        u8 queuePos = votes[i].queuePos;
        if (queuePos > maxPos) {
            maxPos = votes[i].queuePos;
            maxPosCount = 1;

        } else if (queuePos == maxPos) {
            maxPosCount++;
        }
    }

    LOG_DEBUG("Found %d votes with queue position %d", maxPosCount, maxPos);

    // Isolate the maximum queue position votes
    Vote* maxVotes = (Vote*)__alloca(sizeof(Vote)* maxPosCount);
    int copiedVotes = 0;
    for (int i = 0; i < voteCount; i++) {
        if (votes[i].queuePos == maxPos)
            maxVotes[copiedVotes++] = votes[i];
    }

    // Get a random entry from the selected votes
    Random randomizer;
    Vote selectedVote = maxVotes[randomizer.nextU32(maxPosCount)];
    LOG_DEBUG("Selected Voter: %d, Selected Track: %d", selectedVote.aid, selectedVote.track);

    return selectedVote;
}
