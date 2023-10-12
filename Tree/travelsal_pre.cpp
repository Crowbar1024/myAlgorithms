#include "myheader.h"


// 144 medium 前序遍历，递归法，返回结果保存在数组中。可以另建一个函数调用这个函数
void preTraversal(TreeNode* root, vector<int>& v){
    if(!root) return;
    v.push_back(root->val);
    preTraversal(root->left, v);
    preTraversal(root->right, v);
}
// 前序遍历，迭代法，有多种写法
// 定义：先输出头节点，然后依次遍历左子树和右子树
// 我知道的有两种写法：
// 1 每次从栈顶弹出头节点，输出之，依次压入头节点的右儿子和左儿子
// 2 每次从栈顶弹出头节点，输出之，压入右儿子，进入左儿子……直到遍历到最左边的节点
// 2比1快，因为1每次压入左儿子后，下一步的循环又要把它取出；2只压了右儿子，然后直接进入左子树的遍历中
// 其实按照定义，前序最后才遍历右子树，所以输出头节点后直接把右儿子放到栈顶就OK，反正下一步就遍历左子树了，就是2的做法
// 同时注意while(cur)这一步，栈中只保留了树中所有右节点的记录，这与下面的中序和后序的写法不同，后者全部记录了
vector<int> preorderTraversal(TreeNode* root) {
    vector<int> v;
    if (!root) return v;
    stack<TreeNode*> s;
    TreeNode *cur = root;  // 防止污染root
    while (true) {
        while (cur) {
            v.push_back(cur->val);
            if (cur->right) s.push(cur->right);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top(); s.pop();
    }
    return v;
}

// NC5 统计所有路径（head到leaf，1-2-4算124）的和
// 每条路径的值就是字符串的表值
// 注意点是传进的第二个参数不能是引用，因为这个数不能在每次路径的检索中都一直存在
int sumNumbers(TreeNode* root) {
    int sum = 0;
    return sumNumbersHelp(root, sum);
}
int sumNumbersHelp(TreeNode* node, int sum) {
    if (!node) return 0;  // 整棵树只有一个节点的处理方法
    sum = sum*10 + node->val;
    if (!node->left && !node->right) return sum; 
    return sumNumbersHelp(node->left,sum) + sumNumbersHelp(node->right,sum);
}


// Leetcode124 hard
// 给定一个二叉树的根节点root，请你计算它的最大路径和
// 这里的路径被定义为:从该树的任意节点出发，经过父=>子或者子=>父的连接，达到任意节点的序列。
// 注意:
// 1.同一个节点在一条二叉树路径里中最多出现一次
// 2.一条路径至少包含一个节点，且不一定经过根节点
// 显然，经过节点cur的路径会包含他的两条儿子路径
// 难点在于想到子函数只返回单条最长的路径
int maxPathSum(TreeNode* root) {
    int ret = INT_MIN;
    maxPathSumHelp(root, ret);
    return ret;
}
int maxPathSumHelp(TreeNode* root, int& ret) {
    if (!root) return 0;
    int lsum = max(0, maxPathSumHelp(root->left, ret));
    int rsum = max(0, maxPathSumHelp(root->right, ret));
    ret = max(ret, lsum+rsum+root->val);
    return max(lsum, rsum) + root->val;
} 


// 297 把一棵树序列化，这里是转换成string的形式，实现两种形式之间的转换
// 下面的形式是转换成前序遍历
// 每个叶子节点后面跟了两个#，可以理解为string形式中把空指针当成了#，相当于节点多了
string serialize(TreeNode* root) {
    if (root == NULL) return "#";
    return to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
}

// 使用ss用以类型转换
TreeNode* deserialize(string data) {
    if (data == "#") return nullptr;
    stringstream s(data);
    return makeDeserialize(s);
}
TreeNode* makeDeserialize(stringstream& s) {
    string str;
    // 发现第一个定界符后，s的前半部分赋值给str，删除定界符，s相当于输出了前部分，到了后部分的开头
    getline(s, str, ',');
    if (str == "#") return nullptr;
    else {
        // 前序遍历
        TreeNode* root = new TreeNode(stoi(str));
        root->left = makeDeserialize(s);
        root->right = makeDeserialize(s);
        return root;
    }
}



// 226 easy 翻转一棵二叉树
// 226 easy 翻转一棵二叉树
// 其实就是每个节点的左右儿子都交换一下
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
// 易发现是前序遍历
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    TreeNode *cur = root;
    stack<TreeNode*> s;
    while (true) {
        while (cur) {
            swap(cur->left, cur->right);
            if (cur->right) s.emplace(cur->right);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top(); s.pop();
    }
    return root;
}
// 层序自然也可以，没写