#include <common/Common.hpp>

// Forward declaration
class ItemHolderPlayer;

struct ItemProperties {

    enum ItemId {
        ITEM_GREEN_SHELL,
        ITEM_RED_SHELL,
        ITEM_BANANA,
        ITEM_FAKE_BOX,
        ITEM_MUSHROOM,
        ITEM_TRIPLE_MUSHROOM,
        ITEM_BOMB,
        ITEM_BLUE_SHELL,
        ITEM_SHOCK,
        ITEM_STAR,
        ITEM_GOLDEN_MUSHROOM,
        ITEM_MEGA_MUSHROOM,
        ITEM_BLOOPER,
        ITEM_POW_BLOCK,
        ITEM_THUNDERCLOUD,
        ITEM_BULLET_BILL,
        ITEM_TRIPLE_GREEN_SHELL,
        ITEM_TRIPLE_RED_SHELL,
        ITEM_TRIPLE_BANANA,
        ITEM_INVALID,
        ITEM_NONE,
    };

    enum ItemObjId {
        ITEMOBJ_GREEN_SHELL,
        ITEMOBJ_RED_SHELL,
        ITEMOBJ_BANANA,
        ITEMOBJ_MUSHROOM,
        ITEMOBJ_STAR,
        ITEMOBJ_BLUE_SHELL,
        ITEMOBJ_SHOCK,
        ITEMOBJ_FAKE_BOX,
        ITEMOBJ_MEGA_MUSHROOM,
        ITEMOBJ_BOMB,
        ITEMOBJ_BLOOPER,
        ITEMOBJ_POW_BLOCK,
        ITEMOBJ_GOLDEN_MUSHROOM,
        ITEMOBJ_BULLET_BILL,
        ITEMOBJ_THUNDERCLOUD,
        ITEMOBJ_INVALID,
        ITEMOBJ_NONE,
    };

    enum ItemUseType {
        ITEMUSE_USE,
        ITEMUSE_FIRE,
        ITEMUSE_TAIL,
        ITEMUSE_CIRCLE,
    };

    typedef void (*ItemUseFunc) (ItemHolderPlayer* player);

    bool enabled;
    bool enabledOnline;
    // 2 bytes padding

    int objectId;
    u32 limit;
    u32 severity; // unused
    bool isTimedItem; // for Golden Mushroom
    // 3 bytes padding

    int useType;
    ItemUseFunc useFunction; // item is draggable if this is null

    static ItemProperties itemPropertiesTable[19];
};
size_assert(ItemProperties, 0x1C);
