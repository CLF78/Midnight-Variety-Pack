#include <common/Common.hpp>
#include <game/ui/ctrl/UpDownControl.hpp>

class TextControl : public LayoutUIControl {
public:
    TextControl();
    virtual ~TextControl();
    virtual void calcSelf();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
};
size_assert(TextControl, 0x174);

class TextUpDownValueControl : public LayoutUIControl {
public:
    TextUpDownValueControl();
    virtual ~TextUpDownValueControl();
    virtual void init();
    virtual void calc();

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;

    void load(const char* folderName, const char* textCtr, const char* textLoc,
                    const char* valueCtr, const char* valueLoc);

    UpDownAnimator animator;

    TextControl texts[2];
    TextControl* shownText;
    TextControl* hiddenText;

    nw4r::lyt::Pane* fuchiPattern;
    nw4r::lyt::Pane* colorBase;

    static TextUpDownValueControl* construct(void* buffer) {
        return new(buffer) TextUpDownValueControl;
    }
};
size_assert(TextUpDownValueControl, 0x474);
