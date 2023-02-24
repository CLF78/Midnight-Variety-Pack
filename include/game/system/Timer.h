#include <kamek.h>

class Timer {
public:
    virtual ~Timer();

    u16 minutes;
    u8 seconds;
    u16 milliseconds;
    bool active;
};
size_assert(Timer, 0xC);
