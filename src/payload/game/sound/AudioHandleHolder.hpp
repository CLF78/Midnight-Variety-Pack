#pragma once
#include <game/kart/KartObject.hpp>

class AudioHandleHolder {
public:
    void prepare(u32 soundId, bool unk);
    void prepareSpecial(u32 soundId, bool unk);

    static AudioHandleHolder* instance;
};
