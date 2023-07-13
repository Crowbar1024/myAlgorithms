#include "myheader.h"

// 106 medium 利用中序和后序遍历建树
// 左父右  左右父
// 可以找到左子树和右子树，父节点的序号
TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    return buildTreeHelp(inorder, 0, inorder.size() - 1, postorder, 0, postorder.size() - 1);
}
TreeNode* buildTreeHelp(vector<int>& inorder, int inbegin, int inend, 
                        vector<int>& postorder, int postbegin, int postend) {
    if (inbegin > inend || postbegin > postend) return nullptr;
    TreeNode *root = new TreeNode(postorder[postend]);
    int pos = inbegin;
    // 由于树里的元素不重复，可以用unordered_map来省去循环，或者用find
    for (int i = inbegin; i <= inend; ++i) {  // 别忘记inend是可以取到的
        if (inorder[i] == root->val) {
            pos = i;
            break;
        }
    }
    root->left = buildTreeHelp(inorder, inbegin, pos - 1,
                                postorder, postbegin, postbegin + pos - inbegin - 1);
    root->right = buildTreeHelp(inorder, pos + 1, inend, 
                                postorder, postbegin + pos - inbegin, postend - 1);
    return root;
}

// 105 medium 利用前序和中序建树
// 父左右 左父右
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    return buildTreeHelp(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
}
TreeNode* buildTreeHelp(vector<int>& preorder, int prebegin, int preend, vector<int>& inorder, int inbegin, int inend) {
    if (inbegin > inend || prebegin > preend) return nullptr;
    TreeNode *root = new TreeNode(preorder[prebegin]);
    int pos;
    for (int i = inbegin; i <= inend; ++i) {
        if (inorder[i] == root->val) {
            pos = i;
            break;
        }
    }
    root->left = buildTreeHelp(preorder, prebegin + 1, prebegin + pos - inbegin, inorder, inbegin, pos - 1);
    root->right = buildTreeHelp(preorder, prebegin + pos - inbegin + 1, preend, inorder, pos + 1, preend);
    return root;
}


// 654 medium 给定一个不含重复元素的整数数组。一个以此数组构建的最大二叉树定义如下：
// 二叉树的根是数组中的最大元素。
// 左子树是通过数组中最大值左边部分构造出的最大二叉树。
// 右子树是通过数组中最大值右边部分构造出的最大二叉树。
// 通过给定的数组构建最大二叉树，并且输出这个树的根节点。
// 尽量使用左闭右开
TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
    return constructMaximumBinaryTreeHelp(nums, 0, nums.size());
}
TreeNode* constructMaximumBinaryTreeHelp(vector<int>& nums, int l, int r) {
    if (l >= r) return nullptr;
    auto it = max_element(nums.begin()+l, nums.begin()+r);
    TreeNode *root = new TreeNode(*it);
    int idx = distance(nums.begin(), it);
    root->left = constructMaximumBinaryTreeHelp(nums, l, idx);
    root->right = constructMaximumBinaryTreeHelp(nums, idx+1, r);
    return root;
}