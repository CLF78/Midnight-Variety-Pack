#include <game/ui/Page.hpp>
#include <game/ui/page/MultiDriftSelectPage.hpp>
#include <game/ui/PushButton.hpp>

class MultiTransmissionSelectPage : public MultiDriftSelectPage {
public:
    static const Page::PageID id = Page::TRANSMISSION_SELECT_MULTI_PLAYER;
    MultiTransmissionSelectPage();
    virtual ~MultiTransmissionSelectPage(){};
    virtual void onActivate();
    virtual void setupButton(PushButton* pushButton);
    //virtual void beforeCalc();
private:
    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onBckPress(u32 hudSlotId);
    void SetCPUVehicleType();
};
