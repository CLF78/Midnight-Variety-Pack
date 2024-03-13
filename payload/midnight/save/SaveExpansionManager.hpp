#include <common/Common.hpp>
#include <game/util/NandUtil.hpp>

class SaveExpansionManager {
public:
    SaveExpansionManager() : mError(NandUtil::ERROR_NONE) {}

    void Read();
    void Create();
    void Write();

    int mError;

    static SaveExpansionManager sInstance;
};
