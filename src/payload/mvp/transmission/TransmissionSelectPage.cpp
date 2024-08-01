#include "TransmissionSelectPage.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/ui/Message.hpp>
#include <game/util/Random.hpp>

TransmissionSelectPage::TransmissionSelectPage() {

    // Set handlers
    SET_HANDLER_FUNC(onButtonClickHandler, onButtonClick);
    SET_HANDLER_FUNC(onBackPressHandler, onBackPress);

    // Set previous page
    prevPageId = Page::VEHICLE_SELECT;
    if (RaceConfig::instance->menuScenario.settings.isBattle())
        prevPageId = Page::VEHICLE_SELECT_BT;
}

void TransmissionSelectPage::onActivate() {

    // Load custom movie
    // TODO make actual custom movies
    static const char* movies[] = {"thp/button/drift_select.thp"};
    loadMovies(movies, ARRAY_SIZE(movies));

    // Set button texts
    static const u32 buttonTexts[BUTTON_COUNT] = {
        Message::Menu::TRANSMISSION_INSIDE,
        Message::Menu::TRANSMISSION_OUTSIDE,
        Message::Menu::HELP,
    };

    for (int i = 0; i < ARRAY_SIZE(buttonTexts); i++) {
        buttons[i]->setText(buttonTexts[i]);
    }

    // Set the title text and activate the page
    titleBmgId = Message::Menu::TRANSMISSION_TITLE;
    MenuPage::onActivate();

    // Set the default selection
    // TODO decide how to select the default button
    PushButton* lastSelectedButton = buttons[BUTTON_INSIDE];
    setSelection(lastSelectedButton);
}

void TransmissionSelectPage::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Get the first player, as this is a single player page
    RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[0];
    switch (button->buttonId) {

        case BACK_BUTTON:
            loadPrevPage(button);
            break;

        case BUTTON_INSIDE:
            player->transmission = RaceConfig::Player::TRANSMISSION_INSIDE;
            loadNextPageById(Page::DRIFT_SELECT, button);
            setCPUTransmissions();
            break;

        case BUTTON_OUTSIDE:
            player->transmission = RaceConfig::Player::TRANSMISSION_OUTSIDE;
            loadNextPageById(Page::DRIFT_SELECT, button);
            setCPUTransmissions();
            break;

        case BUTTON_HELP:
            pushMessage(Message::Menu::TRANSMISSION_HELP_DESC);
            break;

        default:
            break;
    }
}

void TransmissionSelectPage::onBackPress(u32 hudSlotId) {
    loadPrevPageWithDelay(0.0f);
}

void TransmissionSelectPage::onSelectChange(PushButton* button, u32 hudSlotId) {

    static const u32 instructionTexts[BUTTON_COUNT] = {
        Message::Menu::TRANSMISSION_INSIDE_BOTTOM,
        Message::Menu::TRANSMISSION_OUTSIDE_BOTTOM,
        Message::Menu::TRANSMISSION_HELP_BOTTOM,
    };

    if (button->buttonId == BACK_BUTTON) {
        instructionText->setText(Message::NONE);
    } else {
        instructionText->setText(instructionTexts[button->buttonId]);
    }
}

void TransmissionSelectPage::setCPUTransmissions() {

    // Skip if online
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;
    if (scenario->settings.isOnline())
        return;

    // Set a random transmission for every CPU
    Random random;
    for (int i = 1; i < 12; i++) {
        scenario->players[i].transmission = random.nextU32(2) + 1;
    }
}
