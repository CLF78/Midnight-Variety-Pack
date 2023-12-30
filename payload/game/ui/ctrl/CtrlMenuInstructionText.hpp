#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlMenuInstructionText : public LayoutUIControl {
public:
    virtual ~CtrlMenuInstructionText();

    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void setText(u32 bmgId, MessageInfo* info);
};
size_assert(CtrlMenuInstructionText, 0x174);
