#include "VehicleModelControl.hpp"
#include <game/ui/page/Page.hpp>

/////////////////////////
// Transmission Select //
/////////////////////////

// Play animation when the transmission is selected
REPLACE void VehicleModelControl::setAnimationType(Page::PageId pageId) {

    switch (pageId) {
        case Page::DRIFT_SELECT:
        case Page::CUP_SELECT:
        case Page::DRIFT_SELECT_MULTI_PLAYER:
        case Page::TRANSMISSION_SELECT:
        case Page::TRANSMISSION_SELECT_MULTI_PLAYER:
            animationType = 1;
            break;

        default:
            animationType = 0;
            break;
    }
}
