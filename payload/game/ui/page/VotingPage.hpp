#include <common/Common.hpp>

class VotingPage {
public:
    void submitVote(u32 courseId);

    static VotingPage* getPage() {
        return (VotingPage*)SectionManager::instance->curSection->pages[Page::WIFI_VOTING];
    }
};
