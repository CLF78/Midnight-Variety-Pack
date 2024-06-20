#include <common/Common.hpp>
#include <midnight/online/YesNoPopupPageEx.hpp>

YesNoPopupPageEx::YesNoPopupPageEx() : onBackPress(this, &handleBackPress),
                                       onBackSelectedButton(-1),
                                       isBackButtonEnabled(false) {
    inputManager.setHandler(MenuInputManager::BACK_PRESS, &onBackPress, false, false);
}

void YesNoPopupPageEx::reset() {
    YesNoPopupPage::reset();

    isBackButtonEnabled = false;
    onBackSelectedButton = -1;
}

void YesNoPopupPageEx::forcePressSelected() {
    for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
        if (buttons[i].isSelected()) {
            buttons[i].click(0);
            break;
        }
    }
}

void YesNoPopupPageEx::handleBackPress() {
    if (isBackButtonEnabled && onBackSelectedButton != -1) {
        for (int i = 0; i < 5; i++) {
            if (playerFlags & (1 << i) != 0) {
                buttons[onBackSelectedButton].selectDefault(i);
                buttons[onBackSelectedButton].click(i);
                break;
            }
        }
    }
}
