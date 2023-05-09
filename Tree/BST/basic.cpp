#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <bitset>
#include <set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <atomic>
#include <sstream>
#include <unordered_set>
#include <numeric>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 };

// 700 easy 在BST中找一个数，递归法
TreeNode* searchBST(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    if (root->val > val) return searchBST(root->left, val);
    return searchBST(root->right, val);
}
// 迭代法
TreeNode* searchBST(TreeNode* root, int val) {
    TreeNode *cur = root;
    while (cur) {
        if (cur->val > val) cur = cur->left;
        else if (cur->val < val) cur = cur->right;
        else return cur;
    }
    return nullptr;
}


// 701 medium 往一棵BST中插入一个节点，迭代法
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    TreeNode *cur = root;
    while (cur) {
        if (cur->val > val) {
            if (cur->left) cur = cur->left;
            else {
                cur->left = new TreeNode(val);
                break;
            }
        } else {
            if (cur->right) cur = cur->right;
            else {
                cur->right = new TreeNode(val);
                break;
            }
        }
    }
    return root;
}
// 递归版本
// 需要注意返回值肯定是指针，那么如何保存root呢，当然是root->left=的形式
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else root->right = insertIntoBST(root->right, val);
    return root;
}


// 450 medium 根据key删除BST的某个节点，没有重复的节点
// 这道题的难点是小心delete的使用
TreeNode* deleteNode(TreeNode* root, int key) {
    TreeNode dummy(INT_MIN), *prev = &dummy;
    dummy.left = root;
    while (root) {  // 遍历到要删的节点位置，并记录这个节点的父亲
        if (key < root->val) {
            prev = root;
            root = root->left;
        } else if (key > root->val) {
            prev = root;
            root = root->right;
        } else break;
    }
    if (!root) return dummy.left;  // 没有值为key的节点
    // 此时root就是要删除的节点
    if (!root->left) {  // root没有左儿子，那就直接挂它的右子树
        if (prev->left == root) prev->left = root->right;
        else prev->right = root->right;
        delete root;
    } else if (!root->right) {
        if (prev->left == root) prev->left = root->left;
        else prev->right = root->left;
        delete root;
    } else {  // 最复杂的情况，左右儿子都有
        TreeNode *tmp = root->right, *tmpPrev = root;
        while (tmp->left) {  // 要找个合适的值去代替root，一般选择右子树中最小的，或者左子树中最大的
            tmpPrev = tmp;
            tmp = tmp->left;
        }
        swap(root->val, tmp->val);  // 只交换了字面值，tmp这个指针指的就是要被交换的那个节点
        if (tmpPrev == root) root->right = tmp->right;  // while没有执行
        else tmpPrev->left = tmp->right;
        delete tmp;
    }
    return dummy.left;
}

// 669 medium 只保留BST[low,high]范围内的节点，其他删除
// 我居然一遍过了。果然确定返回类型和终止条件就OK了大部分。
TreeNode* trimBST(TreeNode* root, int low, int high) {
    if (!root) return nullptr;
    if (root->val > high) {
        return trimBST(root->left, low, high);
    } else if (root->val < low) {
        return trimBST(root->right, low, high);
    }
    root->left = trimBST(root->left, low, high);
    root->right = trimBST(root->right, low, high);
    return root;
}



// 938 给了BST的头节点，和一个范围[low,high]，返回树所有满足这个范围的节点的和
// The number of nodes in the tree is in the range [1, 20000].
// 1 <= Node.val <= 105
// 1 <= low <= high <= 105
// All Node.val are unique.
int rangeSumBST(TreeNode* root, int low, int high) {
    if (!root) return 0;
    if (root->val < low) return rangeSumBST(root->right, low, high);
    else if (root->val > high) return rangeSumBST(root->left, low, high);
    else return root->val + rangeSumBST(root->right, low, high) + rangeSumBST(root->left, low, high);
}


// 235 easy 找到节点p和q最小的公共祖先。最大的祖先就是root。一个节点的祖先可以是它自己。
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    while (root) {
        // p和q都在root右子树
        if (root->val < p->val && root->val < q->val) root = root->right;
        // p和q都在root左子树
        else if (root->val > p->val && root->val > q->val) root = root->left;
        else return root;
    }
    return nullptr;
}
// 235是普通树的进阶版。


