#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class CtrlMenuInstructionText : public LayoutUIControl {
public:
    virtual ~CtrlMenuInstructionText();

    virtual void solveAnim(Element* element, float frame);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load();
    void setText(u32 bmgId, MessageInfo* info = nullptr);
};
size_assert(CtrlMenuInstructionText, sizeof(LayoutUIControl));
