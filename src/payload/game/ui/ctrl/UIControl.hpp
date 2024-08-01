#pragma once
#include <game/ui/ControlGroup.hpp>
#include <game/ui/TypeInfo.hpp>
#include <revolution/mtx.h>

enum ElementPosition {
    POS_PROGRAMMER_REL, // Set by the programmer
    POS_LAYOUT,         // Set by the layout artist in a .brctr file
    POS_INHERITED,      // Propagated down from the root
    POS_FINAL,          // Final position displayed on screen
    POS_COUNT,
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
        EVENT_PAGE_CLOSE,
    };

    virtual ~UIControl();

    virtual void init();
    virtual void calc();
    virtual void draw(int drawPass);

    virtual void initSelf();
    virtual void calcSelf();

    virtual void solveAnim(Element* element, float frame);
    virtual void onPageEventSelf(PageEventTrigger event, u32 pageId);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    virtual void vf_30();
    virtual void vf_34();

    void initChildren(u32 count);
    void insertChild(u32 index, UIControl* child);
    void playSound(int soundId, int param_3);

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
