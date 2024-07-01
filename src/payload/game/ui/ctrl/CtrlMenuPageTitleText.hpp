#include <common/Common.hpp>
#include <game/ui/LayoutUIControl.hpp>

class CtrlMenuPageTitleText : public LayoutUIControl {
public:
    virtual ~CtrlMenuPageTitleText();

    virtual void solveAnim(Element* element, float frame);

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void load(bool isOptions);
    void setText(int bmgId, MessageInfo* msgInfo);
};
size_assert(CtrlMenuPageTitleText, sizeof(LayoutUIControl));
