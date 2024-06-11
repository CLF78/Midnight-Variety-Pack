#include <common/Common.hpp>

///////////////////////
// Open Host Support //
///////////////////////

// WifiFriendMenuPage::onNewRoomFront() patch
// Allow opening rooms without having registered any friends
// Credits: WiiLink24
kmWrite32(0x8064D36C, 0x60000000);
