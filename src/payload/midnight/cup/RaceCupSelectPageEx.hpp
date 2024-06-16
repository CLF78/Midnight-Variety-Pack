#include <common/Common.hpp>
#include <game/ui/SectionManager.hpp>
#include <game/ui/page/RaceCupSelectPage.hpp>
#include <game/ui/ctrl/SheetSelectControl.hpp>
#include <midnight/cup/RaceCupSelectArrow.hpp>

class RaceCupSelectPageEx : public RaceCupSelectPage {
public:

    enum LayoutIdx {
        LYT_CUP_SELECT_CUP,
        LYT_CUP_SELECT_COURSE,
        LYT_CUP_ARROWS,
    };

    RaceCupSelectPageEx();
    virtual ~RaceCupSelectPageEx() {}

    virtual void onActivate();
    virtual UIControl* loadLayout(u32 layoutIdx);

    void setCourse(CtrlMenuCupSelectCup* cupHolder, PushButton* button);
    void updateTextMessages(CtrlMenuCupSelectCup* cupHolder, u32 unk);

    static RaceCupSelectPageEx* getPage() {
        return (RaceCupSelectPageEx*)SectionManager::instance->curSection->pages[Page::CUP_SELECT];
    }

    SheetSelectControl arrows;
    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32> leftHandler;
    InputHandler2<RaceCupSelectArrow, void, SheetSelectControl*, u32> rightHandler;
    s32 curPage;
};
