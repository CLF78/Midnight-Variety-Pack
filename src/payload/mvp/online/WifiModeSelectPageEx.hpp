#pragma once
#include <game/ui/page/WifiModeSelectPage.hpp>
#include <mvp/cup/CupManager.hpp>

class WifiModeSelectPageEx : public WifiModeSelectPage {
public:
    WifiModeSelectPageEx();
    virtual ~WifiModeSelectPageEx() {}

    virtual void onInit();
    virtual void onActivate();

    void onButtonClick(PushButton* button, u32 hudSlotId);

    static int getButtonCount() {
        return 2 + ARRAY_SIZE_STATIC(WifiModeSelectPageEx, extraRaceButtons);
    }

    PushButton* getButton(int idx) {
        if (idx == 0)
            return &raceButton;
        if (idx == 1)
            return &battleButton;
        if (idx - 2 < ARRAY_SIZE(extraRaceButtons))
            return &extraRaceButtons[idx - 2];
        return nullptr;
    }

    PushButton extraRaceButtons[CupManager::TRACKS_TOTAL_COUNT - 2];
};
