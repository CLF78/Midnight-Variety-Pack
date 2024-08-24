#include "Page.hpp"

/////////////////////////
// Transmission Select //
/////////////////////////

// MultiVehicleSelectPage::onButtonClick() patches
// Go to transmission select instead of drift select
kmWrite16(0x8084A24A, Page::TRANSMISSION_SELECT_MULTI_PLAYER);
kmWrite16(0x8084A216, Page::TRANSMISSION_SELECT_MULTI_PLAYER);
