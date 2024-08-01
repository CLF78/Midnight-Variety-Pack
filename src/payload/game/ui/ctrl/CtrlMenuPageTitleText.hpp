#pragma once
#include "LayoutUIControl.hpp"

class CtrlMenuPageTitleText : public LayoutUIControl {
public:
    virtual ~CtrlMenuPageTitleText();

    virtual void solveAnim(Element* element, float frame);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;

    void load(bool isOptions);
    void setText(int bmgId, MessageInfo* msgInfo = nullptr);
};
size_assert(CtrlMenuPageTitleText, sizeof(LayoutUIControl));
