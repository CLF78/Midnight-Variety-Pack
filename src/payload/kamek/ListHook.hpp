#define kmDefineListHook(listName) \
    extern kmListHook listName

#define kmCreateListHook(listName, addr) \
    kmListHook listName = kmListHook(); \
    kmBranchDefCpp(addr, NULL, void) { listName.execute(); }

#define kmListHookAdd(listName, func) \
static kmListHookNode kmIdentifier(List, __COUNTER__) = kmListHookNode((kmListHook*)&listName, (Func)&(func))

#define kmListHookDefInt(counter, listName) \
    static void kmIdentifier(ListFunc, counter) (); \
    kmListHookAdd(listName, kmIdentifier(ListFunc, counter)); \
    static void kmIdentifier(ListFunc, counter) ()

#define kmListHookDefCpp(listName) \
    kmListHookDefInt(__COUNTER__, listName)

class kmListHookNode;

class kmListHook {
public:
    kmListHookNode* head;

    kmListHook() {}
    void execute();
};

class kmListHookNode {
public:
    kmListHookNode* next;
    Func func;

    kmListHookNode(kmListHook* parent, Func function) : func(function) {
        this->next = parent->head;
        parent->head = this;
    }
};

kmDefineListHook(RaceStartHook);
kmDefineListHook(BootHook);
kmDefineListHook(NetShutdownHook);
