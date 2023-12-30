#include <common/Common.hpp>
#include <nw4r/ut/utList.hpp>
#include <game/kart/KartPointers.hpp>

class KartObjectProxy {
public:
    KartPointers* pointers;
    nw4r::ut::Link linkList;

    u8 getPlayerIdx();
    VehiclePhysics* getVehiclePhysics();
    void FUN_80591050(int, int, int);
};
