#pragma once
#include <nw4r/lyt/pane.hpp>

class Layout {
public:
    nw4r::lyt::Pane* getPaneByName(const char* name);

    UNK(0x28);
};
size_assert(Layout, 0x28);

class MainLayout : public Layout {
public:
    UNK(0x9C - 0x28);
};
size_assert(MainLayout, 0x9C);

class PictureSourceLayout : public Layout {
public:
    UNK(0xB0 - 0x28);
};
size_assert(PictureSourceLayout, 0xB0);
