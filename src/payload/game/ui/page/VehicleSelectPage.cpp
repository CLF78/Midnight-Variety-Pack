#include "Page.hpp"

/////////////////////////
// Transmission Select //
/////////////////////////

// VehicleSelectPage::onButtonClick() patches
// Go to transmission select instead of drift select
kmWrite16(0x80846D22, Page::TRANSMISSION_SELECT);
kmWrite16(0x80846D5E, Page::TRANSMISSION_SELECT);
kmWrite16(0x80846E1A, Page::TRANSMISSION_SELECT);
kmWrite16(0x80846E3E, Page::TRANSMISSION_SELECT);
