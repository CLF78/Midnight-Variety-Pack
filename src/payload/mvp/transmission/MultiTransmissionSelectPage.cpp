#include "MultiTransmissionSelectPage.hpp"
#include <game/system/RaceConfig.hpp>
#include <game/ui/Message.hpp>
#include <game/util/Random.hpp>
#include <mvp/save/SaveExpansionDrift.hpp>

MultiTransmissionSelectPage::MultiTransmissionSelectPage() {
    SET_HANDLER_FUNC(onButtonClickHandler, MultiTransmissionSelectPage::onButtonClick);
    prevPageId = Page::VEHICLE_SELECT_MULTI_PLAYER;
}

void MultiTransmissionSelectPage::onActivate() {

    // Set title and activate page
    titleBmgId = Message::Menu::TRANSMISSION_TITLE;
    MenuPage::onActivate();

    // For each active player, enable the buttons and set their text
    for (u32 i = 0; i < SectionManager::instance->globalContext->localPlayerCount; i++) {
        multiControlInputManager.players[i].enabled = true;

        u8 outsideBtnIdx = i * 2;
        u8 insideBtnIdx = i * 2 + 1;
        buttons[outsideBtnIdx]->setText(Message::Menu::TRANSMISSION_OUTSIDE);
        buttons[outsideBtnIdx]->hidden = false;
        buttons[outsideBtnIdx]->inputManager.unselectable = false;

        buttons[insideBtnIdx]->setText(Message::Menu::TRANSMISSION_INSIDE);
        buttons[insideBtnIdx]->hidden = false;
        buttons[insideBtnIdx]->inputManager.unselectable = false;

        RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[i];
        u8 defaultTransmission = SaveExpansionDrift::GetSection()->GetData(i)->Get(player->vehicleId);
        u8 buttonIdx = (defaultTransmission == RaceConfig::Player::TRANSMISSION_INSIDE) ? insideBtnIdx :
                                                                                          outsideBtnIdx;
        buttons[buttonIdx]->selectDefault(i);
    }
}

void MultiTransmissionSelectPage::afterCalc() {

    // Check if the page is active
    if (pageState != STATE_ACTIVE || timer != nullptr) {
        return;
    }

    // Check that the timer has ended
    if (timer->value > 0.0f) {
        return;
    }

    // Pick for each player
    for (u32 i = 0; i < SectionManager::instance->globalContext->localPlayerCount; i++) {

        // Skip players that have already picked
        if (!multiControlInputManager.players[i].enabled) {
            continue;
        }

        // Get the current selected button (or pick the default selection for the player if none is selected)
        PushButton* selectedButton = nullptr;
        u8 outsideBtnIdx = i * 2;
        u8 insideBtnIdx = i * 2 + 1;

        if (buttons[insideBtnIdx]->isSelected()) {
            selectedButton = buttons[insideBtnIdx];
        }
        else if (buttons[outsideBtnIdx]->isSelected()) {
            selectedButton = buttons[outsideBtnIdx];
        }
        else {
            RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[i];
            u8 defaultTransmission = SaveExpansionDrift::GetSection()->GetData(i)->Get(player->vehicleId);
            u8 buttonIdx = (defaultTransmission == RaceConfig::Player::TRANSMISSION_INSIDE) ? insideBtnIdx :
                                                                                              outsideBtnIdx;
            selectedButton = buttons[buttonIdx];
        }

        // Select the button and force-click it
        onSelectChange(selectedButton, i);
        selectedButton->selectFocus();
        selectedButton->click(i);
    }
}

void MultiTransmissionSelectPage::onButtonClick(PushButton* button, u32 hudSlotId) {

    // Handle the back button separately
    // Only allow the first player to go back using this button
    if (button->buttonId == BACK_BUTTON) {
        if (hudSlotId == 0) {
            loadPrevPage(button);
        }
    }

    // Regular handling
    else {
        RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[hudSlotId];
        switch (button->buttonId) {

            case BUTTON_OUTSIDE_P1:
            case BUTTON_OUTSIDE_P2:
            case BUTTON_OUTSIDE_P3:
            case BUTTON_OUTSIDE_P4:
                player->transmission = RaceConfig::Player::TRANSMISSION_OUTSIDE;
                SaveExpansionDrift::GetSection()->GetData(hudSlotId)->Set(player->vehicleId,
                                                                          player->transmission);
                buttons[button->buttonId + 1]->hidden = true;
                buttons[button->buttonId + 1]->inputManager.unselectable = true;
                break;

            case BUTTON_INSIDE_P1:
            case BUTTON_INSIDE_P2:
            case BUTTON_INSIDE_P3:
            case BUTTON_INSIDE_P4:
                player->transmission = RaceConfig::Player::TRANSMISSION_INSIDE;
                SaveExpansionDrift::GetSection()->GetData(hudSlotId)->Set(player->vehicleId,
                                                                          player->transmission);
                buttons[button->buttonId - 1]->hidden = true;
                buttons[button->buttonId - 1]->inputManager.unselectable = true;
                break;

            default:
                break;
        }

        // Check if all players have set their transmission, and if so go to the next page
        multiControlInputManager.players[hudSlotId].enabled = false;
        if (checkAllPlayersReady()) {
            loadNextPageById(Page::DRIFT_SELECT_MULTI_PLAYER, button);
            setCPUTransmissions();
        }
    }
}

void MultiTransmissionSelectPage::setCPUTransmissions() {

    // Skip if online
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;
    if (scenario->settings.isOnline()) {
        return;
    }

    // Set a random transmission for every CPU
    Random random;
    for (u32 i = SectionManager::instance->globalContext->localPlayerCount; i < 12; i++) {
        scenario->players[i].transmission = random.nextU32(2) + 1;
    }
}
