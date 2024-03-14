#include <common/Common.hpp>
#include <game/util/NandUtil.hpp>

namespace SaveExpansionManager {

    int Read();
    int Create();
    int Write();
    int Delete();

    extern int sError;
    extern int sCheckError;

} // namespace SaveExpansionManager
