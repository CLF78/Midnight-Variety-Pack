#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/ctrl/BlackBackControl.hpp>
#include <game/ui/page/YesNoPage.hpp>

class YesNoPopupPage : public YesNoPage {
public:
    YesNoPopupPage();
    virtual ~YesNoPopupPage();

    virtual void onInit();
    virtual void beforeInAnim();
    virtual void afterOutAnim();

    virtual TypeInfo* getTypeInfo() const;

    virtual void reset();

    BlackBackControl blackBackControl;
    LayoutUIControl messagePopup;
};
size_assert(YesNoPopupPage, 0xBA0);
