#include <game/ui/page/FriendRoomJoinPage.hpp>
#include <mvp/transmission/MultiTransmissionSelectPage.hpp>
#include <mvp/transmission/TransmissionSelectPage.hpp>

///////////////////////
// Always Show Timer //
///////////////////////

// Always display the timer when selecting the vehicle combo after a friend room start
kmWrite32(0x805DDBA0, 0x60000000);

/////////////////////////
// Transmission Select //
/////////////////////////

// Store the timer reference in the transmission selection pages
REPLACE void FriendRoomJoinPage::startRoom() {
    REPLACED();

    if (TransmissionSelectPage* transmissionSelectPage = TransmissionSelectPage::getPage()) {
        transmissionSelectPage->timer = &timer;
    }

    if (MultiTransmissionSelectPage* multiTransmissionSelectPage = MultiTransmissionSelectPage::getPage()) {
        multiTransmissionSelectPage->timer = &timer;
    }
}
