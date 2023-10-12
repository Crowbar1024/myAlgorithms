#include "myheader.h"


// 比较难以想到的递归法




// 234 easy 判断一个链表是否是回文串
// 判断回文的递归解
// 用check把p递归到末尾，然后一层层往栈顶上走
// 本来是把第一个参数设置为外部的一个变量，但这样写更cool
// 因为递归调用了n次，所以空间复杂度是On
bool isPalindrome(ListNode* head) {
    return check(head, head);
}
bool check(ListNode*& head, ListNode* p) {
    if(!p) return true;
    bool isPal = check(head, p->next);
    if(!isPal || head->val != p->val) return false;
    // 因为传进来的head是指针的引用，更新后，函数返回还是更新值。
    // 如果是指针，会消亡，毕竟还是变量
    head = head->next;
    return isPal;
}


// hard 反转栈，只能使用递归，inplace操作。
// 比如[1,2,3，先需要返回1，并且要保留[2,3的栈的结构
// 最后要把1压入栈，那么递归函数在弹出栈底和压栈之间
void reverseStack(stack<int>& s) {
    if (s.empty()) return;
    int bottom = getStackBottom(s);
    reverseStack(s);
    s.push(bottom);
}
// 简单来说，就是先不断弹出栈顶，然后不断压入栈，那么递归函数的位置就很清楚了，在弹出和压入之间
// 且返回的是相同的一个数，即栈底。
int getStackBottom(stack<int>& s) {
    int x = s.top();
    s.pop();
    if (s.empty()) {
        return x;    
    }
    int bottom = getStackBottom(s);
    s.push(x);
    return bottom;
}