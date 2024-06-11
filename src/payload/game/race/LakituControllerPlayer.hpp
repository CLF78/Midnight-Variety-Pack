#include <common/Common.hpp>
#include <game/gfx/ModelDirector.hpp>
#include <game/kart/KartObjectProxy.hpp>

class LakituControllerPlayer {
public:
    virtual ~LakituControllerPlayer();

    ModelDirector* lakituModel;
    ModelDirector* rodModel;
    ModelDirector* signalModel;
    ModelDirector* lapModel;
    ModelDirector* finalLapModel;
    ModelDirector* reverseModel;
    ModelDirector* flagModel;
    ModelDirector* hairModel;
    ModelDirector* lampModel;

    u8 unk[0x1CC-0x28];

    KartObjectProxy* mpProxy;
    u8 unk2[0x218-0x1D0];
};
size_assert(LakituControllerPlayer, 0x218);
