// Allow Kamek hooks to be defined from C++ source files
#pragma section ".kamek"

// Hook type IDs must match the Kamek source!
enum KamekCommandType {
    kctNone,
    kctWrite,
    kctWriteArea, // replaces kctConditionalWrite
    kctInjectBranch,
    kctInjectCall,
    kctPatchExit,
};

// Base macros
#define kmHookFn \
    extern "C" static
#define kmIdentifierImpl(key, counter) \
    _k##key##counter
#define kmIdentifier(key, counter) \
    kmIdentifierImpl(key, counter)

// Remove non-existent Intellisense errors
#ifndef __INTELLISENSE__
    #define kmSection __declspec (section ".kamek")
#else
    #define kmSection
#endif

// General hook definition macros
#define kmHookInt(counter) \
    kmSection static const u32 kmIdentifierImpl(Hook, counter)
#define kmHook0(type) \
    kmHookInt(__COUNTER__)[2] = { 0, (type) }
#define kmHook1(type, arg0) \
    kmHookInt(__COUNTER__)[3] = { 1, (type), (u32)(arg0) }
#define kmHook2(type, arg0, arg1) \
    kmHookInt(__COUNTER__)[4] = { 2, (type), (u32)(arg0), (u32)(arg1) }
#define kmHook3(type, arg0, arg1, arg2) \
    kmHookInt(__COUNTER__)[5] = { 3, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2) }
#define kmHook4(type, arg0, arg1, arg2, arg3) \
    kmHookInt(__COUNTER__)[6] = { 4, (type), (u32)(arg0), (u32)(arg1), (u32)(arg2), (u32)(arg3) }

// kmWrite
// Write value to address
#define kmWritePointer(addr, ptr) kmHook3(kctWrite, 1, (addr), (ptr))
#define kmWrite32(addr, value) kmHook3(kctWrite, 2, (addr), (value))
#define kmWrite16(addr, value) kmHook3(kctWrite, 3, (addr), (value))
#define kmWrite8(addr, value) kmHook3(kctWrite, 4, (addr), (value))
#define kmWriteArea(addr, value) kmHook4(kctWriteArea, 1, (addr), (value), sizeof((value)))

// kmPatchExitPoint
// Force the end of a Kamek function to always jump to a specific address
// (if the address is 0, the end remains as-is (i.e. blr))
#define kmPatchExitPoint(funcStart, dest) kmHook2(kctPatchExit, (funcStart), (dest))

// kmBranch, kmCall
// Set up a branch from a specific instruction to a specific address
#define kmBranch(addr, ptr) kmHook2(kctInjectBranch, (addr), (ptr))
#define kmCall(addr, ptr) kmHook2(kctInjectCall, (addr), (ptr))

// kmCondBranch
// Set up a conditional branch from an address to another
enum {
    KM_COND_GREATER_OR_EQ,
    KM_COND_LESS_OR_EQ,
    KM_COND_NOT_EQ,
    KM_COND_NO_SO,

    KM_COND_REVERSE = 0x100,
    KM_COND_LESS = KM_COND_REVERSE | KM_COND_GREATER_OR_EQ,
    KM_COND_GREATER = KM_COND_REVERSE | KM_COND_LESS_OR_EQ,
    KM_COND_EQ = KM_COND_REVERSE | KM_COND_NOT_EQ,
    KM_COND_SO = KM_COND_REVERSE | KM_COND_NO_SO,
};

#define kmCondBranch(addr, ptr, cond, cr) \
    kmWrite32(addr, 0x40800000 | ((cr & 7) << 18) | ((cond & 0x103) << 16) | ((ptr - addr) & 0xFFFC))

// kmBranchDefCpp, kmBranchDefAsm
// Set up a branch (b) from a specific instruction to a function defined
// directly underneath. If exitPoint is not NULL, the function will
// branch to exitPoint when done; otherwise, it executes blr as normal
#define kmBranchDefInt(counter, addr, exitPoint, returnType, ...) \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__); \
    kmBranch(addr, kmIdentifierImpl(UserFunc, counter)); \
    kmPatchExitPoint(kmIdentifierImpl(UserFunc, counter), exitPoint); \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__)

#define kmBranchDefCpp(addr, exitPoint, returnType, ...) \
    kmBranchDefInt(__COUNTER__, addr, exitPoint, returnType, __VA_ARGS__)
#define kmBranchDefAsm(addr, exitPoint) \
    kmBranchDefInt(__COUNTER__, addr, exitPoint, asm void, )

// kmCallDefCpp, kmCallDefAsm
// Set up a branch with link (bl) from a specific instruction to a function
// defined directly underneath.
#define kmCallDefInt(counter, addr, returnType, ...) \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__); \
    kmCall(addr, kmIdentifierImpl(UserFunc, counter)); \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__)

#define kmCallDefCpp(addr, returnType, ...) \
    kmCallDefInt(__COUNTER__, addr, returnType, __VA_ARGS__)
#define kmCallDefAsm(addr) \
    kmCallDefInt(__COUNTER__, addr, asm void, )

// kmPointerDefCpp, kmPointerDefAsm
// Replace a pointer to a function with one to the function defined directly underneath.
#define kmPointerDefInt(counter, addr, returnType, ...) \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__); \
    kmWritePointer(addr, kmIdentifierImpl(UserFunc, counter)); \
    static returnType kmIdentifierImpl(UserFunc, counter) (__VA_ARGS__)

#define kmPointerDefCpp(addr, returnType, ...) \
    kmPointerDefInt(__COUNTER__, addr, returnType, __VA_ARGS__)
#define kmPointerDefAsm(addr) \
    kmPointerDefInt(__COUNTER__, addr, asm void, )
