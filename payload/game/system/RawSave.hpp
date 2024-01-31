#include <common/Common.hpp>

struct RawSave {

    struct License {
        u8 data[0x8CC0];
    };
    size_assert(License, 0x8CC0);

    struct GlobalData {

        enum Settings {
            CONTROLLER_RUMBLE = 0x1,
            FLAG_REGION_DISPLAY = 0x2,
            UNK_4 = 0x4,
            MESSAGE_SERVICE = 0x8,
            DATA_SHARING = 0x10,
            MII_DISPLAY = 0x20,
            DEFAULT_VALUE = MII_DISPLAY | UNK_4 | FLAG_REGION_DISPLAY | CONTROLLER_RUMBLE,

            // custom fields onwards
            WIIMMFI_DATA_SHARING = 0x80,
        };

        u32 magic;
        u32 settings;
        u8 unk[0x37FA];
        u16 regionId;
    };
    size_assert(GlobalData, 0x3804);

    struct UnkBlock {
        u8 data[0x14F0];
    };
    size_assert(UnkBlock, 0x14F0);

    u32 magic;
    u32 version;
    License licenses[4];
    GlobalData globalData;
    UnkBlock unkBlock;
    u32 crc32;
};
size_assert(RawSave, 0x28000);
