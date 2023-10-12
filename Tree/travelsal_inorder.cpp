#include "myheader.h"

// 94 medium 中序遍历，迭代法
// 定义：先遍历左子树，再输出头节点，最后遍历右子树
// 到了代码的实现里，由于需要先输出左儿子，所以得一直往左边遍历到空值，类似前序的while(cur)操作
// 栈顶取出的节点，按照定义左子树已经遍历完了，所以直接输出自己，再遍历自己的右子树
// 至于为什么这种写法可以保证当拿到栈顶节点时，它的左子树已经遍历过了：栈的性质
vector<int> inorderTraversal(TreeNode* root) {
    vector<int> v;
    if (!root) return v;
    stack<TreeNode*> s;
    TreeNode *cur = root;
    while (true) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top(); s.pop();
        v.push_back(cur->val);
        cur = cur->right;
    }
    return v;
}
// morris版本
// 不要栈了，所以空间复杂度只有O1
// 核心是每次遍历左子树的前提是，找到cur的左子树的最右节点（若没有，即左子树的root点）
// 如果这个节点为空，将这个节点的右子树指向cur，接着遍历cur的左子树
// 否则，说明已经cur的左子树已经遍历完了，开始遍历右子树
// 其实和后序遍历的思想有点像，既然没有栈来保存节点了，那么当前节点的cur不知道自己的左子树有没有遍历完
// 时间复杂度：O(n)，其中n是节点数。没有左子树的节点只被访问一次，有左子树的节点被访问两次。
vector<int> inorderTraversal2(TreeNode* root) {
    vector<int> v;
    if (!root) return v;
    TreeNode *cur = root;
    while (cur) {
        TreeNode* lr = cur->left;  // left subtree most right node
        if (lr) {
            while (lr->right && lr->right != cur) {
                lr = lr->right;
            }
            if (lr->right == nullptr) {
                lr->right = cur;
                cur = cur->left;
            } else {  // 说明左子树遍历完了
                lr->right = nullptr;  // 恢复结构
                v.push_back(cur->val);
                cur = cur->right;
            }
        } else {  // 没有左子树，那就输出自己，再遍历右子树
            v.push_back(cur->val);
            cur = cur->right;
        }
    }
    return v;
}








// 235是二叉搜索树的版本，思路更加简单