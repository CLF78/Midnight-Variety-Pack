#include <common/Common.hpp>
#include <nw4r/db/console.hpp>

namespace EGG {

class Exception {
public:
    virtual ~Exception();

    static Exception* sException;
    static nw4r::db::ConsoleHead* sConsoleHandle;
};

} // namespace EGG
