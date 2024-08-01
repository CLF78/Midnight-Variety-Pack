#pragma once

namespace EGG {

class ListNode {
public:
    ListNode* prev;
    ListNode* next;
};

class List {
public:
    virtual ~List();

    ListNode* head;
    ListNode* tail;
};

} // namespace EGG
