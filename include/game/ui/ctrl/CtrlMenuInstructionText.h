#include <kamek.h>
#include <game/ui/LayoutUIControl.h>

class CtrlMenuInstructionText : public LayoutUIControl {
public:
    virtual ~CtrlMenuInstructionText();

    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
};
size_assert(CtrlMenuInstructionText, 0x174);
