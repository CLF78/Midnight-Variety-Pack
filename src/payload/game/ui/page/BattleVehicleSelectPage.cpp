#include <common/Common.hpp>
#include <game/ui/page/Page.hpp>

/////////////////////////
// Transmission Select //
/////////////////////////

// BattleVehicleSelectPage::onButtonClick() patches
// Go to transmission select instead of drift select
kmWrite16(0x8083AA12, Page::TRANSMISSION_SELECT);
kmWrite16(0x8083AA3E, Page::TRANSMISSION_SELECT);
