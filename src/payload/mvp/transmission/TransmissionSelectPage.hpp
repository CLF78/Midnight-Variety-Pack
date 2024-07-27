#include <common/Common.hpp>
#include <game/ui/page/DriftSelectPage.hpp>

class TransmissionSelectPage : public DriftSelectPage {
public:
    enum ButtonId {
        BUTTON_INSIDE,
        BUTTON_OUTSIDE,
        BUTTON_HELP,
        BUTTON_COUNT,
    };

    TransmissionSelectPage();
    virtual ~TransmissionSelectPage() {}

    virtual void onActivate();
    virtual void onSelectChange(PushButton* button, u32 hudSlotId);

    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onBackPress(u32 hudSlotId);

    void setCPUTransmissions();
};
