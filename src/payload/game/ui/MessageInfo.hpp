#pragma once
#include <common/Common.hpp>
#include <game/system/Mii.hpp>

struct MessageEscapeSequence {
    u16 magic;
    u8 length;
    u8 fontType;
    u16 sequenceType;
    u16 settings[3];
};

class MessageInfo {
public:
    MessageInfo();

    void setCondMessageValue(int idx, bool val, bool prependSpaceCharacter) {
        intVals[idx] = val + 1;
        prependSpace = prependSpaceCharacter;
    }

    void reset() {
        for (int i = 0; i < ARRAY_SIZE(intVals); i++) {
            intVals[i] = 0;
            messageIds[i] = 0;
            miis[i] = nullptr;
            licenseIds[i] = 0;
            playerIds[i] = -1;
            strings[i] = nullptr;
        }

        useColoredBorder = false;
        prependSpace = false;
    }

    int intVals[9];
    int messageIds[9];
    Mii* miis[9];
    u8 licenseIds[9];
    // 3 bytes padding

    int playerIds[9];
    wchar_t* strings[9];
    bool useColoredBorder;

    bool prependSpace; // custom field
    // 2 bytes padding
};
size_assert(MessageInfo, 0xC4);
