
#include <nw4r/math/types.hpp>
#include <game/kart/KartParam.hpp>

class KartSettings {
public:
    KartSettings(u32 playerIdx, int kart, int character, bool isBike, const KartParam& kartParam, void* unknown,
        void* kartDriverDispEntry, void* kartPartsDispEntry,
        void* bikePartsDispEntry, void* driverDispEntry);

    u32 isBike;
    u32 kart;
    u32 character;
    u16 wheelCount0;
    u16 wheelCount1;
    u8 playerIdx;
    u8 _11[2];
    const KartParam& kartParam;
    int* _18;
    void* kartDriverDispEntry;
    void* kartPartsDispEntry;
    void* bikePartsDispEntry;
    void* driverDispEntry;
    float wheelCountReciprocal;
    float wheelCountPlus1Reciprocal;
    void* gpStats;
    u8 _38[0x3C - 0x38];
};
size_assert(KartSettings, 0x3C);
