#include "myheader.h"

// 145 medium 后序遍历，迭代法
// 定义：依次遍历左子树和右子树，最后输出头节点
// 第一步，和中序一样，为了先输出左儿子，所以得一直往左边遍历到空值，即while(cur)操作
// 同时按照定义，在弹出当前节点时，就默认了已经遍历完了这个节点的左子树和右子树
// 为了连贯性，继承中序的写法，那么在拿到栈顶节点时，就有了一个问题：它的右子树是否被遍历过？
// 所以后序和中序在实现上的不同之处就在于，在弹出栈顶节点时，记录这个节点
// 由于后序遍历一直是遍历完子树后再输出，所以上次记录的节点对应当前的节点来说，有多种可能：
// 1 自己的N级父节点的左兄弟。这种情况属于当前节点属于某个节点的右子树的一直往左的节点的某一个，没触底，所以还没被弹出。
// 2 自己有右儿子，但last不是自己的右儿子，那么说明右子树没有遍历过，开始遍历
// 3 自己没有右儿子，反正左子树已经遍历过了，last是谁无所谓，弹出自己并把自己设为last
// 同时，后序遍历的遍历次序决定了，while(cur)的每一次压栈操作，栈里的元素顺序就是树的head到cur的路径，这个性质很有用
// 解释一下为什么中序没有这个特点：因为中序的cur需要弹出自己再进入右儿子，当然路径就不完整了
// 先遍历自己的子树再输出自己，叫做回溯法，可以用于很多种问题
vector<int> postorderTraversal(TreeNode* root) {
    vector<int> v;
    if (!root) return v;
    stack<TreeNode*> s;
    TreeNode *cur = root, *last = nullptr;
    while (true) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top();
        // 进入右子树
        if (cur->right && cur->right != last) {
            cur = cur->right;
            // 注意这里没有弹出节点，这保证了栈可以存放路径
        } else {  // 左子树和右子树已经遍历过了
            v.push_back(cur->val);
            s.pop();
            last = cur;
            cur = nullptr;  // cur已经被弹出了，要取出第二个栈顶了，所以不能进入while(cur)
        }
    }
    return v;
}


// 104 easy 找树最大的深度
// 后序遍历，层序遍历都行
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    int ret = max(maxDepth(root->left), maxDepth(root->right)) + 1;
    return ret;
}
// 看了下递归的形式，就是后序遍历
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    stack<TreeNode*> s;
    TreeNode *cur = root, *last = nullptr;
    int ret = 0;
    while (true) {
        while (cur) {
            s.push(cur);
            ret = max(ret, (int)s.size());
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top();
        if (cur->right && cur->right != last) cur = cur->right;
        else {
            s.pop();
            last = cur;
            cur = nullptr;    
        }
    }
    return ret;
}


// 112 easy 是否存在头节点到叶子节点的一条路径，使得路径之和为sum，递归写法
// 注意([],0)的输入被判false
// 后来发现这是叶子节点的问题，因为真正结束是在叶子节点处才知道求和成不成功
bool hasPathSum(TreeNode* root, int sum) {
    if (!root) return false;
    if (!root->left && !root->right) return sum == root->val;
    return hasPathSum(root->left, sum - root->val) || hasPathSum(root->right, sum - root->val);
}
// 迭代写法，medium 核心思想是后序遍历的栈的入栈时栈的快照符合树路径这个性质
bool hasPathSum(TreeNode* root, int sum) {
    if (!root) return false;
    stack<TreeNode*> s;
    TreeNode *cur = root, *last = nullptr;
    int pathSum = 0;
    while (true) {
        while (cur) {
            pathSum += cur->val;
            s.push(cur);  // 此时栈中保存了从head到cur的完整路径
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top();
        if (cur->right && cur->right != last) cur = cur->right;
        else {
            if (!cur->left && !cur->right && pathSum == sum) return true;
            pathSum -= cur->val;
            s.pop();
            last = cur;
            cur = nullptr;    
        }
    }
    return false;
}

// 113 medium 112进阶，找到所有符合条件的路径，NC8同
// 后序遍历法更优秀，因为只需要在上面加一行代码，复用性更强
vector<vector<int>> pathSum(TreeNode* root, int sum) {
    vector<vector<int>> v;
    if (!root) return v;
    stack<TreeNode*> s;
    TreeNode *cur = root, *last = nullptr;
    int ret = 0;
    vector<int> path;
    while (true) {
        while (cur) {
            ret += cur->val;
            path.push_back(cur->val);
            s.push(cur);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top();
        if (cur->right && cur->right != last) cur = cur->right;
        else {
            if (!cur->left && !cur->right && ret == sum) {
                v.push_back(path);
                // 注意此时不能continue，因为要弹出当前节点
            }
            ret -= cur->val;
            path.pop_back();  // 注意也要从path弹出
            s.pop();
            last = cur;
            cur = nullptr;    
        }
    }
    return v;
}



// 236 medium 找到节点p和q最小的公共祖先。最大的祖先就是root。一个节点的祖先可以是它自己。
// 递归的思路一定是确定参数和返回值，明显原来的三个参数就是递归函数的参数了，所以分析开头退出递归的情况。
// 如果root为空，没什么好说的，返回空。
// 然后就是最核心的一点，如果任意两节点就是root，那么返回root。
// 这样左右节点的返回值，如果只有一个为空，那么说明非空的就是最近的公共祖先。
// 如果都有值，说明root才是。
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode *left = lowestCommonAncestor(root->left, p, q);
    TreeNode *right = lowestCommonAncestor(root->right, p, q);
    return !left ? right : right ? root : left;
}
// 遍历写法的心思想是后序遍历的栈的入栈时栈的快照符合树路径这个性质，保存root到p,q的path
// 至于为什么要用vector去模拟栈，因为需要从头遍历两条path
// 然后找到第一次出现分歧的点，这个点的前一个就是一开始相同路径的最后一个点，即最小的祖先
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    vector<TreeNode*> s, sp, sq;
    TreeNode *cur = root, *last = nullptr;
    while (true) {
        while (cur) {
            s.emplace_back(cur);
            if (cur == p) sp = s;
            if (cur == q) sq = s;
            if (!sp.empty() && !sq.empty()) break;
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.back();
        if (cur->right && cur->right != last) cur = cur->right;
        else {
            s.pop_back();
            last = cur;
            cur = nullptr;
        }
    }
    int n = min(sp.size(), sq.size());
    for (int i = 1; i != n; ++i) {
        if (sp[i] != sq[i]) return sp[i - 1];
    }
    return sp[n - 1];  // 这种情况返回值是p和q中某一个是另一个的子节点
}
// 这道题也有树是BST的例子。