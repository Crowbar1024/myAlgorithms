#include "myheader.h"


// 222 medium 求完全二叉树的节点个数。
// 在完全二叉树中，除了最底层节点可能没填满外，其余每层节点数都达到最大值，
// 并且最下面一层的节点都集中在该层最左边的若干位置。若最底层为第 h 层，则该层包含 1~ 2h 个节点。
// 并且从上到下，从左到右，都是1,2,3的递增自然数
// 这道题的解法很多，我首先试下递归法
int countNodes(TreeNode* root) {
    if (!root) return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}
// 比较无脑，同理层序遍历，这两种方法没有利用到完全二叉树的性质。但依然事件复杂度较低，仅O(n)
// 如果要利用这个性质，也比较容易想到，左右子树的的最左一定是最长
// 于是首先写一个函数计算最长深度
int getDepth(TreeNode* root) {
    int ret =  0;
    TreeNode* cur = root;
    while (cur) {
        ++ret;
        cur = cur->left;
    }
    return ret;
}
// 如果左右子树的最长深度一致，说明右子树有可能最后一行右边有所缺漏；否则左大于右的
int countNodes(TreeNode* root) {
    if (!root) return 0;
    int ldepth = getDepth(root->left);
    int rdepth = getDepth(root->right);
    if (ldepth == rdepth) {
        return (1<<ldepth) + countNodes(root->right);
    }
    return (1<<rdepth) + countNodes(root->left);
}
// 我算了一下，O(n) = 2log(n/2)+O(n/2)，其时间复杂度为O(logn*logn)
// 其实这种方法可以继续优化，因为左子树和右子树的深度必定是左边要么比右边大1，或者相等，没有必要全写
// 只需要计算每次右子树的深度就行，而每次该深度又是其左子树的深度+1
int countNodes(TreeNode* root) {
    int depth = getDepth(root);  // 不写root->left是避免空指针引用，把空指针判断写在其他地方
    return countNodesHelp(root, depth-1);  // 同理空指针
}
int countNodesHelp(TreeNode* root, int ldepth) {
    if (!root) return 0;
    int rdepth = getDepth(root->right);
    if (ldepth == rdepth) {  // 左满
        return (1<<ldepth) + countNodesHelp(root->right, ldepth-1);
    } else {  // 右最后一行空
        return (1<<rdepth) + countNodesHelp(root->left, ldepth-1);
    }
}
// O(n) = log(n)+O1(n)
// O1(n) = logn(n/2)+O1(n/2)
// 应该比前者低一点
// 还有一种思路清奇的做法，二分法，二分的对象是最后一层的节点数目
// 因为完全二叉树的性质
// 1          0001
// 2 3        0010 0011
// 4 5 6 7    0100 0101 0110 0111
// 第i层节点的二进制的第i位为1，剩下的01序列就是从根节点到该节点的路径
// 比如6，表示root先向右，再向左
int countNodes(TreeNode* root) {
    if (!root) return 0;
    int depth = getDepth(root);
    // 如果上界取到，这里会出现一个问题 4 5 6 7，找6
    // (4+7)/2 = 5  [5,7]
    // (5+7)/2 = 6  [6,7]
    // (6+7)/2 = 6  [6,7] ...
    // 会一直循环下去
    // 所以我需要将上界取不到，并且修改while 的条件为l<r-1
    // 如果上界去得到
    int l = 1<<(depth-1), r = (1<<depth);  // [l,r)
    while (l < r-1) {
        int mid = (l+r)/2;
        if (exist(root, depth, mid)) {
            l = mid;  // 可以取到
        } else {
            r = mid;  // 取不到
        }
    }
    return l;
}
bool exist(TreeNode* root, int level, int target) {
    TreeNode* cur = root;
    int bits = 1<<(level-2);
    while (cur && bits) {
        if (target&bits) {  // 1 就是向右
            cur = cur->right;
        } else {
            cur = cur->left;
        }
        bits >>= 1;
    }
    return cur != nullptr;
}