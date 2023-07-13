#include "myheader.h"

// 110 easy 判断一棵树是不是AVL平衡树
// AVL需要知道其左子树高度和其右子树高度的差值。
// 分别求出其左右子树的高度，然后如果差值小于等于1，则返回当前二叉树的高度，否则则返回-1，表示已经不是二叉平衡树了。
bool isBalanced(TreeNode* root) {
    return getHeight(root) != -1;
}
int getHeight(TreeNode* root) {
    if (!root) return 0;
    int lh = getHeight(root->left);
    if (lh == -1) return -1;
    int rh = getHeight(root->right);
    if (rh == -1) return -1;
    if (abs(lh - rh) <= 1) return max(lh, rh) + 1;  // +1是加了自己
    return -1;
}

