#pragma once

class ControlInputManager {
public:
    UNK(0x75);
    bool unselectable;
    UNK(0x84 - 0x76);
};
size_assert(ControlInputManager, 0x84);
