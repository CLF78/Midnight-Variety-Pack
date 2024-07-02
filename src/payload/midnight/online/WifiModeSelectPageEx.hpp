#include <common/Common.hpp>
#include <game/ui/page/WifiModeSelectPage.hpp>
#include <midnight/cup/CupManager.hpp>

class WifiModeSelectPageEx : public WifiModeSelectPage {
public:
    WifiModeSelectPageEx() : extraRaceButtons() {}
    virtual ~WifiModeSelectPageEx() {}

    virtual void onInit();
    virtual void onActivate();

    int getButtonCount() {
        return 2 + ARRAY_SIZE(extraRaceButtons);
    }

    PushButton* getButton(int idx) {
        if (idx == 0)
            return &raceButton;
        else if (idx == 1)
            return &battleButton;
        else if (idx - 2 < ARRAY_SIZE(extraRaceButtons))
            return &extraRaceButtons[idx - 2];
        return nullptr;
    }

    PushButton extraRaceButtons[CupManager::TRACKS_TOTAL_COUNT - 2];
};
