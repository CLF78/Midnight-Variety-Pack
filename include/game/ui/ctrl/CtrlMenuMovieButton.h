#include <kamek.h>
#include <game/ui/PushButton.h>
#include <game/ui/MenuMovie.h>

class CtrlMenuMovieButton : public MenuMovie, PushButton {
};
size_assert(CtrlMenuMovieButton, 0x26C);
