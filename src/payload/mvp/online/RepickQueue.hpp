#pragma once
#include <platform/string.h>

class RepickQueue {
public:

    enum {
        NOT_IN_QUEUE = 0xFF,
        NO_PICK = 0xFFFF,
    };

    struct RawQueue {
        RawQueue() { Clear(); }

        bool operator==(const RawQueue& queue) const {
            for (u32 i = 0; i < ARRAY_SIZE(lastPicks); i++) {
                if (lastPicks[i] != queue.lastPicks[i])
                    return false;
            }

            return true;
        }

        void Clear();
        void Push(u16 track);
        u8 GetQueuePosition(u16 track) const;

        u16 lastPicks[32];
    };

    struct Vote {

        void Clear() {
            track = NO_PICK;
            aid = 0xFF;
            queuePos = NOT_IN_QUEUE;
        }

        void Set(u16 course, u8 player) {
            track = course;
            aid = player;
            queuePos = instance.GetQueuePosition(course);
        }

        u16 track;
        u8 aid;
        u8 queuePos;
    };

    void Clear() { queue.Clear(); ClearVotes(); }

    void Push(u16 track) { queue.Push(track); }
    u8 GetQueuePosition(u16 track) const { return queue.GetQueuePosition(track); }

    void ClearVotes();
    void AddVote(u8 aid, u16 vote);
    Vote GetWinningVote() const;

    RawQueue queue;
    Vote votes[12];
    u8 voteCount;

    static RepickQueue instance;
};
