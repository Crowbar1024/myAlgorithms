#include "myheader.h"


// 21 easy 合并两个有序的非递减顺序链表，可以重复节点
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


// 23 hard 合并k个有序的链表 分治
ListNode* mergeKLists(vector<ListNode*>& lists) {
    return mergeKListsHelp(lists, 0, lists.size()-1);
}
ListNode* mergeKListsHelp(vector<ListNode*>& lists, int l, int r) {
    if (l > r) return nullptr;
    if (l == r) return lists[l];
    int mid = (l+r)/2;
    return mergeTwoLists(mergeKListsHelp(lists, l, mid), mergeKListsHelp(lists, mid+1, r));
}
// 败者树
// 因为叶子节点个数n0为输入流个数n，根据完全二叉树的性质，n0=n2+1，又因为败者树不存在n1节点，所以总节点数为2n-1。
// 为了好看，n2节点下标从1开始，[1:n-1]，这样叶子节点是从n开始的，[n:2n-1]，所以一共创建2n个节点
// 理论上每次败者树返回胜者应该是最小的，所以败者应该是大的流编号
// 关键：节点是左右儿子返回值中的败者，并不是左右儿子值的败者，子树的返回值是胜者，所以是胜者中的败者
// 所以如果某个节点为INT_MAX，并不意味着它往上都是INT_MAX
class Solution {
public:
    int n; // 输入流的个数
    // 每次递归做的事情其实是记录tr[i]为左右儿子中的败者，返回tr[i]左右儿子中的胜者
    // 结果都是用输入流编号表示
    int build(vector<ListNode*>& lists, vector<int>& tr, int i) {
        // 因为[1:n-1]是非叶子节点，记录的是败者，所以最终的胜者需要存在tr[0]处，
        // 因为该节点是头节点tr[1]上的节点，只有一个直线向下的儿子
        if (!i) {
            tr[0] = build(lists, tr, 1);
            return tr[0];
        }
        if (i >= n) return tr[i] = i-n; // 叶子节点储存输入流的编号
        int lwin = build(lists, tr, i<<1); // 返回左子树的胜者
        int rwin = build(lists, tr, i<<1|1);
        // 因为某些输入流的长度有可能为0，常规的败者树就是每个输入流后面加一个INT_MAX作为哨兵，
        // 那么空指针说明就是最大的那个，即败者。
        // 即build后非叶子节点i的lists[i]为空，说明该输入流长度为0，无法比较，确实是败者
        if (!lists[lwin] || !lists[rwin]) tr[i] = lists[lwin] ? rwin : lwin;
        else tr[i] = lists[lwin]->val<lists[rwin]->val ? rwin : lwin; // 大的是败者
        return lwin+rwin-tr[i]; // winID
    }
    // 比较的规则是与父亲节点比较，胜者可以参与更高层的比较，一直向上，直到根节点。失败者留在当前节点。
    ListNode* update(vector<ListNode*>& lists, vector<int>& tr) {
        ListNode* winner = lists[tr[0]]; // 胜者所在的数据流，不能立即返回，需要更新次胜者
        if (!winner) return nullptr; // 遍历完了
        lists[tr[0]] = lists[tr[0]]->next; // 数据流前进
        int father = (tr[0]+n)>>1; // 开始自底向上需要对比的败者的非叶子节点序号
        int curID = tr[0]; // 与父节点比较的子节点，记录的是数据流编号，固定为胜者
        // 需要注意空指针的存在，因为在更新中，可能有些输入流已经遍历完了，要考虑这些情况
        while (father) {
            int fatherID = tr[father]; // 固定为败者
            if (!lists[fatherID]) {} // 如果父节点记录的是某个输入流的终止，cur会向上，即不会发生变化
            else if (!lists[curID] || lists[fatherID]->val < lists[curID]->val) { 
                // 如果子节点是某个输入流的终止，父节点不是，说明是败者，
                // 如果cur比前面的败者要大，cur更是败者。败者放到父节点
                swap(curID, fatherID);
            }
            tr[father] = fatherID; // 更新败者
            father >>= 1; // 自底向上
        }
        tr[0] = curID; // 最后的新胜者
        return winner; // 返回原来的胜者节点
    }
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        this->n = lists.size();
        // 完全二叉树，败者树，叶子节点[n:2n-1]存的是输入流的cur，非叶子节点[1:n-1]存的是左右儿子中败者的下标
        vector<int> tr(2*n, 0);
        ListNode dummy(INT_MIN), *cur = &dummy;
        build(lists, tr, 0);
        while (cur) {
            cur->next = update(lists, tr);
            cur = cur->next;
        }
        return dummy.next;
    }
};


// 2 medium 两个链表相加并输出，顺序是低位到高位，返回也是
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(INT_MIN), *tail = &dummy;
    int carry = 0;
    while (l1 || l2 || carry) {
        int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
        carry = sum / 10;
        tail->next = new ListNode(sum % 10);  // 返回的是链表，需要保存
        tail = tail->next;
        if (l1) l1 = l1->next;
        if (l2) l2 = l2->next;
    }
    return dummy.next;
}


// 61 medium 链表向右移动k个单位，尾巴回到头，类似数组向右移动，返回新的链表
ListNode* rotateRight(ListNode* head, int k) {
    if (!head) return nullptr;
    ListNode *cur = head, *newHead = nullptr;
    int len = 0;
    while (cur) {
        cur = cur->next;
        len++;
    }
    k = k % len;
    if (!k) return head;
    int cnt = 1;
    for (cur = head; cur; cur = cur->next, cnt++) {
        if (cnt == len - k) {  // 把第len-k个节点开始，最后一个节点的next接到head即可
            ListNode *tmp = cur;
            newHead = cur->next;
            while (tmp->next) {
                tmp = tmp->next;
            }
            cur->next = nullptr;
            tmp->next = head;
            break;
        }
    }
    return newHead;
}

