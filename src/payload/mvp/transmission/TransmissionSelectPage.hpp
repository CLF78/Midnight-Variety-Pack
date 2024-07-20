#include <game/ui/Page.hpp>
#include <game/ui/page/DriftSelectPage.hpp>
#include <game/ui/PushButton.hpp>


class TransmissionSelectPage : public DriftSelectPage{
public:
    static const Page::PageID id = Page::TRANSMISSION_SELECT;
    TransmissionSelectPage();
    virtual ~TransmissionSelectPage(){}
    virtual void onActivate();
    //virtual void beforeCalc(); // todo when random combo exists
    virtual void setupButton(PushButton* pushButton);
private:
    void onButtonClick(PushButton* button, u32 hudSlotId);
    void onButtonSelect(PushButton* button, u32 hudSlotId);
    void onBackPress(u32 hudSlotId);
    void SetCPUVehicleType();
};
