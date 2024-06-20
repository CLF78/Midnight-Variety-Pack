#include <common/Common.hpp>
#include <game/ui/page/YesNoPopupPage.hpp>

class YesNoPopupPageEx : public YesNoPopupPage {
public:
    YesNoPopupPageEx();
    virtual ~YesNoPopupPageEx() {}
    virtual void reset();

    void handleBackPress();

    static YesNoPopupPageEx* getPage() {
        return (YesNoPopupPageEx*)SectionManager::instance->curSection->pages[Page::ONLINE_VOTE_PROMPT];
    }

    InputHandler0<YesNoPopupPageEx, void> onBackPress;
    int onBackSelectedButton;
    bool isBackButtonEnabled;
};
