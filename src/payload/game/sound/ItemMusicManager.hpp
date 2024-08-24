#pragma once
#include <egg/core/eggDisposer.hpp>
#include <nw4r/snd/SoundHandle.hpp>

class ItemMusicManager : public EGG::Disposer {
public:
    enum State {
        KINOKO_BIG,
        STAR,
        DAMAGE,
        IDLE,
    };

    u32 state;
    u8 _14[3];
    bool speedUp; // custom field

    nw4r::snd::SoundHandle* _18;
    nw4r::snd::SoundHandle* _1C;

    f32 volume;
    f32 pitch;
    f32 _28;

    static ItemMusicManager* instance;
};
size_assert(ItemMusicManager, 0x2C);
