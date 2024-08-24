#pragma once

class UIControlTimer {
public:
    float value;
    bool active;
    PAD(3);
    int seconds;
};
size_assert(UIControlTimer, 0xC);
