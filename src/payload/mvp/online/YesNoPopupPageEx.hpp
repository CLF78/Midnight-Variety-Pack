#include <common/Common.hpp>
#include <game/ui/page/YesNoPopupPage.hpp>

class YesNoPopupPageEx : public YesNoPopupPage {
public:
    YesNoPopupPageEx();
    virtual ~YesNoPopupPageEx() {}
    virtual void reset();

    void onBackPress(u32 hudSlotId);
    void forcePressSelected();

    static YesNoPopupPageEx* getPage() {
        return (YesNoPopupPageEx*)SectionManager::instance->curSection->pages[Page::ONLINE_VOTE_PROMPT];
    }

    InputHandler1<YesNoPopupPageEx, void, u32> onBackPressHandler;
    int onBackSelectedButton;
    bool isBackButtonEnabled;
};
