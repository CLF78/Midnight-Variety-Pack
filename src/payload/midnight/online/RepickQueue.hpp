#include <common/Common.hpp>
#include <platform/string.h>

class RepickQueue {
public:

    enum {
        NOT_IN_QUEUE = 0xFF,
        NO_PICK = 0xFFFF,
    };

    struct Vote {
        Vote() { Clear(); }

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

    RepickQueue() { Clear(); }

    void Clear();

    void ClearVotes();
    void AddVote(u8 aid, u16 vote);

    void Push(u16 track);
    u8 GetQueuePosition(u16 track);
    Vote GetWinningVote();

    u16 lastPicks[32];
    Vote votes[12];
    u8 voteCount;

    static RepickQueue instance;
};
