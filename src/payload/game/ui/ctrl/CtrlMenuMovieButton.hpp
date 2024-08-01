#pragma once
#include "PushButton.hpp"
#include <game/ui/MenuMovie.hpp>

class CtrlMenuMovieButton : public MenuMovie, public PushButton {
public:
    CtrlMenuMovieButton() {}
    virtual ~CtrlMenuMovieButton();
    virtual int getMovieCount() const;
    virtual char** getPaneNames() const;

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;
};
size_assert(CtrlMenuMovieButton, 0x26C);
