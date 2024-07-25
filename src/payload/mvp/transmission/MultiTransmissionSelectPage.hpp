#include <common/Common.hpp>
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

    void onButtonClick(PushButton* button, u32 hudSlotId);
    void setCPUTransmissions();
};
