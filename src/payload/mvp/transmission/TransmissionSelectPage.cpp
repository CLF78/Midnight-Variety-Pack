#include <mvp/transmission/TransmissionSelectPage.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/ctrl/VehicleModelControl.hpp>
#include <game/util/Random.hpp>

void LoadTransmissionFromVehicle(MenuPage* menuPage, Page::PageId pageId, PushButton* button){
    pageId = Page::TRANSMISSION_SELECT;
    menuPage->loadNextPageById(pageId, button);
}
//Normal
kmCall(0x80846d2c, LoadTransmissionFromVehicle);
kmCall(0x80846d64, LoadTransmissionFromVehicle);
kmCall(0x80846e1c, LoadTransmissionFromVehicle);
kmCall(0x80846e40, LoadTransmissionFromVehicle);
//Battle
kmCall(0x8083aa20, LoadTransmissionFromVehicle);
kmCall(0x8083aa40, LoadTransmissionFromVehicle);

void LoadTransmissionFromDrift(MenuPage* menuPage, float delay){
    menuPage->prevPageId = Page::TRANSMISSION_SELECT;
    menuPage->loadPrevPageWithDelay(delay);
}
kmBranch(0x8084e700, LoadTransmissionFromDrift);

TransmissionSelectPage::TransmissionSelectPage(){
    nextPageId = Page::DRIFT_SELECT;
    prevPageId = Page::VEHICLE_SELECT;
    if (RaceConfig::instance->menuScenario.settings.isBattle()) prevPageId = Page::VEHICLE_SELECT_BT;
    SET_HANDLER_FUNC(onButtonClickHandler, onButtonClick);
    SET_HANDLER_FUNC(onButtonSelectHandler, onButtonSelect);
    SET_HANDLER_FUNC(onBackPressHandler, onBackPress);
}

void TransmissionSelectPage::onActivate(){
    const char* movies[] = {"thp/button/drift_select.thp"}; //todo create custom thps
    loadMovies(movies, ARRAY_SIZE(movies));

    for (int i = 0; i < this->buttonCount; i++){
        if (i == 0){
            this->buttons[i]->setText(Message::Menu::TRANSMISSION_INSIDE);
        }
        else if (i == 1){
            this->buttons[i]->setText(Message::Menu::TRANSMISSION_OUTSIDE);
        }
        else if (i == 2){
            this->buttons[i]->setText(Message::Menu::TRANSMISSION_HELP);
        }
    }
    this->titleBmgId = Message::Menu::TRANSMISSION_TITLE;
    MenuPage::onActivate();
    PushButton* lastSelectedButton = this->buttons[0]; //todo 
    this->setSelection(lastSelectedButton);
    u32 buttonId = lastSelectedButton->buttonId;
    if (buttonId == 0){
        this->instructionText->setText(Message::Menu::TRANSMISSION_INSIDE_BOTTOM);
    }
    else if(buttonId == 1){
        this->instructionText->setText(Message::Menu::TRANSMISSION_OUTSIDE_BOTTOM);
    }
    else if(buttonId == 2){
        this->instructionText->setText(Message::Menu::TRANSMISSION_HELP_BOTTOM);
    }
}

void TransmissionSelectPage::setupButton(PushButton* button){
    button->setOnClickHandler(&this->onButtonClickHandler, 0);
    button->setOnSelectHandler(&this->onButtonSelectHandler);
    button->setOnDeselectHandler(&this->onButtonDeselectHandler);
}

void TransmissionSelectPage::SetCPUVehicleType(){
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;
    if (scenario->settings.isOnline()) return;
    Random random;
    for (int i = 1; i < 12; i++){
        scenario->players[i].transmission = random.nextU32(2) + 1;
    }
}

void TransmissionSelectPage::onButtonClick(PushButton* button, u32){
    RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[0]; // 0 because this is SinglePlayer only
    switch (button->buttonId)
    {
        case 0:
            player->transmission = RaceConfig::Player::TRANSMISSION_INSIDE;
            this->loadNextPageById(Page::DRIFT_SELECT, button);
            SetCPUVehicleType();
            break;
        case 1:
            player->transmission = RaceConfig::Player::TRANSMISSION_OUTSIDE;
            this->loadNextPageById(Page::DRIFT_SELECT, button);
            SetCPUVehicleType();
            break;
        case 2:
            this->pushMessage(Message::Menu::TRANSMISSION_HELP_DESC);
            break;
        default:
            break;
    }
}

void TransmissionSelectPage::onButtonSelect(PushButton* button, u32 hudSlotId){
    u32 buttonId = button->buttonId;
    
    if (buttonId == 0){
        this->instructionText->setText(Message::Menu::TRANSMISSION_INSIDE_BOTTOM);
    }
    else if(buttonId == 1){
        this->instructionText->setText(Message::Menu::TRANSMISSION_OUTSIDE_BOTTOM);
    }
    else if(buttonId == 2){
        this->instructionText->setText(Message::Menu::TRANSMISSION_HELP_BOTTOM);
    }
}

void TransmissionSelectPage::onBackPress(u32 hudSlotId){
    this->loadPrevPageWithDelay(0.0f);
}

void FixVehicleModelTransition(VehicleModelControl* ctrl, Page::PageId id){ //todo use REPLACE to replace the functions hooked
    if(id == Page::TRANSMISSION_SELECT){
        id = Page::DRIFT_SELECT;
    }
    ctrl->setAnimationType(id);
};
kmCall(0x80847678, FixVehicleModelTransition);
kmCall(0x808476c8, FixVehicleModelTransition);
