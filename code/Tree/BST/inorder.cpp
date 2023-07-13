#include "myheader.h"


// 530 easy BST任意两节点的差的绝对值的最小值
// 最简单的方法肯定是利用中序遍历是递增序列的性质
// 可以发现root离root节点最近的值应该是左子树的最后一层的右儿子和右子树最后一层的左儿子
// 但这种写法感觉不对，复杂度应该是O(n^2)，只适用于找离某个节点最近的情况
// 而递增序列的性质不一定要用迭代直接得到输出数组，也可以用树结构来写
// 因为拿到一个节点，其中序遍历的前者必然是其左儿子
int getMinimumDifference(TreeNode* root) {
    int ret = INT_MAX, pre = -1;
    inorder(root, pre, ret);
    return ret;
}
// ret 统计了以cur为根节点的子树的最小绝对差，pre是左父右的顺序
// 这里pre的设计就是中序遍历的内核
void inorder(TreeNode* cur, int& pre, int& ret) {
    if (!cur) return;
    inorder(cur->left, pre, ret);
    if (pre == -1) {
        pre = cur->val;
    } else {
        ret = min(ret, cur->val-pre);  // pre的顺序保证了永远为正
        pre = cur->val;
    }
    inorder(cur->right, pre, ret);
}

// 501 easy 寻找BST中的众数，若有多个，无关顺序
// 和530的思路一样，利用中序遍历的性质
// 这里我直接想套530的框架，发现是错的
// 因为比如 1 2 2，这样的遍历，当遍历到第1个2时，需要把当前的节点放进来，而不是pre
// 并且判断点是否相等与对res操作必须分开，做到逻辑清晰
class Solution {
public:
    int pre;
    int count;
    int maxCount;
    vector<int> ret;
    vector<int> findMode(TreeNode* root) {
        int pre = -1, count = maxCount = 1;
        ret.clear();
        inorder(root);
        return ret;
    }
    void inorder(TreeNode* cur) {
        if (!cur) return;
        inorder(cur->left);

        if (pre == -1) {
            pre = cur->val;
        } else if (cur->val == pre) {
            ++count;
        } else {
            count = 1;
        }
        if (count > maxCount) {
            ret.clear();
            ret.emplace_back(cur->val);
            maxCount = count;
        } else if (maxCount == count) {
            ret.emplace_back(cur->val);
        }
        pre = cur->val;
        
        inorder(cur->right);
    }
};


// 538 medium 每个节点的值变成原树中大于或等于原值之和
// 同501，530，都是用递归模拟中序遍历，这里其实就是从中序遍历的数组从末尾遍历，所以调换一下方向就行了
// 注意题干，节点数值可以取负数，所以pre初始化的值需要注意。
TreeNode* convertBST(TreeNode* root) {
    int pre = INT_MIN;
    inorder(root, pre);
    return root;
}
void inorder(TreeNode* root, int& pre) {
    if (!root) return;
    inorder(root->right, pre);
    if (pre == INT_MIN) {
        pre = root->val;
    } else {
        root->val += pre;
        pre = root->val;
    }
    inorder(root->left, pre);
}
// morris解法，其实就是把morris反过来就行了，即多余节点是当前节点右子树的最左节点
TreeNode* convertBST(TreeNode* root) {
    TreeNode* cur = root;
    int sum = 0;
    while (cur) {
        TreeNode* rl = cur->right;
        if (rl) {
            while (rl->left && rl->left != cur) {
                rl = rl->left;
            }
            if (!rl->left) {
                rl->left = cur;
                cur = cur->right;
            } else {
                sum += cur->val;
                cur->val = sum;
                rl->left = nullptr;
                cur = cur->left;
            }
        } else {
            sum += cur->val;
            cur->val = sum;
            cur = cur->left;
        }
    }
    return root;
}

// 108 easy 根据一个排好序的数组去建一课BST
// 最简单的思路，每次分一半，分别给左右儿子，按照惯例左闭右开
TreeNode* sortedArrayToBST(vector<int>& nums) {
    return sortedArrayToBSTHelp(0, nums.size(), nums);
}
TreeNode* sortedArrayToBSTHelp(int l, int r, vector<int>& nums) {
    if (l >= r) return nullptr;
    int mid = (r - l) / 2 + l;
    TreeNode *root = new TreeNode(nums[mid]);
    root->left = sortedArrayToBSTHelp(l, mid, nums);
    root->right = sortedArrayToBSTHelp(mid+1, r, nums);
    return root;
}


// 99 medium 一棵BST，只有两个节点的值交换了位置，现在需要交换回来，需要空间复杂度O1
// 第一种方案，利用BST的中序遍历是升序的性质，找到数组中不正常的两个数字，再用任何一种方法遍历树，交换两个节点的值
// 其空间复杂度是二叉树所有节点的个数
// 第二种方案，不用数组存，直接在中序遍历的同时，寻找每次弹出栈顶的元素，是否与前一个大小关系不是大于
// 其中细节就是一个升序序列中两个异常的元素xy的大小关系：
// 第一个x肯定比其后一个大，第二个y肯定比其前一个小
// 所以第一个异常的pre一下子就可以确定是x，然后将所有异常的cur视作y
// 为了找到后马上退出，可以发现如果进入了第二次异常关系，x肯定不为空，因为已经找到了
// 其空间复杂度是二叉树的高度
// 两者的时间复杂度都是On，即遍历完二叉树的时间
void recoverTree(TreeNode* root) {
    if (!root) return;
    stack<TreeNode*> s;
    TreeNode *cur = root, *pre = nullptr;
    TreeNode *x = nullptr, *y = nullptr;
    while (true) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top(); s.pop();
        if (pre && cur->val < pre->val) {
            y = cur;
            if (!x) x = pre;
            else break;
        }
        pre = cur;
        cur = cur->right;
    }
    swap(x->val, y->val);
}
// 第三种方案，比较极端，即用morris


// 98 medium 判断一棵树是不是有效的二叉搜索树，DFS
// 递归写法：左小右大，设置区间的范围
bool isValidBST(TreeNode* root) {
    if (!root) return true;
    return isValidBSTHelp(root, LONG_MIN, LONG_MAX);
}
bool isValidBSTHelp(TreeNode* root, long min, long max) {
    if (!root) return true;
    if (root->val <= min || root->val >= max) return false;
    return isValidBSTHelp(root->left, min, root->val) && isValidBSTHelp(root->right, root->val, max);
}
// 迭代写法。因为BST的中序遍历是升序序列
bool isValidBST(TreeNode* root) {
    vector<int> v;
    if (!root) return true;
    stack<TreeNode*> s;
    TreeNode *cur = root;
    while (true) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        if (s.empty()) break;
        cur = s.top();
        v.push_back(cur->val);
        s.pop();
        cur = cur->right;
    }
    for (int i = 0; i != v.size()-1; i++)
        if (v[i] >= v[i+1]) return false;
    return true;
}