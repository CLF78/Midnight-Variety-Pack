#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>
#include <game/ui/MenuMovie.hpp>

class CtrlMenuMovieLayout : public MenuMovie, public LayoutUIControl {
    virtual ~CtrlMenuMovieLayout();
    virtual int getMovieCount() const;
    virtual char** getPaneNames() const;
};
size_assert(CtrlMenuMovieLayout, 0x18C);
