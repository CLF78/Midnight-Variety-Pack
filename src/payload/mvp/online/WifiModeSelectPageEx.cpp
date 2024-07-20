#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/RandomMatchingPage.hpp>
#include <mvp/online/WifiModeSelectPageEx.hpp>
#include <mvp/save/SaveExpansionRating.hpp>
#include <platform/stdio.h>

WifiModeSelectPageEx::WifiModeSelectPageEx() : extraRaceButtons() {
    onButtonPress.handle = (typeof(onButtonPress.handle))&WifiModeSelectPageEx::handleButtonPress;
}

void WifiModeSelectPageEx::onInit() {

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
    initChildren(3 + getButtonCount());
    int curChildIdx = 0;

    // Add the title text
    insertChild(curChildIdx++, &titleText, 0);
    titleText.load(false);

    // Add each button
    for (int i = 0; i < getButtonCount(); i++) {

        // Get the button and insert it
        PushButton* btn = getButton(i);
        insertChild(curChildIdx++, btn, 0);

        // Load the layout
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Button%d", i);
        btn->load("button", "WifiMenuModeSelect", buffer, 1, false, false);

        // Set button ID and handlers
        btn->buttonId = i + 1;
        btn->setOnClickHandler(&onButtonPress, 0);
        btn->setOnSelectHandler(&onButtonSelect);

        // Set tracklist name
        u16 cupListName = CupManager::GetCupListData(i)->cupListName;
        btn->setMatText("text", cupListName, nullptr);
        btn->setMatText("text_light_01", cupListName, nullptr);
        btn->setMatText("text_light_02", cupListName, nullptr);

        // Set rating message
        MessageInfo msgInfo;
        msgInfo.intVals[0] = SaveExpansionRating::GetSection()->GetData(i)->mRating;
        int msgId = (i >= CupManager::TRACKS_VS_COUNT) ?
                    Message::Menu::BATTLE_RATING :
                    Message::Menu::RACE_RATING;
        btn->setMatText("go", msgId, &msgInfo);
    }

    // Add the back button
    insertChild(curChildIdx++, &backButton, 0);
    backButton.load("button", "Back", "ButtonBack", 1, false, true);
    backButton.buttonId = getButtonCount() + 1;

    // Set handlers
    backButton.setOnClickHandler(&onBackButtonPress, 0);
    backButton.setOnSelectHandler(&onButtonSelect);
    inputManager.setHandler(MultiControlInputManager::BACK_PRESS, &onBack, false, false);

    // Add the instruction text
    insertChild(curChildIdx++, &instructionText, 0);
    instructionText.load();

    // Set the default selection
    // TODO update instruction text correctly
    getButton(0)->selectDefault(0);
    instructionText.setText(4314, nullptr);
}

// Update the title and instruction text correctly
void WifiModeSelectPageEx::onActivate() {

    // Fix the title text
    titleText.setText(Message::Menu::WORLDWIDE, nullptr);

    // TODO update instruction text correctly
    if (animId == ANIM_NEXT) {
        getButton(0)->selectDefault(0);
        instructionText.setText(4314, nullptr);
    }

    // Set replacement page to NONE
    replacementPage = Page::NONE;
}

// Update the tracklist and region correctly
void WifiModeSelectPageEx::handleButtonPress(PushButton* button) {

    // Get RandomMatchingPage
    RandomMatchingPage* page = RandomMatchingPage::getPage();
    u32 cupList = button->buttonId - 1;

    // Set the online region, cup list and RaceConfig game mode accordingly
    CupManager::currentOnlineRegion = CupManager::GetCupListData(cupList)->onlineRegion;
    if (cupList >= CupManager::TRACKS_VS_COUNT) {
        CupManager::currentBattleCupList = cupList;
        page->setupGameMode(true);
    } else {
        CupManager::currentCupList = cupList;
        page->setupGameMode(false);
    }

    // Set the next page and go to it
    replacementPage = Page::GLOBAL_SEARCH_MANAGER;
    replace(ANIM_NEXT, button->getDelay());
}
