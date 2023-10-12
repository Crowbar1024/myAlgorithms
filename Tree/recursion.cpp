#include "myheader.h"




// 101 easy 判断一棵树是否可以成镜像
bool isSymmetric(TreeNode* root) {
    if (!root) return true;
    return isSymmetricHelp(root->left, root->right);
}
bool isSymmetricHelp(TreeNode* l, TreeNode* r) {
    if (!l || !r) return l == r;
    if (l->val != r->val) return false;
    return isSymmetricHelp(l->left, r->right) && isSymmetricHelp(l->right, r->left);
}

// 100 easy 判断两棵树是否完全一样
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p || !q) return p == q;
    if (p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

// 572 easy 两棵二叉树root和subRoot。检验root中是否包含和subRoot具有相同结构和节点值的子树。
bool isSubtree(TreeNode* root, TreeNode* subRoot) {
    if (!root) return false;
    return isSubtreeHelp(root, subRoot) ||
            isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
}
bool isSubtreeHelp(TreeNode* l, TreeNode* r) {
    if (!l || !r) return l == r;
    if (l->val != r->val) return false;
    return isSubtreeHelp(l->left, r->left) && isSubtreeHelp(l->right, r->right);
}


// 617 easy 合并两棵二叉树，重叠的节点是原来节点之和
TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
    if (!root1 && !root2) return nullptr;
    TreeNode *cur = new TreeNode(0);
    if (root1 && root2) {
        cur->val= root1->val + root2->val;
        cur->left = mergeTrees(root1->left, root2->left);
        cur->right = mergeTrees(root1->right, root2->right);
    } else if (root1 && !root2){
        cur->val = root1->val;
        cur->left = mergeTrees(root1->left, nullptr);
        cur->right = mergeTrees(root1->right, nullptr);
    } else {
        cur->val = root2->val;
        cur->left = mergeTrees(nullptr, root2->left);
        cur->right = mergeTrees(nullptr, root2->right);
    }
    return cur;
}
// 以上是我的写法，很繁琐，所有节点都是新建的，而且递归深度可以更短，因为可以直接返回非空的节点
TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {
    if (!root1) return root2;
    if (!root2) return root1;
    root1->val += root2->val;
    root1->left = mergeTrees(root1->left, root2->left);
    root1->right = mergeTrees(root1->right, root2->right);
    return root1;
}