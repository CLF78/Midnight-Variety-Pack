#include <kamek.h>
#include <game/ui/LayoutUIControl.h>

class CtrlMenuPageTitleText : public LayoutUIControl {
public:
    CtrlMenuPageTitleText() {}
    virtual ~CtrlMenuPageTitleText();

    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
};
size_assert(CtrlMenuPageTitleText, 0x174);
