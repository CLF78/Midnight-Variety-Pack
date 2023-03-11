#include <kamek.h>
#include <rvl/os/OSContext.h>

namespace nw4r {
namespace db {

void Exception_Printf_(char* format, ...);
void ShowMainInfo_(u16 error, OSContext* context, u32 dsisr, u32 dar);

} // namespace db
} // namespace nw4r
