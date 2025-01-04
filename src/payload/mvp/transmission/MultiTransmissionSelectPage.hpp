#pragma once
#include <game/ui/page/MultiDriftSelectPage.hpp>

class MultiTransmissionSelectPage : public MultiDriftSelectPage {
public:
    enum ButtonId {
        BUTTON_OUTSIDE_P1,
        BUTTON_INSIDE_P1,
        BUTTON_OUTSIDE_P2,
        BUTTON_INSIDE_P2,
        BUTTON_OUTSIDE_P3,
        BUTTON_INSIDE_P3,
        BUTTON_OUTSIDE_P4,
        BUTTON_INSIDE_P4,
        BUTTON_COUNT,
    };

    MultiTransmissionSelectPage();
    virtual ~MultiTransmissionSelectPage() {}

    virtual void onActivate();
    virtual void afterCalc();

    static MultiTransmissionSelectPage* getPage() {
        return (MultiTransmissionSelectPage*)
            SectionManager::instance->curSection->getPage(Page::TRANSMISSION_SELECT_MULTI_PLAYER);
    }

    void onButtonClick(PushButton* button, u32 hudSlotId);
    static void setCPUTransmissions();
};
