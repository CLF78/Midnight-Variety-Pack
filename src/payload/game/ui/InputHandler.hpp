#include <common/Common.hpp>

#define SET_HANDLER(destHandler, srcHandler) \
    destHandler = (typeof(destHandler))&srcHandler

#define SET_HANDLER_FUNC(handler, func) \
    handler.handle = (typeof(handler.handle))&func

class InputHandlerBase {};

template <class T, typename R>
class InputHandler0 : public InputHandlerBase {
public:
    InputHandler0(T* pOwner, R (T::*pInputFunc)()) : owner(pOwner), handle(pInputFunc) {}
    virtual R operator()() { return (owner->*handle)(); }

    T* owner;
    R (T::*handle)();
};

template <class T, typename R, typename A1>
class InputHandler1 : public InputHandlerBase {
public:
    InputHandler1(T* pOwner, R (T::*pInputFunc)(A1)) : owner(pOwner), handle(pInputFunc) {}
    virtual R operator()(A1 arg1) { return (owner->*handle)(arg1); }

    T* owner;
    R (T::*handle)(A1);
};

template <class T, typename R, typename A1, typename A2>
class InputHandler2 : public InputHandlerBase {
public:
    InputHandler2(T* pOwner, R (T::*pInputFunc)(A1, A2)) : owner(pOwner), handle(pInputFunc) {}
    virtual R operator()(A1 arg1, A2 arg2) { return (owner->*handle)(arg1, arg2); }

    T* owner;
    R (T::*handle)(A1, A2);
};

template <class T, typename R, typename A1, typename A2, typename A3>
class InputHandler3 : public InputHandlerBase {
public:
    InputHandler3(T* pOwner, R (T::*pInputFunc)(A1, A2, A3)) : owner(pOwner), handle(pInputFunc) {}
    virtual R operator()(A1 arg1, A2 arg2, A3 arg3) { return (owner->*handle)(arg1, arg2, arg3); }

    T* owner;
    R (T::*handle)(A1, A2, A3);
};
