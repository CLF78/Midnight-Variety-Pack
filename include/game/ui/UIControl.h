#include <kamek.h>
#include <rvl/mtx/mtx.h>
#include <game/ui/ControlGroup.h>
#include <game/ui/TypeInfo.h>

enum ElementPosition {
    POS_PROGRAMMER_REL, // Set by the programmer
    POS_LAYOUT,         // Set by the layout artist in a .brctr file
    POS_INHERITED,      // Propagated down from the root
    POS_FINAL,          // Final position displayed on screen
    POS_COUNT
};

struct Element {
    VEC3 trans;
    VEC2 scale;
    u8 opacity;
};

class UIControl {
public:
    enum PageEventTrigger {
        EVENT_PAGE_OPEN,
        EVENT_PAGE_CLOSE
    };

    virtual ~UIControl();

    virtual void init();
    virtual void calc();
    virtual void draw(int draw_pass);

    virtual void initSelf();
    virtual void calcSelf();

    virtual void solveAnim(Element* element, float frame);
    virtual void onPageEventSelf(PageEventTrigger event, u32 page_id);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    virtual void vf30();
    virtual void vf34();

    Element elementPositions[POS_COUNT];

    ControlGroup* group; // the true "parent" is group->parent
    ControlGroup children;

    f32 zIndex;
    bool hidden;
    u32 drawPass;

    bool animated;
    f32 animStartFrame;

    s32 onShowSfxId;
    s32 onHideSfxId;
};
size_assert(UIControl, 0x98);
