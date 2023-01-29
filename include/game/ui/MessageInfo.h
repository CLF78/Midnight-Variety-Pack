#include <kamek.h>

class Mii;

class MessageInfo {
public:
    int intVals[9];
    int messageIds[9];
    Mii* miis[9];
    u8 licenseIds[9];
    int playerIds[9];
    wchar_t* strings[9];
    bool useColoredBorder;
};
size_assert(MessageInfo, 0xC4);
