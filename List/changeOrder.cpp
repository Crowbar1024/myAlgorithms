#include "myheader.h"


// 206 easy 反转链表
ListNode* reverseList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode dummy(INT_MIN, head), *prev = &dummy; // prev->next是翻转序列的头，最后是链表的开头
    ListNode *tail = prev->next; // 翻转序列的末尾，最后是链表的末尾，一直指向head，
    while (tail->next) {
        ListNode* tmp = tail->next; // 未翻转序列的头，要把这个移到翻转序列的头
        tail->next = tmp->next; // 原来的head指向未翻转序列头的下一个，最后一次会指向nullptr
        tmp->next = prev->next; // 未翻转序列头的下一个是翻转序列的头，完成反转
        prev->next = tmp;  // 新翻转序列的头就是原未翻转序列的头
    }
    return dummy.next;
}

// 92 medium 在[m,n]范围内翻转整个链表，下标从1开始
// prev的作用就是记录翻转序列的前一个，并且在翻转循环里写prev->next能够少一行结束循环时让prev->next为第m个节点的代码，因为每次都更新了
ListNode* reverseBetween(ListNode* head, int m, int n) {
    ListNode dummy(INT_MIN, head), *prev = &dummy;
    for (int i = 0; i < m-1; i++) prev = prev->next; // 记录第m-1个节点
    ListNode *tail = prev->next;
    for (int i = 0; i < n-m; i++) { // [m,n]一共有n-m+1个节点，需要翻转n-m次
        ListNode* tmp = tail->next;
        tail->next = tmp->next;
        tmp->next = prev->next;
        prev->next = tmp;
    }
    return dummy.next;
}

// 25 hard K 个一组翻转链表，如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(INT_MIN, head), *prev = &dummy;
    while (true) {
        ListNode* tmp = prev; // 检测剩下的节点数是否是k的倍数
        for (int i = 0; i < k; i++) {
            tmp = tmp->next;
            if (!tmp) return dummy.next;
        }
        ListNode *tail = prev->next;
        for (int i = 0; i < k-1; i++) { // k个数翻转k-1次
            ListNode* tmp = tail->next;
            tail->next = tmp->next;
            tmp->next = prev->next;
            prev->next = tmp;
        }
        prev = tail; // 更新下一段的prev
    }
}

// 234 easy 判断一个链表是否是回文串
// 先把后半段翻转，再进行回文的判断，最后回复原始的顺序
// 举个例子，1->2->3->4->5 翻转后。会变成1->2->3->5->4
// 或者 1->2->3->4 变成 1->2->4->3
bool isPalindrome(ListNode* head) {
    if (!head || !head->next) return true;
    ListNode dummy(INT_MIN, head), *prev = &dummy;
    ListNode *cur = prev;
    while (cur && cur->next) {
        cur = cur->next->next;
        prev = prev->next;
    }
    // 若节点数是偶数，cur指向最后一个节点，prev指向中间偏左的节点；若是奇数，cur指向nullptr，prev指向中间节点。
    cur = prev->next; // 跳转到即将翻转的序列
    while (cur->next) {
        ListNode *tmp = cur->next;
        cur->next = tmp->next;
        tmp->next = prev->next;
        prev->next = tmp;
    }
    ListNode *tmp = dummy.next; // 前串
    cur = prev->next; // 后串
    while (cur) {
        if (tmp->val != cur->val) return false; // 回文判断
        tmp = tmp->next;
        cur = cur->next;
    }
    cur = prev->next;
    while (cur->next) {
        ListNode *tmp = cur->next;
        cur->next = tmp->next;
        tmp->next = prev->next;
        prev->next = tmp;
    }
    return true;
}


// 24 medium 交换每两个相邻的节点(1234 -> 2143)
ListNode* swapPairs(ListNode* head) {
    ListNode dummy(INT_MIN, head), *prev = &dummy;
    while (prev->next && prev->next->next) {
        ListNode* l = prev->next;
        ListNode* r = l->next;
        l->next = r->next; // 所以while要这样写
        r->next = prev->next;
        prev->next = r;
        prev = l; // 跳到下一对之前
    }
    return dummy.next;
}


// NC02 (1 n) (2 n-1) ...
// 将链表头尾重新排序，n表示原最后一个
// 要求空间复杂度O1，事件On
// 感觉medium，很快想到，翻转后半部分，然后接下来的操作就是拼接
// tip1 非常关键，比如1(p1)->2->3(p2) 在进行拼接时，3->2，但2->3，产生环
// 所以一个简单的思路就是，切断前后两个将要拼接的链表
void reorderList(ListNode *head) {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) return;
    ListNode *p1 = head, *p2 = head;
    while (p2->next != nullptr && p2->next->next != nullptr) {
        p1 = p1->next;
        p2 = p2->next->next;
    }
    ListNode *newHead = p1->next;  // 需要翻转链表的头节点
    p1->next = nullptr;  // tip1
    p1 = newHead;
    p2 = newHead->next;
    while (p2 != nullptr) {
        p1->next = p2->next;
        p2->next = newHead;
        newHead = p2;
        p2 = p1->next;
    }
    p1 = head;
    p2 = newHead;
    while (p2 != nullptr) {
        ListNode *tmp1 = p1->next, *tmp2 = p2->next;
        p1->next = p2;
        p2->next = tmp1;
        p1 = tmp1;
        p2 = tmp2;
    }
}




// 86 medium 分割链表 使得所有小于 x 的节点都出现在大于或等于 x 的节点之前。
// 保留两个分区中每个节点的初始相对位置。
// 需维护两个链表，一个顺序存储所有小于的，一个顺序所有大于等于的，最后拼起来
ListNode* partition(ListNode* head, int x) {
    if (!head || !head->next) return head;
    ListNode dummy1(INT_MIN), *l = &dummy1;
    ListNode dummy2(INT_MIN), *r = &dummy2;
    while (head) {
        if (head->val < x) {
            l->next = head;
            l = l->next;
        } else {
            r->next = head;
            r = r->next;
        }
        head = head->next;
    }
    // r可能指向最后一个节点，也可能不是。如果不是的话，后面跟的是小于x的节点，所以要避免这种情况。
    r->next = nullptr;
    l->next = dummy2.next; // 拼接两个链表
    return dummy1.next;
}

// 147 medium 对链表进行插入排序
// 插入排序：在数组中待插节点的前面部分是有序序列，待插节点不断向前swap直到插入位置。
// 链表就不用swap了，但依然需要找到插入位置。
ListNode* insertionSortList(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode dummy(INT_MIN, head), *lastNode = dummy.next; // 插入位置前的有序序列的最后一个节点，初始化为第一个节点
    head = head->next; // 第一个元素默认有序，所以插入位置从第二个开始遍历
    while (head) {
        if (lastNode->val <= head->val) { // 大节点，不需要插入
            lastNode = lastNode->next;
        } else {
            // dummy->1(pre)->5->3(head)->4：把3插到1->5之间
            ListNode *pre = &dummy; // 插入位置的前一个元素，因为有可能要插在头节点前，所以要设置哨兵
            while (pre->next->val <= head->val) pre = pre->next;
            lastNode->next = head->next;
            head->next = pre->next;
            pre->next = head;
        }
        head = lastNode->next; // 有序序列的下一个就是新的待插节点
    }
    return dummy.next;
}

// 148 medium 排序链表 要求Onlogn
// 链表不能交换，所以选择分治
// 简单来说，就是利用21的两个有序链表的合并方法
// 注意因为一开始不知道尾节点，用nullptr替代，所以这里右边界需要取不到
ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    return sortListHelp(head, nullptr);
}
ListNode* sortListHelp(ListNode* head, ListNode* tail) {
    // 不会遇到 !head 的情况。
    // 同时因为tail取不到，所以遍历的终止条件是下面的判断。
    // 要将该节点拎出来，用于mergeTwoLists将两个临近的节点合并。
    if (head->next == tail) {
        head->next = nullptr;
        return head;
    }
    ListNode *slow = head, *fast = head;
    while (fast != tail && fast->next != tail) { // 注意tail可能不是nullptr
        slow = slow->next;
        fast = fast->next->next;
    }
    // 此时的slow是取不到的右边界，即奇数的中间，偶数的右边那个
    return mergeTwoLists(sortListHelp(head, slow), sortListHelp(slow, tail));
}
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(INT_MIN), *cur = &dummy;
    while (l1 && l2) {
        if (l1->val < l2->val) {
            cur->next = l1;
            l1 = l1->next;
        } else {
            cur->next = l2;
            l2 = l2->next;
        }
        cur = cur->next;
    }
    cur->next = l1 ? l1 : l2;
    return dummy.next;
}


// 328 medium 按照序号的奇偶顺序重新排列整个链表，奇前偶后。要求空间复杂度O(1)
// in-place操作，调整原链表的顺序就行了
ListNode* oddEvenList(ListNode* head) {
    if (head == nullptr) return nullptr;
    ListNode *oddcur = head, *evencur = head->next, *record = evencur;
    while (evencur && evencur->next) {
        oddcur->next = oddcur->next->next;
        evencur->next = evencur->next->next;
        oddcur = oddcur->next;
        evencur = evencur->next;
    }
    oddcur->next = record;
    return head;
}
