#include "myheader.h"

// 430 medium 拉平有子节点的双向链表，类似树的前序遍历，把child变成next
// 核心思想是把有子节点的next全部放到自己子节点的tail后
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;  // 往下指的节点
};

Node* flatten(Node* head) {
    for (Node *cur = head; cur; cur = cur->next) {
        if (cur->child) {
            Node *orignNext = cur->next;
            cur->next = cur->child;
            cur->next->prev = cur;
            cur->child = nullptr;
            Node *orignChildTail = cur->next;
            while (orignChildTail->next) orignChildTail = orignChildTail->next;
            orignChildTail->next = orignNext;
            if (orignNext) orignNext->prev = orignChildTail;
        }
    }
    return head;
}

// 138 medium 一个节点有个random节点，指向随机一个，可能是null，返回这个链表的深拷贝
// 核心思想，在原链表的每个节点后插入它的复制
class Node {
public:
    int val;
    Node* next;
    Node* random;
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};
Node* copyRandomList(Node* head) {
    if (!head) return nullptr;
    Node dummy(INT_MIN), *tail = &dummy;
    Node *cur = head;
    while (cur) {  // 每个节点后插入节点的拷贝
        Node *tmp = cur->next;
        cur->next = new Node(cur->val);
        cur = cur->next->next = tmp;
    }
    cur = head;
    while (cur) {  // 拷贝节点的随机节点是原始节点的随机节点的拷贝
        if (cur->random) cur->next->random = cur->random->next;
        cur = cur->next->next;
    }
    cur = head;
    while (cur) {
        tail = tail->next = cur->next;  // 新建的链表就是需要深度拷贝的链表
        cur = cur->next = cur->next->next;  // 恢复原来的链表结构
    }
    return dummy.next;
}