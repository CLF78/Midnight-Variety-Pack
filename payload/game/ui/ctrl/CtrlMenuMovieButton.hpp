#include <common/Common.hpp>
#include <game/ui/PushButton.hpp>
#include <game/ui/MenuMovie.hpp>

class CtrlMenuMovieButton : public MenuMovie, public PushButton {
public:
    CtrlMenuMovieButton() {}
    virtual ~CtrlMenuMovieButton();
    virtual int getMovieCount() const;
    virtual char** getPaneNames() const;

    virtual const TypeInfo* getTypeInfo() const;
    virtual const char* getTypeName() const;
};
size_assert(CtrlMenuMovieButton, 0x26C);
