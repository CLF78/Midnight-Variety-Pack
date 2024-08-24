#pragma once
#include <nw4r/db/console.hpp>

namespace EGG {

class Exception {
public:
    virtual ~Exception();

    static Exception* sException;
    static nw4r::db::ConsoleHead* sConsoleHandle;
};
size_assert(Exception, 0x4);

bool ExceptionCallback(nw4r::db::ConsoleHead* console, void* arg);

} // namespace EGG
