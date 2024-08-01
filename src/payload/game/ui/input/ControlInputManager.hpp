#pragma once

class ControlInputManager {
public:
    u8 unk[0x75];
    bool unselectable;
    u8 unk2[0xE];
};
size_assert(ControlInputManager, 0x84);
