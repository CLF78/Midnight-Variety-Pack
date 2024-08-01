#pragma once
#include "GlobalContext.hpp"
#include "MessageGroup.hpp"
#include "Section.hpp"
#include "SectionPad.hpp"
#include <game/system/SaveGhostManager.hpp>
#include <nw4r/ut/Color.hpp>

class SectionManager {
public:
    enum State {
        IDLE,
        REINIT_REQUESTED,
        CHANGE_REQUESTED,
        REINIT_READY,
        CHANGE_READY,
        OVERRIDE_READY,
    };

    void createSection();

    Section* curSection;

    Section::SectionId bootSectionId;
    Section::SectionId forcedBootSectionId; // overrides bootSectionId if set
    Section::SectionId nextSectionId;
    Section::SectionId prevSectionId;

    Page::AnimationDirection currentAnimDirection;
    Page::AnimationDirection nextAnimDirection;
    u32 animDelay;
    bool firstLoad;
    // 3 bytes padding

    u32 statusBitfield;
    nw4r::ut::Color fadeOutColor;
    int someFrameCounter;
    State state;
    SectionPad pad;

    SaveGhostManager* saveGhostManager;
    MessageGroup* systemMsgGroup;
    GlobalContext* globalContext;

    static SectionManager* instance;
};
size_assert(SectionManager, 0x9C);
