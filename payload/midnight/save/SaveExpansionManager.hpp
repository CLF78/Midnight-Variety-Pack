#include <common/Common.hpp>
#include <game/util/NandUtil.hpp>

class SaveExpansionManager {
public:
    SaveExpansionManager() : mError(NandUtil::ERROR_NONE) {}
    void Init();

    int mError;

    static SaveExpansionManager sInstance;
};
