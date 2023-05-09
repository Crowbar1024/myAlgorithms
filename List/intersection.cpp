#include "myheader.h"

// 141 easy 单链表是否有环
// 双指针解法，利用快慢两个指针去找：如果有环，那么快的必定会套圈遇到慢的。
bool hasCycle(ListNode *head) {
    if (!head || !head->next) return false;
    ListNode *p1 = head, *p2 = head;
    while (!p2 && !p2->next) {
        p1 = p1->next;
        p2 = p2->next->next;
        if (p1 == p2) {
            return true;
        }
    }
    return false;
}
// 还可以用unordered_set<ListNode*> s;


// 142 medium 单链表是否有环，如果有返回环的入口
// 双指针解法
// 起点到环入口的距离：x1
// 环入口到第一次相遇点的距离：x2（假如有环，一定相遇于环中某点）
// 接着让两个指针分别从相遇点和链表头出发，两者都改为每次走一步，最终相遇于环入口
// 第一次相遇点到环入口的距离（实际上是第二次相遇点）：x3
// 这样x2+x3就是环的周长
// 下面是第一次相遇时各自走过的路程
// 慢的路线：x1+x2+m(x3+x2)
// 快的路线：x1+x2+x3+x2+n(x3+x2)，体现快多走的性质
// 快的路线长度是慢的路线的2倍
// 得到x1 = x3 + (x2+x3)*(n-2m)
// 所以从相遇点和起点同时同速出发，必定相遇在环的起点
ListNode *detectCycle(ListNode *head) {
    if (head == nullptr || head->next == nullptr) return nullptr;
    ListNode *p1 = head, *p2 = head;
    while (p2 != nullptr && p2->next != nullptr) {  // 避免空指针引用
        p1 = p1->next;
        p2 = p2->next->next;
        if (p1 == p2) {
            p2 = head;
            while (p1 != p2) {
                p1 = p1->next;
                p2 = p2->next;
            }
            return p2;
        }
    }
    return nullptr;
}

// 160 两个单链表的末端重叠在一起，找到重叠部分的交点
// 假设表1前半部分a，表2前半部分b，公共部分c（因为一个节点只有一个next，所以会有一个公共部分）
// 让指针p1走完表1后走表2，p2走完表2后走1
// a-c-b = b-c-a
// 所以至多会在第2次经过交点时碰头
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (headA == nullptr || headB == nullptr) return nullptr;
    ListNode *p1 = headA, *p2 = headB;
    while (p1 != p2) {
        p1 = p1->next;     // BUG1：为图方便用了三元赋值法，p1 = nullptr ? headB : p1->next 显然需要注意顺序，如果走完后都是空指针，那么就会直接返回空指针
        p2 = p2->next;
        if (p1 == p2) return p1;
        if (p1 == nullptr) p1 = headB;
        if (p2 == nullptr) p2 = headA;
    }
    return p1;
}


// 287 medium 数组长度为n+1，是由[1,n]中的数组成（可以不用完），其中一个数字重复了多次，找到这个重复的数字
// 位运算解法很简单，这里不用位运算（先全域异或，在异或数组）
// 即把数组画成链表，举个例子：2231，0->2 1->2 2->3 3->1
// 0->2->3->1->2，如果有自己指向自己的且没有其他指向自己，就是一个孤岛，不需要画出来
// 显然可见，重复的元素就是环的起点，根据快慢指针142的思路，要找的就是环的起点
// 然后从相遇点和起点同时同速再次出发，下一个相遇点必定是环的起点。
// 因为必定有环，所以在while判断数组末尾无法像链表一样判断是否空指针，只能判断是否相等，这就导致快慢指针的初始位置不能相等
// 所以要让他们先走一次
int findDuplicate(vector<int>& nums) {
    int slowPtr = nums[0], fastPtr = nums[nums[0]];  // 都先走一次
    while (slowPtr != fastPtr) {
        slowPtr = nums[slowPtr];
        fastPtr = nums[nums[fastPtr]];
    }
    fastPtr = 0;  // 一个在起点，一个在第一次相遇点
    while (slowPtr != fastPtr) {
        slowPtr = nums[slowPtr];
        fastPtr = nums[fastPtr];
    }
    return slowPtr;
}

