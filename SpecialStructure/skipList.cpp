#include "myheader.h"

/*
    跳表 是在 O(log(n)) 时间内完成增加、删除、搜索操作的数据结构。空间复杂度是 O(n)。

    跳表相比于树堆与红黑树，其功能与性能相当，并且跳表的代码长度相较下更短，其设计思想与链表相似。

    跳表实际为一种多层的有序链表，跳表的每一层都为一个有序链表，且满足每个位于第 i 层的节点有 P 的概率出现在第 i+1 层，其中 P 为常数。

    跳表在进行查找时，首先从当前的最高层开始查找，在当前层水平地逐个比较直至当前节点的下一个节点大于等于目标节点，
    然后移动至下一层进行查找，重复这个过程直至到达最底层（具有全部元素）。
    此时，若下一个节点是目标节点，则成功查找；反之，则元素不存在。由于从高层往低层开始查找，由于低层出现的元素可能不会出现在高层，
    因此跳表在进行查找的过程中会跳过一些元素，相比于有序链表的查询，跳表的查询速度会更快。

*/

/*
    -1 1   3        nullptr
    -1 1   3 5      nullptr
    -1 1  2 3 5 6 7  nullptr
    head节点的val为1，vector的长度是3（竖着摆放，head的长度就是跳表的高度）。
    next[i]表示当前节点的第i个level对应的下一个节点，比如head的next[1]为3
    上面一共有6个ListNode，每个ListNode的长度可能不同
*/
// 1206 hard 设计跳表
// 由于数据范围为[0,20000]，并且增删过程中头节点可能会变，所以设置哨兵节点为-1
// 注意，跳表中可能存在多个相同的值，你的代码需要处理这种情况。即重复值
// 所以删除操作，如果存在多个 num ，删除其中任意一个即可。
// 这样的话，删除和添加操作会发生改变。比如删除，3个5的长度不同，从上往下，可能会误删高的。
// 所以最优选择是给列节点一个cnt属性即可。
const int MAX_LEVEL = 32;  // head的最大长度，即跳表真实高度
struct SkiplistNode {
    int val;
    int cnt;
    vector<SkiplistNode*> next;
    // 设置每一列的值和长度
    SkiplistNode(int val_, int _level = MAX_LEVEL, int cnt_ = 1) : val(val_), next(_level, nullptr), cnt(cnt_) {}
};
class Skiplist {
private:
    int height;  // 默认为0（没有任何数据）。为1，即没有缓存，只有第0层实际数据层。这是实际高度，不是真实高度
    // 每次添加一个节点，有概率height会增大。同理删除会下降。
    SkiplistNode* head;  // 哨兵节点，最前面的一列

    // 返回 1 表示当前插入的该元素不需要建索引，只需要存储数据到原始链表即可（概率 1/2）
    // 返回 2 表示当前插入的该元素需要建一级索引（概率 1/4）
    // 返回 3 表示当前插入的该元素需要建二级索引（概率 1/8）...
    int generateRand() {
        int level = 1;
        // 每次50%概率脱离该循环
        while (rand()%2==1 && level<MAX_LEVEL) ++level;
        return level;
    }
public:
    Skiplist() : height(0), head(new SkiplistNode(-1)) {}

    // 利用next寻找在node在curLevel这一层的后面节点中恰好比target小的节点，即target是第一个大于等于返回值的节点
    // 所以该函数不会返回nullptr。
    SkiplistNode* findPre(SkiplistNode* node, int curlevel, int target) {
        while (node->next[curlevel] != nullptr && node->next[curlevel]->val < target) {
            node = node->next[curlevel];
        }
        return node;
    }

    bool search(int target) {
        SkiplistNode *cur = head;
        for (int i = height-1; i >= 0; --i) {  // 从最上面那个level开始搜索
            cur = findPre(cur, i, target);
            if (cur->next[i] != nullptr && cur->next[i]->val == target) {
                return true;
            }
        }
        return false;
    }
    
    void add(int num) {
        // 首先判断存在不，存在就数量+1，返回了。
        SkiplistNode *cur = head;
        for (int i = height-1; i >= 0; --i) {  // 从最上面那个level开始搜索
            cur = findPre(cur, i, num);
            if (cur->next[i] != nullptr && cur->next[i]->val == num) {  // 找到了，数量+1
                cur->next[i]->cnt += 1;
                return;
            }
        }
        cur = head;
        int level = generateRand();  // 表示新建的数据长度（1就是只有一个最下层）
        SkiplistNode* node = new SkiplistNode(num, level);
        for (int i = height-1; i >= 0; --i) {
            cur = findPre(cur, i, num);
            if (i < level) {  // 该层level需要插值
                if (cur->next[i] == nullptr) {
                    cur->next[i] = node;
                } else {
                    SkiplistNode* tmp = cur->next[i];
                    cur->next[i] = node;
                    node->next[i] = tmp;
                }
            }
        }
        // 单独的一层里面有两个点，哨兵head的值和num，所以head的长度一开始设置的很大
        for (int i = height; i < level; ++i) head->next[i] = node;
        height = max(height, level);
    }
    
    // 如果有多个，只删一个，即cnt-1，cnt为0就全部删除
    bool erase(int num) {
        if (search(num) == false) return false;  // 首先判断找得到不
        SkiplistNode *cur = head, *node = nullptr;
        for (int i = height-1; i >= 0; --i) {
            cur = findPre(cur, i, num);
            if (cur->next[i] != nullptr && cur->next[i]->val == num) {
                if (cur->next[i]->cnt > 1) {  // 直接减一个返回
                    cur->next[i]->cnt -= 1;
                    return;
                } else {  // 只有等于1的情况，等于0，第一行已经离开了
                    if (!node) node = cur->next[i];  // 定位到要删的列
                    cur->next[i] = cur->next[i]->next[i];
                }
            }
        }
        delete node;
        for (int i = height-1; i >= 0; --i) {
            if (head->next[i] == nullptr) {
                height = i+1;  // 高度减小
                break;
            }
        }
        return true;
    }
};