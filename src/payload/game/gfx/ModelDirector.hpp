#include <common/Common.hpp>
#include <game/gfx/ModelTransformator.hpp>

class ModelDirector {
public:
    u8 unk[0x28];
    ModelTransformator* modelTransformator;
    u8 unk2[0x4C-0x2C];
};
size_assert(ModelDirector, 0x4C);
