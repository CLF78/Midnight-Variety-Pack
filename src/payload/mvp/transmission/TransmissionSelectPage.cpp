#include "TransmissionSelectPage.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/ctrl/CtrlMenuMovieButton.hpp>
#include <game/util/Random.hpp>
#include <mvp/save/SaveExpansionDrift.hpp>

TransmissionSelectPage::TransmissionSelectPage() {

    // Set handlers
    SET_HANDLER_FUNC(onButtonClickHandler, TransmissionSelectPage::onButtonClick);
    SET_HANDLER_FUNC(onBackPressHandler, TransmissionSelectPage::onBackPress);

    // Set previous page
    prevPageId = Page::VEHICLE_SELECT;
    if (RaceConfig::instance->menuScenario.settings.isBattle()) {
        prevPageId = Page::VEHICLE_SELECT_BT;
    }
}

void TransmissionSelectPage::onActivate() {

    // Load custom movie
    // TODO make actual custom movies
    Section::SectionId section = (Section::SectionId)SectionManager::instance->curSection->sectionId;
    if (Section::getSceneId(section) != Scene::SCENE_GLOBE) {
        static const char* movies[] = { "thp/button/drift_select.thp" };
        loadMovies(movies, ARRAY_SIZE(movies));
    }

    // Set the title text and activate the page
    titleBmgId = Message::Menu::TRANSMISSION_TITLE;
    MenuPage::onActivate();

    // Set the default selection
    u32 vehicle = SectionManager::instance->globalContext->playerVehicles[0];
    u8 defaultTransmission = SaveExpansionDrift::GetSection()->GetData(0)->Get(vehicle);
    u8 buttonIdx = (defaultTransmission == RaceConfig::Player::TRANSMISSION_INSIDE) ? BUTTON_INSIDE :
                                                                                      BUTTON_OUTSIDE;
    setSelection(buttons[buttonIdx]);
}

PushButton* TransmissionSelectPage::loadButton(int buttonIdx) {

    // Load help button
    if (buttonIdx == BUTTON_HELP) {
        PushButton* button = new PushButton();
        insertChild(curChildCount++, button, 0);
        button->load("button", "DriftHelp", "DriftHelp", activePlayers, false, false);
        return button;
    }

    // Get scene id
    Section::SectionId sectionId = (Section::SectionId)SectionManager::instance->curSection->sectionId;
    Scene::SceneId sceneId = Section::getSceneId(sectionId);
    static const char* buttonVariants[2] = { "ButtonInside", "ButtonOutside" };

    // Load online-specific buttons
    if (sceneId == Scene::SCENE_GLOBE) {
        PushButton* button = new PushButton();
        insertChild(curChildCount++, button, 0);
        button->load("button", "GlobeTransmissionSelect", buttonVariants[buttonIdx], activePlayers, false,
                     false);
        return button;
    }

    // Else load regular buttons
    CtrlMenuMovieButton* button = new CtrlMenuMovieButton();
    insertChild(curChildCount++, button, 0);
    button->load("button", "TransmissionSelect", buttonVariants[buttonIdx], activePlayers, false, false);
    button->setMovieCrop("black_base", ((float)buttonIdx) * 0.5f, ((float)(buttonIdx + 1)) * 0.5f, 0.0f,
                         1.0f);
    return button;
}

void TransmissionSelectPage::afterCalc() {

    // Check if the page is active
    if (pageState != STATE_ACTIVE || timer == nullptr) {
        return;
    }

    // Check that the timer has ended
    if (timer->value > 0.0f) {
        return;
    }

    // Get the current selected button (or pick the default selection if none is selected)
    PushButton* selectedButton = nullptr;
    if (buttons[BUTTON_INSIDE]->isSelected()) {
        selectedButton = buttons[BUTTON_INSIDE];
    }
    else if (buttons[BUTTON_OUTSIDE]->isSelected()) {
        selectedButton = buttons[BUTTON_OUTSIDE];
    }
    else {
        u32 vehicle = SectionManager::instance->globalContext->playerVehicles[0];
        u8 defaultTransmission = SaveExpansionDrift::GetSection()->GetData(0)->Get(vehicle);
        u8 buttonIdx = (defaultTransmission == RaceConfig::Player::TRANSMISSION_INSIDE) ? BUTTON_INSIDE :
                                                                                          BUTTON_OUTSIDE;
        selectedButton = buttons[buttonIdx];
    }

    // Select the button and force-click it
    onSelectChange(selectedButton, 0);
    selectedButton->select(0);
    selectedButton->click(0);
}

void TransmissionSelectPage::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Get the first player, as this is a single player page
    RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[0];
    u32 vehicle = SectionManager::instance->globalContext->playerVehicles[0];
    switch (button->buttonId) {

        case BACK_BUTTON:
            loadPrevPage(button);
            break;

        case BUTTON_INSIDE:
            player->transmission = RaceConfig::Player::TRANSMISSION_INSIDE;
            SaveExpansionDrift::GetSection()->GetData(0)->Set(vehicle, player->transmission);
            loadNextPageById(Page::DRIFT_SELECT, button);
            setCPUTransmissions();
            break;

        case BUTTON_OUTSIDE:
            player->transmission = RaceConfig::Player::TRANSMISSION_OUTSIDE;
            SaveExpansionDrift::GetSection()->GetData(0)->Set(vehicle, player->transmission);
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
    }
    else {
        instructionText->setText(instructionTexts[button->buttonId]);
    }
}

void TransmissionSelectPage::setCPUTransmissions() {

    // Skip if online
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;
    if (scenario->settings.isOnline()) {
        return;
    }

    // Set a random transmission for every CPU
    Random random;
    for (int i = 1; i < 12; i++) {
        scenario->players[i].transmission = random.nextU32(2) + 1;
    }
}
