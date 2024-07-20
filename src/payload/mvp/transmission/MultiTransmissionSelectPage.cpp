
#include <mvp/transmission/MultiTransmissionSelectPage.hpp>
#include <game/system/RaceConfig.hpp>
#include <game/ui/Message.hpp>
#include <game/ui/ctrl/VehicleModelControl.hpp>
#include <game/util/Random.hpp>

static void LoadMultiTransmissionFromKart(MenuPage* menuPage, Page::PageID pageId, float delay){
    pageId = Page::TRANSMISSION_SELECT_MULTI_PLAYER;
    menuPage->loadNextPageWithDelayById(pageId, delay);
}
kmCall(0x8084a218, LoadMultiTransmissionFromKart);
kmCall(0x8084a24c, LoadMultiTransmissionFromKart);

static void LoadMultiTransmissionFromDrift(MenuPage* menuPage, float delay){
    menuPage->prevPageId = Page::TRANSMISSION_SELECT_MULTI_PLAYER;
    menuPage->loadPrevPageWithDelay(delay);
}
kmCall(0x8084b8a8, LoadMultiTransmissionFromDrift);

MultiTransmissionSelectPage::MultiTransmissionSelectPage(){
    nextPageId = Page::DRIFT_SELECT_MULTI_PLAYER;
    prevPageId = Page::VEHICLE_SELECT_MULTI_PLAYER;
    SET_HANDLER_FUNC(onButtonClickHandler, onButtonClick);
    SET_HANDLER_FUNC(onBackPressHandler, onBackPress);
}

void MultiTransmissionSelectPage::onActivate(){
    for (int i = 0; i < this->buttonCount; i+=2){
        this->buttons[i]->setText(Message::Menu::TRANSMISSION_OUTSIDE);
        this->buttons[i+1]->setText(Message::Menu::TRANSMISSION_INSIDE);
    }
    this->titleBmgId = Message::Menu::TRANSMISSION_TITLE;
    MenuPage::onActivate();
    for (int i = 0; i < SectionManager::instance->globalContext->humanPlayerCount; i++)
    {
        this->multiControlInputManager.players[i].enabled = true;
        this->buttons[i*2]->hidden = false;
        this->buttons[i*2]->inputManager.unselectable = false;
        this->buttons[i*2+1]->hidden = false;
        this->buttons[i*2+1]->inputManager.unselectable = false;
    }
}

void MultiTransmissionSelectPage::setupButton(PushButton* button){
    button->setOnClickHandler(&this->onButtonClickHandler, 0);
    button->setOnDeselectHandler(&this->onButtonDeselectHandler);
}

void MultiTransmissionSelectPage::SetCPUVehicleType(){
    RaceConfig::Scenario* scenario = &RaceConfig::instance->menuScenario;
    if (scenario->settings.isOnline()) return;
    Random random;
    u8 localPlayerCount = SectionManager::instance->globalContext->humanPlayerCount;
    for (int i = localPlayerCount; i < 12; i++){
        scenario->players[i].transmission = random.nextU32(2) + 1;
    }
}

void MultiTransmissionSelectPage::onButtonClick(PushButton* button, u32 hudSlotId){
    this->activePlayers += (1 << hudSlotId);
    RaceConfig::Player* player = &RaceConfig::instance->menuScenario.players[hudSlotId];
    switch (button->buttonId)
    {
        case 0:
        case 2:
        case 4:
        case 6:
            player->transmission = RaceConfig::Player::TRANSMISSION_OUTSIDE;
            this->buttons[button->buttonId+1]->hidden = true;
            this->buttons[button->buttonId+1]->inputManager.unselectable = true;
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            player->transmission = RaceConfig::Player::TRANSMISSION_INSIDE;
            this->buttons[button->buttonId-1]->hidden = true;
            this->buttons[button->buttonId-1]->inputManager.unselectable = true;
            break;
        default:
            break;
    }
    this->multiControlInputManager.players[hudSlotId].enabled = false;
    bool allPlayersSelected = checkAllPlayersActive();
    if (allPlayersSelected){
        this->loadNextPageById(Page::DRIFT_SELECT_MULTI_PLAYER, button);
        SetCPUVehicleType();
    }
}

void MultiTransmissionSelectPage::onBackPress(u32 hudSlotId){
    if (!this->multiControlInputManager.players[hudSlotId].enabled){
        this->multiControlInputManager.players[hudSlotId].enabled = true;
        this->buttons[hudSlotId*2]->hidden = false;
        this->buttons[hudSlotId*2]->inputManager.unselectable = false;
        this->buttons[hudSlotId*2+1]->hidden = false;
        this->buttons[hudSlotId*2+1]->inputManager.unselectable = false;
        this->playSound(6, 0); // Back Press sound
        this->activePlayers -= (1 << hudSlotId);
    }
    else{
        for (int i = 0; i < SectionManager::instance->globalContext->humanPlayerCount; i++)
        {
            this->multiControlInputManager.players[i].enabled = true;
        }
        this->loadPrevPageWithDelay(0.0f);
    }
}

void FixMultiVehicleModelTransition(VehicleModelControl* ctrl, Page::PageID id){
    if(id == Page::TRANSMISSION_SELECT_MULTI_PLAYER){
        id = Page::DRIFT_SELECT_MULTI_PLAYER;
    }
    ctrl->setAnimationType(id);
};
kmCall(0x8084a770, FixMultiVehicleModelTransition);
kmCall(0x8084a6dc, FixMultiVehicleModelTransition);
