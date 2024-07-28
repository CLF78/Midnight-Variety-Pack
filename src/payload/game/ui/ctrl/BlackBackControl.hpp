#pragma once
#include <common/Common.hpp>
#include <game/ui/ctrl/LayoutUIControl.hpp>

class BlackBackControl : public LayoutUIControl {
public:
    virtual ~BlackBackControl();

    virtual void solveAnim(Element* element, float frame);

    RUNTIME_TYPE_INFO_NODECL;
    RUNTIME_TYPENAME_INFO_NODECL;
};
size_assert(BlackBackControl, sizeof(LayoutUIControl));
