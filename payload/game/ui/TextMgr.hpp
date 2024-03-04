#include <common/Common.hpp>

class TextMgr {
public:
    void putChar(wchar_t c);

    static TextMgr* instance;
};
