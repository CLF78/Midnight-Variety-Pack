#include <common/Common.hpp>

class RKNetUSERHandler {
public:
    void SetSendUSERPacket(u8 aid);

    static RKNetUSERHandler* instance;
};
