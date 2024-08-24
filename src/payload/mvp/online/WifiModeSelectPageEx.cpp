#include "WifiModeSelectPageEx.hpp"
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/RandomMatchingPage.hpp>
#include <mvp/save/SaveExpansionRating.hpp>
#include <platform/stdio.h>

WifiModeSelectPageEx::WifiModeSelectPageEx() {
    SET_HANDLER_FUNC(onButtonClickHandler, WifiModeSelectPageEx::onButtonClick);
}

void WifiModeSelectPageEx::onInit() {

    // Initialize the input manager
    switch (SectionManager::instance->curSection->sectionId) {
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
    for (u32 i = 0; i < getButtonCount(); i++) {

        // Get the button and insert it
        PushButton* btn = getButton(i);
        insertChild(curChildIdx++, btn, 0);

        // Load the layout
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "Button%d", i);
        btn->load("button", "WifiMenuModeSelect", buffer, 1, false, false);

        // Set button ID and handlers
        btn->buttonId = (int)(i + 1);
        btn->setOnClickHandler(&onButtonClickHandler, 0);
        btn->setOnSelectHandler(&onButtonSelectHandler);

        // Set tracklist name
        const u16 cupListName = CupManager::GetCupList(i)->cupListName;
        btn->setMatText("text", cupListName, nullptr);
        btn->setMatText("text_light_01", cupListName, nullptr);
        btn->setMatText("text_light_02", cupListName, nullptr);

        // Set rating message
        MessageInfo msgInfo;
        msgInfo.intVals[0] = SaveExpansionRating::GetSection()->GetData(i)->mRating;
        const u32 msgId = (i >= CupManager::TRACKS_VS_COUNT) ? Message::Menu::BATTLE_RATING :
                                                               Message::Menu::RACE_RATING;
        btn->setMatText("go", msgId, &msgInfo);
    }

    // Add the back button
    insertChild(curChildIdx++, &backButton, 0);
    backButton.load("button", "Back", "ButtonBack", 1, false, true);
    backButton.buttonId = (int)(getButtonCount() + 1);

    // Set handlers
    backButton.setOnClickHandler(&onBackButtonClickHandler, 0);
    backButton.setOnSelectHandler(&onButtonSelectHandler);
    inputManager.setHandler(MultiControlInputManager::BACK_PRESS, &onBackHandler, false, false);

    // Add the instruction text
    insertChild(curChildIdx++, &instructionText, 0);
    instructionText.load();

    // Set the default selection
    // TODO update instruction text correctly
    getButton(0)->selectDefault(0);
    instructionText.setText(4314);
}

// Update the title and instruction text correctly
void WifiModeSelectPageEx::onActivate() {

    // Fix the title text
    titleText.setText(Message::Menu::WORLDWIDE);

    // TODO update instruction text correctly
    if (animId == ANIM_NEXT) {
        getButton(0)->selectDefault(0);
        instructionText.setText(4314);
    }

    // Set replacement page to NONE
    replacementPage = Page::NONE;
}

// Update the tracklist and region correctly
void WifiModeSelectPageEx::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Get RandomMatchingPage
    RandomMatchingPage* page = RandomMatchingPage::getPage();
    const u8 cupList = button->buttonId - 1;

    // Set the online region, cup list and RaceConfig game mode accordingly
    CupManager::currentOnlineRegion = CupManager::GetCupList(cupList)->onlineRegion;
    if (cupList >= CupManager::TRACKS_VS_COUNT) {
        CupManager::currentBattleCupList = cupList;
        page->setupGameMode(true);
    }
    else {
        CupManager::currentCupList = cupList;
        page->setupGameMode(false);
    }

    // Set the next page and go to it
    replacementPage = Page::GLOBAL_SEARCH_MANAGER;
    replace(ANIM_NEXT, button->getDelay());
}
