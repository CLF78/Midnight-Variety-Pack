#pragma once

namespace EGG {

class ListNode {
public:
    ListNode* prev;
    ListNode* next;
};
size_assert(ListNode, 0x8);

class List {
public:
    virtual ~List();

    ListNode* head;
    ListNode* tail;
};
size_assert(List, 0xC);

} // namespace EGG
