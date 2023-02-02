#include <kamek.h>
#include <stdlib/new.h>

// This is technically incorrect but i couldn't care less
template<class T>
class InputHandler {
public:
    InputHandler(T* pOwner, void (T::*pInputFunc)(void*, u32)) { owner = pOwner; inputFunc = pInputFunc; }
    virtual void execute(void* object, u32 localPlayerId) { (owner->*inputFunc)(object, localPlayerId); }

    T* owner;
    void (T::*inputFunc)(void*, u32);

    static InputHandler* construct(T* pOwner, void (T::*pInputFunc)(void*, u32), void* buffer) {
        return new(buffer) InputHandler(pOwner, pInputFunc);
    }
};

template<class T, class U>
class InputHandlerEx {
public:
    InputHandlerEx(T* pOwner, void (T::*pInputFunc)(U*, u32)) { owner = pOwner; inputFunc = pInputFunc; }
    virtual void execute(U* object, u32 localPlayerId) { (owner->*inputFunc)(object, localPlayerId); }

    T* owner;
    void (T::*inputFunc)(U*, u32);

    static InputHandlerEx* construct(T* pOwner, void (T::*pInputFunc)(U*, u32), void* buffer) {
        return new(buffer) InputHandlerEx(pOwner, pInputFunc);
    }
};
