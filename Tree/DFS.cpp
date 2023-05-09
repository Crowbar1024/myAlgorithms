#include "myheader.h"

// 257 easy 输出二叉树的所有路径（root-叶子节点）
// 典型的DFS和BFS都能轻易解决的问题
vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> res;
    string path = "";
    dfs(path, root, res);
    return res;
}
void dfs(string& path, TreeNode* cur, vector<string>& res) {
    if (!cur) return;
    string s = to_string(cur->val);
    path += s;
    if (!cur->left && !cur->right) {
        res.emplace_back(path);
    } else {
        // 选择列表只有两个
        path += "->";
        dfs(path, cur->left, res);
        if (cur->left) path.erase(path.end()-to_string(cur->left->val).size()-2, path.end());
        else path.erase(path.end()-2, path.end());

        path += "->";
        dfs(path, cur->right, res);
        if (cur->right) path.erase(path.end()-to_string(cur->right->val).size()-2, path.end());
        else path.erase(path.end()-2, path.end());
    }
}
// 没有引用的做法，效率略低于上面
void dfs(string path, TreeNode* cur, vector<string>& res) {
    if (!cur) return;
    string s = to_string(cur->val);
    path += s;
    if (!cur->left && !cur->right) {
        res.emplace_back(path);
    } else {
        path += "->";
        dfs(path, cur->left, res);
        dfs(path, cur->right, res);
    }
}
// 考虑一下两种方法的时间复杂度，显然都是O(n^2)级别的
// 前者一共访问了n次节点，每次都会对path进行erase操作，erase复杂度O(n)
// 后者一共访问了n次节点，每次都会对path进行拷贝构造，复杂度O(n)
// 空间复杂度就需要考虑递归的栈空间了
// 最坏的情况：所有节点只有一个节点，即深度为n的树，递归层数就是n
// 前者只在一个string上操作，空间复杂度就是O(n)
// 后者每层的复杂度是O(i)，累计O(n^2)
// 最好的情况，是一棵AVL树，后者递归栈深度logn，1+2+...+logn，累计O((logn)^2)





// 113 medium 112进阶，找到所有符合条件的路径
// DFS版本，存在路径和选择列表，由于dfs没有判断为空，所以我写了两个条件判断
// 小问题，没说没有负数，所以剪枝没有必要
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    if (!root) return vector<vector<int>>{};
    vector<vector<int>> res;
    vector<int> path;
    TreeNode *cur = root;
    dfs(res, cur, targetSum, path);
    return res;
}
void dfs(vector<vector<int>>& res, TreeNode* cur, int sum, vector<int>& path) {
    path.push_back(cur->val);
    sum -= cur->val;
    if (!cur->left && !cur->right) {
        if (!sum) res.push_back(path);
        return;
    }
    if (cur->left) {
        dfs(res, cur->left, sum, path);
        path.pop_back();
    }
    if (cur->right) {
        dfs(res, cur->right, sum, path);
        path.pop_back();
    }
}

// 404 easy 左叶子值之和
int sumOfLeftLeaves(TreeNode* root) {
    if (!root) return 0;
    return dfs(root);
}
int dfs(TreeNode* cur) {
    int res = 0;
    if (cur->left) {
        res += isLeaf(cur->left) ? cur->left->val : dfs(cur->left);
    }
    if (cur->right) {
        res += isLeaf(cur->right) ? 0 : dfs(cur->right);
    }
    return res;
}
bool isLeaf(TreeNode* cur) {
    return !cur->left && !cur->right;
}
// 时间复杂度就是节点数目On
// 最坏空间复杂度有依然是链式的子树的深度，n层，每层只有一个变量，所以也是On
// 层序遍历也是可行的，只要加入叶子节点判断