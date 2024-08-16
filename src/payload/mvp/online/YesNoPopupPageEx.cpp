#include "YesNoPopupPageEx.hpp"

YesNoPopupPageEx::YesNoPopupPageEx() : onBackPressHandler(this, &YesNoPopupPageEx::onBackPress),
                                       onBackSelectedButton(-1),
                                       isBackButtonEnabled(false) {
    inputManager.setHandler(MenuInputManager::BACK_PRESS, &onBackPressHandler, false, false);
}

void YesNoPopupPageEx::reset() {
    YesNoPopupPage::reset();

    isBackButtonEnabled = false;
    onBackSelectedButton = -1;
}

void YesNoPopupPageEx::forcePressSelected() {
    for (u32 i = 0; i < ARRAY_SIZE(buttons); i++) {
        if (buttons[i].isSelected()) {
            buttons[i].click(0);
            break;
        }
    }
}

void YesNoPopupPageEx::onBackPress(u32 hudSlotId) {
    if (isBackButtonEnabled && onBackSelectedButton != -1 && (playerFlags & (1 << hudSlotId)) != 0) {
        buttons[onBackSelectedButton].selectDefault(hudSlotId);
        buttons[onBackSelectedButton].click(hudSlotId);
    }
}
