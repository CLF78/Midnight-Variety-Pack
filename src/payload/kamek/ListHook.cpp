
void kmListHook::execute() {
    kmListHookNode* curr = head;
    while (curr != nullptr) {
        curr->func();
        curr = curr->next;
    }
}

kmCreateListHook(BootHook, 0x800074A0);
kmCreateListHook(RoomOpenHook, 0x800D1AB0);
kmCreateListHook(RaceUpdateHook, 0x8053369C);
kmCreateListHook(RaceStartHook, 0x80554728);
kmCreateListHook(NetShutdownHook, 0x80657A6C);
