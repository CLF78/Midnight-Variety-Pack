#include <kamek.h>

class Layout {
public:
    u8 _00[0x28];
};
size_assert(Layout, 0x28);

class MainLayout : public Layout {
public:
    u8 _28[0x9c - 0x28];
};
size_assert(MainLayout, 0x9C);
