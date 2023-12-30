#include <common/Common.hpp>
#include <midnight/cup/CupManager.hpp>
#include <midnight/cup/CupData.hpp>

class SaveExpansion {
public:

    struct Cup {
        Cup() : completed(false), rank(0xFF) {}

        bool completed;
        u8 rank;
    };

    SaveExpansion() {
        gpRanks = new Cup[CupManager::GetCupListData(CupManager::TRACKS_MODERN)->cupCount];
    }

    // GP Data
    Cup* gpRanks;

    // TODO finish adding whatever
};
