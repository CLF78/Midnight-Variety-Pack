#pragma once

class Timer {
public:
    virtual ~Timer();

    u32 getTimeMs() const { return milliseconds + seconds * 1000 + minutes * 60 * 1000; }

    u16 minutes;
    u8 seconds;
    PAD(1);

    u16 milliseconds;
    bool active;
    PAD(1);
};
size_assert(Timer, 0xC);
