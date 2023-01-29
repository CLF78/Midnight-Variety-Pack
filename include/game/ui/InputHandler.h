#include <kamek.h>

template<class T>
class InputHandler {
public:
    virtual void execute(u32 localPlayerId);

    T* owner;
    void (T::*inputFunc)();
};
