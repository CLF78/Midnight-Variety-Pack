#include <common/Common.hpp>

void kmListHook::execute() {
    kmListHookNode* curr = this->head;
    while (curr != nullptr) {
        curr->func();
        curr = curr->next;
    }
}

kmCreateListHook(RaceStartHook, 0x80554728);
kmCreateListHook(BootHook, 0x800074A0);
kmCreateListHook(NetShutdownHook, 0x80657A6C);
