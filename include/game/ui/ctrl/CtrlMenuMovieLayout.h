#include <kamek.h>
#include <game/ui/LayoutUIControl.h>
#include <game/ui/MenuMovie.h>

class CtrlMenuMovieLayout : public MenuMovie, public LayoutUIControl {
};
size_assert(CtrlMenuMovieLayout, 0x18C);
