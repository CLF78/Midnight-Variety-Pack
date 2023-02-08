#include <kamek.h>
#include <game/ui/Page.h>

class MenuMovie {
public:
    virtual ~MenuMovie();

    nw4r::lyt::Pane** panes;
    Page* parent;
    u8 _C;
    u32 _10;
    void* _14;
};
size_assert(MenuMovie, 0x18);
