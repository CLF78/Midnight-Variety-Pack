#include <common/Common.hpp>
#include <game/ui/ctrl/CtrlMenuMovieButton.hpp>
#include <game/ui/page/ClassSelectPage.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/system/RaceConfig.hpp>

///////////////////////////
// Custom Engine Classes //
///////////////////////////

static const char* btnVariants[] = {
    "Button50cc",
    "Button100cc",
    "Button150cc",
};

static const char* btnMats[] = {
    "kinoko_1",
    "kinoko_2",
    "kinoko_3",
};

// ClassSelectPage::ClassSelectPage() patch
// Fix the back button when exiting TT mode after changing course/character
kmBranchDefCpp(0x80626C0C, NULL, ClassSelectPage*, ClassSelectPage* self) {
    self->prevPageId = Page::SINGLE_PLAYER_MENU;
    self->buttonCount = 3;
    return self;
}

// ClassSelectPage::onStartPress() override
// Disable the "change vehicles" behaviour
kmWrite32(0x8083FBC8, 0x4E800020);

// ClassSelectPage::onInit() override
// Remove the mirror button and select the correct button when exiting TT mode
kmPointerDefCpp(0x808D93C0, void, ClassSelectPage* self) {

    // Default behaviour
    self->hasBackButton = true;
    self->callOnInit();
    SectionManager::instance->curSection->loadTHPManager();
    self->switchModeOff = true;
    self->multiControlInputManager.setDistanceFunc(MultiControlInputManager::Y_WRAP);

    // If we are not returning from TT mode, we don't need to set the button
    u32 section = SectionManager::instance->curSection->sectionID;
    if (section != Section::MENUSINGLE_FROM_TT_CHANGE_CHAR && section != Section::MENUSINGLE_FROM_TT_CHANGE_COURSE)
        return;

    // Get each button id
    for (int i = 0; i < self->buttonCount; i++) {
        PushButton* btn = self->buttons[i];
        int buttonId = btn->buttonId;

        // Skip all irrelevant buttons
        if (buttonId < 0 || buttonId > 2)
            continue;

        // If the engine class matches the current button's, set the button and return
        u32 engineClass = ClassSelectPage::engineClasses[buttonId];
        if (RaceConfig::instance->menuScenario.settings.engineClass == engineClass) {
            btn->selectDefault(0);
            return;
        }
    }
}

// ClassSelectPage::onActivate() override
// Hide the switch vehicles option by default
kmPointerDefCpp(0x808D93C8, void, ClassSelectPage* self) {

    // Call base function
    self->callOnActivate();

    // Load movie
    const char* movies[] = { "thp/button/class_top.thp" };
    self->loadMovies(movies, ARRAY_SIZE(movies));

    // Toggle mirror flag
    RaceConfig::instance->menuScenario.settings.modeFlags &= ~RaceConfig::Settings::FLAG_MIRROR;
    self->switchButton.hidden = true;
}

// ClassSelectPage::loadButton() override
// Skip creating the mirror mode button, force kart+bike movies and update the text messages
kmPointerDefCpp(0x808D941C, PushButton*, ClassSelectPage* self, int buttonIdx) {

    // Create the button and insert it
    CtrlMenuMovieButton* movieBtn = new CtrlMenuMovieButton();
    self->insertChild(self->curChildCount++, movieBtn, 0);

    // Load the layout
    movieBtn->load("button", "ClassSelect", btnVariants[buttonIdx], self->activePlayers, false, false);

    // Set movie, icon and text message
    u32 cropTop = (buttonIdx > 1) ? buttonIdx : buttonIdx + 4;
    const float totalBtns = 1.0f / 6.0f;
    movieBtn->setMovieCrop("black_base", cropTop * totalBtns, (cropTop + 1) * totalBtns, 0.0f, 1.0f);
    movieBtn->setMatIcon("kinoko_set_p", btnMats[buttonIdx]);
    movieBtn->setText(20000 + (buttonIdx & 3), nullptr);
    return movieBtn;
}

// Replace CCs used for each button
kmWrite32(0x808AD168, RaceConfig::Settings::CC_150);
kmWrite32(0x808AD16C, RaceConfig::Settings::CC_200);
kmWrite32(0x808AD170, RaceConfig::Settings::CC_500);
