#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/ctrl/BlackBackControl.hpp>
#include <game/ui/page/YesNoPage.hpp>

class YesNoPopupPage : public YesNoPage {
public:
    virtual ~YesNoPopupPage();

    virtual void onInit();
    virtual void beforeInAnim();
    virtual void afterOutAnim();

    virtual TypeInfo* getTypeInfo() const;

    virtual void reset();

    static YesNoPopupPage* getPage() {
        return (YesNoPopupPage*)SectionManager::instance->curSection->pages[Page::ONLINE_VOTE_PROMPT];
    }

    BlackBackControl blackBackControl;
    LayoutUIControl messagePopup;
};
size_assert(YesNoPopupPage, 0xBA0);
