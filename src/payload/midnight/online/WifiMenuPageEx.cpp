#include <common/Common.hpp>
#include <game/sound/SoundEffect.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/page/MessagePopupPage.hpp>
#include <midnight/online/WifiMenuPageEx.hpp>
#include <wiimmfi/Auth.hpp>
#include <wiimmfi/Status.hpp>

/////////////////////////////
// Remove Worldwide Button //
/////////////////////////////

void WifiMenuPageEx::onInit() {

    // Initialize the input manager
    switch (SectionManager::instance->curSection->sectionID) {
        case Section::WIFI_MENU_2P:
        case Section::WIFI_MENU_2P_FROM_DC:
        case Section::WIFI_MENU_2P_FROM_FRIENDLIST:
            inputManager.init(1, true);
            break;

        default:
            inputManager.init(1, false);
            break;
    }

    // Set button looping
    setInputManager(&inputManager);
    inputManager.setDistanceFunc(MultiControlInputManager::Y_WRAP);

    // Initialize children
    initChildren(5);
    int curChildIdx = 0;

    // Add the title text
    insertChild(curChildIdx++, &titleText, 0);
    titleText.load(false);

    // Set the title text
    switch (SectionManager::instance->curSection->sectionID) {
        case Section::WIFI_MENU_1P:
        case Section::WIFI_MENU_1P_FROM_DC:
        case Section::WIFI_MENU_1P_FROM_FRIENDLIST:
            titleText.setText(Message::Menu::TITLE_TEXT_WIFI_MAIN_MENU_1P, nullptr);
            break;

        case Section::WIFI_MENU_2P:
        case Section::WIFI_MENU_2P_FROM_DC:
        case Section::WIFI_MENU_2P_FROM_FRIENDLIST:
            titleText.setText(Message::Menu::TITLE_TEXT_WIFI_MAIN_MENU_2P, nullptr);
            break;
    }

    // Add the worldwide button
    insertChild(curChildIdx++, &worldwideButton, 0);
    worldwideButton.load("button", "WifiMenuSingleTop", "ButtonWorld", 1, false, false);
    worldwideButton.buttonId = BTN_WORLDWIDE;

    // Replace the handler with the regional one
    worldwideButton.setOnClickHandler(&onRegionalButtonPressed, 0);
    worldwideButton.setOnSelectHandler(&onButtonSelect);

    // Add the friends button
    insertChild(curChildIdx++, &friendButton, 0);
    friendButton.loadWithAnims(FriendStatusButton::animNames, "button",
                               "WifiMenuSingleTopFriendButton", "ButtonFriend", 1, false);

    // Set animations and other things
    friendButton.animator.getGroup(4)->setAnimation(0, 0.0f);
    friendButton.animator.getGroup(5)->setAnimation(0, 0.0f);
    friendButton.miiGroup = nullptr;
    friendButton.friendIndex = -1;
    friendButton.buttonId = BTN_FRIENDS;

    // Set handlers
    friendButton.setOnClickHandler(&onFriendButtonPressed, 0);
    friendButton.setOnSelectHandler(&onButtonSelect);

    // Add the back button
    insertChild(curChildIdx++, &backButton, 0);
    backButton.load("button", "Back", "ButtonBack", 1, false, true);
    backButton.buttonId = BTN_BACK;

    // Set handlers
    backButton.setOnClickHandler(&onBackButtonPressed, 0);
    backButton.setOnSelectHandler(&onButtonSelect);
    inputManager.setHandler(MultiControlInputManager::BACK_PRESS, &onBack, false, false);

    // Add the instruction text
    insertChild(curChildIdx++, &instructionText, 0);
    instructionText.load();
    instructionText.setText(Message::Menu::INSTRUCTION_TEXT_WORLDWIDE, nullptr);

    // Set the default selection
    worldwideButton.selectDefault(0);

    // Final code
    SectionManager::instance->curSection->loadFriendListManager();
    setAnimSfxIds(SE_DUMMY, SE_DUMMY);
}

////////////////////////////////
// Wiimmfi Console Assignment //
////////////////////////////////

// Display the Wiimmfi console assignment message if available
// Credits: Wiimmfi
void WifiMenuPageEx::beforeCalc() {

    // Check if a message is available, if not bail
    if (!Wiimmfi::Auth::sConsoleAssignMessage)
        return;

    // Get the message popup page, if not available bail
    MessagePopupPage* page = MessagePopupPage::getPage();
    if (!page) return;

    // Inform the server we have seen the message
    Wiimmfi::Status::SendMessage("message", "type=server");

    // Display the message
    MessageInfo msgInfo;
    msgInfo.strings[0] = Wiimmfi::Auth::sConsoleAssignMessage;
    page->setWindowMessage(Message::Menu::STRING_FORMATTER, &msgInfo);

    // Display the page and delete the pointer so this doesn't run again next frame
    addPage(Page::MESSAGE_POPUP, Page::ANIM_NEXT);
    Wiimmfi::Auth::sConsoleAssignMessage = nullptr;
}
