#include <common/Common.hpp>
#include <game/ui/Page.hpp>
#include <game/ui/SectionManager.hpp>

class RandomMatchingPage : public Page {
public:
    void setupGameMode(bool isBattle);

    static RandomMatchingPage* getPage() {
        return (RandomMatchingPage*)SectionManager::instance->curSection->pages[Page::GLOBAL_SEARCH_MANAGER];
    }
};
