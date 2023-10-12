#include "myheader.h"

// 102 medium 层序遍历
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> ret;
    if (!root) return ret;
    queue<TreeNode*> q;
    TreeNode *cur = root;
    q.push(cur);
    while (!q.empty()) {
        vector<int> level;
        int n = q.size();  // 不能写进循环，因为长度会变
        for (int i = 0; i < n; i++) {
            cur = q.front(); q.pop();
            level.emplace_back(cur->val);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        ret.emplace_back(level);
    }
    return ret;
}
// 107 mudium 逆序输出，reverse就行
// 109 medium 输出右侧，只要每次for中记录最后的元素
// 637 easy 输出每层平均数，注意sum需要long
// 515 medium 每行最大值
// 429 medium 多叉树，根本没区别
// 104 easy 找树最大的深度，有递归法；层序遍历把当前这层的树的所有节点遍历完，这一轮就是一层的计数
// 111 easy 二叉树最短的深度，同104，当某一层的节点没有左右儿子时，自然是最短的，所以后序有点麻烦，只要叶子节点
// 513 easy 找树最后一行的最左值，层序遍历每次for取第一个就行；或者改变层序遍历顺序，先放右儿子，这样最后遍历到的点就是结果

// 116 medium 给出了一棵完美二叉树，即所有叶子节点在同一层，所有父节点有两个儿子
// 现在需要将每一层的节点的next指向右边的节点，如果没有，指向nullptr
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node() : val(0), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
Node* connect(Node* root) {
    if (!root) return nullptr;
    queue<Node*> q;
    Node *cur = root;
    q.push(cur);
    while (!q.empty()) {
        vector<int> level;
        int n = q.size();
        Node* curPre = nullptr;
        for (int i = 0; i < n; i++) {
            if (!i) {
                curPre = q.front(); q.pop();
                cur = curPre;
            } else {
                cur = q.front(); q.pop();
                curPre = curPre->next = cur;
            }
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        cur->next = nullptr;
    }
    return root;
}
// 117 medium 116变体，但给的树不是完美二叉树，只是普通的二叉树，一摸一样



// 101 easy 判断一棵树是否可以成镜像，有递归法
// 迭代法，依然是层序遍历，与minDepth和maxDepth一致
// 每一层放入的顺序就是l->left r->right l->right r->ledft
// 保证每两个拿出来是成镜像的一组


// NC14 层序遍历每层输出顺序正反交替
vector<vector<int> > Print(TreeNode* pRoot) {
    vector<vector<int>> v;
    if (!pRoot) return v;
    queue<TreeNode*> q;
    TreeNode *cur = pRoot;
    q.push(cur);
    bool flag = false;
    while (!q.empty()) {
        vector<int> level;
        int n = q.size();
        while (n--) {
            cur = q.front();
            q.pop();
            level.push_back(cur->val);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        if (flag) {
            reverse(level.begin(), level.end());
        }
        v.push_back(level);
        flag = !flag;
    }
    return v;
}


