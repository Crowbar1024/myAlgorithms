#include "myheader.h"

// 203 easy 删除所有为val的元素
ListNode* removeElements(ListNode* head, int val) {
    ListNode dummy(INT_MIN, head), *cur = &dummy;
    while (cur->next) {
        if (cur->next->val == val) {
            ListNode *tmp = cur->next;
            cur->next = cur->next->next;
            tmp = tmp->next = nullptr;
            delete tmp;
        } else cur = cur->next;
    }
    return dummy.next;
}

// 83 easy easy 升序链表，删除所有重复的，让所有元素只出现一次
ListNode* deleteDuplicates(ListNode* head) {
    ListNode dummy(INT_MIN, head), *cur = &dummy;
    while (cur->next && cur->next->next) {
        if  (cur->next->val == cur->next->next->val) {
            ListNode* tmp = cur->next;
            cur->next = cur->next->next;
            tmp = tmp->next = nullptr;
            delete tmp;
        } else cur = cur->next;
    }
    return dummy.next;
}

// 237 medium 删除链表中的node节点，要求给定节点的值不应该存在于链表中。保证node不是最后一个节点。
// 实际上只需要把node的值变成node->next的值，然后删除node->next
void deleteNode(ListNode* node) {
    node->val = node->next->val;
    ListNode* tmp = node->next;
    node->next = tmp->next;
    tmp = tmp->next = nullptr;
    delete tmp;
}

// 19 medium 删除倒数第n个点
// x0x0x 删除倒数第2个节点，即0
// p1先不走，p2先到正数第2个节点，然后两个指针再往后走
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(INT_MIN, head), *p1 = &dummy, *p2 = &dummy;
    while (n--) p2 = p2->next; // p2到了正数第2个节点
    while (p2->next) {
        p1 = p1->next; // p1到倒数第2个0的前一个x
        p2 = p2->next; // p2到了最后一个节点x
    }
    ListNode *tmp = p1->next;
    p1->next = tmp->next;
    tmp = tmp->next = nullptr;
    delete tmp;
    return dummy.next;
}