#include <game/ui/page/Page.hpp>
#include <game/ui/page/MultiDriftSelectPage.hpp>
#include <game/ui/ctrl/PushButton.hpp>

class MultiTransmissionSelectPage : public MultiDriftSelectPage {
public:
    static const Page::PageId id = Page::TRANSMISSION_SELECT_MULTI_PLAYER;
    MultiTransmissionSelectPage();
    virtual ~MultiTransmissionSelectPage(){};
    virtual void onActivate();
    virtual void setupButton(PushButton* pushButton);
    //virtual void beforeCalc();
private:
    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onBackPress(u32 hudSlotId);
    void SetCPUVehicleType();
};
