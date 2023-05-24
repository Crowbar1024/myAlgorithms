#include "myheader.h"

// 257 easy 输出二叉树的所有路径（root-叶子节点）
// DFS和BFS的板子应用题
// 本质上是层序遍历
vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> ret;
    queue<TreeNode*> q;
    queue<string> path;  // 这一步关键
    TreeNode* cur = root;
    q.emplace(cur);
    string s = to_string(cur->val);
    path.emplace(s);
    while (!q.empty()) {
        cur = q.front(); q.pop();
        s = path.front(); path.pop();
        if (!cur->left && !cur->right) {
            ret.emplace_back(s);
            continue;;
        } 
        if (cur->left) {
            q.emplace(cur->left);
            path.emplace(s + "->" + to_string(cur->left->val));
        }
        if (cur->right) {
            q.emplace(cur->right);
            path.emplace(s + "->" + to_string(cur->right->val));
        }
    }
    return ret;
}


