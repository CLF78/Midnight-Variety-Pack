#include "KartObject.hpp"

class KartActor {
public:
    virtual ~KartActor();

    UNK(0xB4 - 0x4);
    bool isChargingSSMT;
    PAD(3);
    void* engineMgr;
    UNK(0xDB - 0xBC);
    u8 currentLap;
    KartObject* kartObject;
    bool isLocal;
    bool isGhost;
    u8 wheelCount;
    u8 _E3;
    u32 team;
    bool isColliding;
    u8 driftState;
    u8 _EA[2];
    float speedNorm;
    u16 kclFlag;
    u16 variant;
    u32 outputLineFlag;
    u32 roadState;
};
size_assert(KartActor, 0xFC);
