#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <time.h>
#include "MyList.h"

MyList::MyList() {
    head = nullptr;
    size = 0;
}

MyList::MyList(std::vector<int>& arr) {
    int len = arr.size();
    if (!len) {
        MyList();
        return;
    }
    ListNode *cur = new ListNode(arr[0]);
    head = cur;
    size = len;
    for (int i = 1; i < size; i++) {
        cur->next = new ListNode(arr[i]);
        cur = cur->next;
    }
}

/** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
int MyList::get(int index) {
    if (index >= size || index < 0) {
        return -1;
    }
    ListNode *cur = head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->val;
}

void MyList::printList() {
    if (!head) {
        printf("nullptr\n");
        return;
    }
    ListNode *cur = head;
    while (cur) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

/** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
void MyList::addAtHead(int val) {
    size++;
    ListNode *cur = new ListNode(val);
    cur->next = head;
    head = cur;
}

/** Append a node of value val to the last element of the linked list. */
void MyList::addAtTail(int val) {
    size++;
    if (head == nullptr) {
        head = new ListNode(val);
        return;
    }
    ListNode *tmp = head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = new ListNode(val);
}

/** Add a node of value val before the index-th node in the linked list. 
 * If index equals to the length of linked list, the node will be appended to the end of linked list. 
 * If index is greater than the length, the node will not be inserted. */
void MyList::addAtIndex(int index, int val) {
    if (index > size || index < 0) {
        return;
    } else if(index == size) {
        addAtTail(val);
        return;
    }
    if (index == 0) {
        addAtHead(val);
        return;
    }
    ListNode *tmp = head;
    for (int i = 0; i < index - 1; i++) {
        tmp = tmp->next;
    }
    ListNode *cur = new ListNode(val);
    cur->next = tmp->next;
    tmp->next = cur;
    size++;
}

/** Delete the index-th node in the linked list, if the index is valid. */
void MyList::deleteAtIndex(int index) {
    if (index >= size || index < 0) {
        return;
    }
    size--;
    if (index == 0) {
        ListNode *cur = head;
        head = head->next;
        delete cur;
        return;
    }
    ListNode *prev = head;
    for (int i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    ListNode *deleteElem = prev->next;
    prev->next = deleteElem->next;
    delete deleteElem;
}

int MyList::getSize() {
    return size;
}

bool MyList::isPalindrome() {
    if (!head || head->next == nullptr) {
        return true;
    }
    ListNode *fastPtr = head, *slowPtr = head, *cur = head;
    while (fastPtr->next && fastPtr->next->next) {    // 这个边界条件确定了slowPtr->next的位置恰好是链表的下半部分
        fastPtr = fastPtr->next->next;
        slowPtr = slowPtr->next;
    }
    slowPtr = slowPtr->next;
    // 开始翻转链表的下半部分（其实可以直接用之前写好的函数）
    ListNode *p1 = slowPtr, *p2 = slowPtr->next;
    while (p2) {
        p1->next = p2->next;
        p2->next = slowPtr;
        slowPtr = p2;
        p2 = p1->next;
    }
    // 翻转后。举个例子，会变成1->2->3->4<-5，或者1->2->3<-4这种形式（4/3的next是nullptr）
    // 所以前一个节点是一直指向原来的4/3，所以不需要做调整
    ListNode *record = slowPtr;    // 根据之前的代码注释，此时slowPtr是新的下半部分链表的head（上面例子的5/4），保存用于最后的恢复操作
    bool res = true;
    // 开始判断是否回文
    while (slowPtr) {
        if (slowPtr->val != cur->val) {
            res = false;
            break;
        }
        slowPtr = slowPtr->next;
        cur = cur->next;
    }
    // 还原这个链表
    p1 = record, p2 = record->next;
    while (p2) {
        p1->next = p2->next;
        p2->next = record;
        record = p2;
        p2 = p1->next;
    }
    return res;
}
