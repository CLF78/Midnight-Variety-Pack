#pragma once
#include "page/Page.hpp"
#include <nw4r/lyt/pane.hpp>
#include <nw4r/math/types.hpp>

class MenuMovie {
public:
    MenuMovie() : hidden(false), delay(0) {}
    virtual ~MenuMovie();

    virtual void toggle(bool hide);
    virtual int getMovieCount() const = 0;
    virtual char** getPaneNames() const = 0;

    nw4r::lyt::Pane** panes;
    Page* parent;
    bool hidden;
    u32 delay;
    nw4r::math::VEC2* scales;
};
size_assert(MenuMovie, 0x18);
