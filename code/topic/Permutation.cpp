#include "myheader.h"


// 首先明确一个概念，决策树一般没有 head 节点，
// DFS 的 for 遍历本质上是遍历以当前节点 pos 为首的同层兄弟节点。（排列和组合都是如此）
// 而初始化都是拿数组的头元素的下标 0 作为兄弟中的第一个。而下标恰好又能和决策树的路径长度结合在一起。

// 排列和组合的区别就在 dfs 传进去参数的区别，前者 pos+1，后者 i+1
// 前者意味着节点的每个子路径长度相同，而后者会越来越短

// DFS是符合字典序的，所以也可以输出结果的排名


// 1 避免 swap 写法（虽然能节省好多空间，但控制不好）
// 2 使用 used 额外空间时需要注意边界条件。比如 for 循环都是从 0 开始



// 46 medium 全排列
// 有swap的写法
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> ret;
        dfs(nums, 0, ret);
        return ret;
    }
    void dfs(vector<int>& nums, int pos, vector<vector<int>>& ret) {
        if (pos == nums.size()) {
            ret.push_back(nums);
            return;
        }
        for (int i = pos; i < nums.size(); i++) {  // 同层节点
            swap(nums[i], nums[pos]); // 同层的兄弟节点和第一个老大pos交换位置。比如1-2-x-x遍历完了之后，第2层遍历到了3，交换位置变成 1-3-2
            dfs(nums, pos+1, ret);  // 然后进入第三层，此时3的子树就是由2和4组成
            swap(nums[i], nums[pos]);  // 兄弟子树遍历完了，换下个兄弟了，得把位置恢复了
        }
    }
};
// 如果使用交换，这就意味着需要用一个snap记录每次的路径。
// 如果不适用额外空间，会出现一个问题，比如1->2->3遍历完了，下次1->3时，进去的还是3，这意味着1->3->3
// 现在就可以发现 swap 的优秀之处了：他把兄弟中的老大也换到了当“层”节点的屁股后面，即后续会遍历到曾经的老大
// 而现在的方法不会遍历到老大，因为老大没有过去。
// 为了解决这个问题，需要引入 used 数组去记录。这时候需要注意一件事，不能传pos了。原因如下：
// 举个例子，1-2-(4)-3，到了4时，进入下层，下层没有3，需要nums都遍历一遍。这就是需要 used 的原因。
class Solution {
public:
    vector<int> snap;  // 决策树一条路径
    vector<bool> used;
    vector<vector<int>> ret;
    vector<vector<int>> permute(vector<int>& nums) {
        used.resize(nums.size(), false);
        dfs(nums);
        return ret;
    }
    void dfs(vector<int>& nums) {
        if (snap.size() == nums.size()) {
            ret.push_back(snap);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (!used[i]) {
                snap.emplace_back(nums[i]);
                used[i] = true;
                dfs(nums);
                used[i] = false;
                snap.pop_back();
            }
            
        }
    }
};

// 47 medium 46进阶 给定一个可包含重复数字的序列，按任意顺序返回所有不重复的全排列。
// [1,1,2]---->[[1,1,2],[1,2,1],[2,1,1]]
// 根据题意，那么兄弟节点之间重复只能选一个。
// 根据这个需求，那么节点之间需要排序。
// 那么当 nums[i] == nums[i-1] 需要注意一件事，就是以 nums[i-1]为首的子树有没有遍历完？
// 遍历完了说明兄弟nums[i]不能用了。
class Solution {
public:
    vector<int> snap;
    vector<bool> used;
    vector<vector<int>> ret;
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        used.resize(nums.size(), false);
        dfs(nums);
        return ret;
    }
    void dfs(vector<int>& nums) {
        if (snap.size() == nums.size()) {
            ret.emplace_back(snap);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;  // 相同的兄弟遍历过了
            if (!used[i]) {
                snap.emplace_back(nums[i]);
                used[i] = true;
                dfs(nums);
                used[i] = false;
                snap.pop_back();
            }
        }
    }
};

// 31 medium 下一个排列，如果已经最大，让它最小
// ...s[i]...s[j]...
// 4,5,(2),6,(3),1
// 从后往前找到第一个s[i]<s[i+1]的i，然后找到后面的第一个大于s[i]的s[j]，交换i和j，然后i后面的翻转就行
// 原理：下一个排列即恰好大于当前排列，所以要找到末尾的降序数组的前一个元素s[i]
// 所以要选择其中恰好大于s[i]那个才是恰好大于。并且交换后，由于原来s[j]>s[j+1]，如果s[i]<s[j+1]，那么找到就不是i了
// 所以交换后能保证降序，此时将i后面的翻转就是正序，即是答案。
void nextPermutation(vector<int>& nums) {
    int i = nums.size()-1;
    while (i--) {
        if (nums[i] < nums[i+1]) break;
    }
    if (i >= 0) {
        for (int j = nums.size()-1; j > i; --j) {
            if (nums[j] > nums[i]) {
                swap(nums[j], nums[i]);
                break;
            }
        }
    }
    reverse(nums.begin()+i+1, nums.end()); // 一种是i=-1的，即已经最大的情况
}


// 526 medium [1:n]组成的排列，每个元素只要满足下述条件之一，该数组就是一个优美的排列，问优美排列个数
// perm[i] 能够被 i 整除（下标从 1 开始）
// i 能够被 perm[i] 整除
// 虽说是排列，但不能用swap去做，原因显而易见，交换了之后，可以判断i上的数字，但pos位置上的
void dfs(vector<int>& perm, int pos, int& cnt) {
    if (pos == perm.size()) {
        cnt += 1;
        return;
    }
    for (int i = pos; i < perm.size(); ++i) {
        if (perm[i]%pos==0 || pos%perm[i]==0) { // 由于i在pos之后，所以pos位置已经判断过了
            swap(perm[i], perm[pos]);
            dfs(perm, pos+1, cnt);
            swap(perm[i], perm[pos]);
        }
    }
}
int countArrangement(int n) {
    vector<int> perm(n+1, 0);
    for (int i = 1; i <= n; ++i) perm[i] = i;
    int cnt = 0;
    dfs(perm, 1, cnt);
    return cnt;
}

// 60 hard 给定 n 和 k，返回第 k 个排列。
// 如果用dfs的解法遍历，虽然能保证顺序，但时间复杂度是kn，但k最大值有可能是n!，所以不能这么做
// 这是康托展开的经典例子：康托展开表示的就是在n个不同元素的全排列中，比当前排列组合小的个数
// 原理：n的排列数为n!，那么以1为第一个元素的排列数为(n-1)!
class Solution {
public:
    const vector<int> fac = {1, 1, 2, 6, 24, 120, 720, 5040, 40320}; // 阶乘，n<=9，所以最多算(9-1)!
    
    // 计算s的康托展开值
    // 比如 34152
    // 4152比3小的有1和2，就是2*(n-1)!
    // 152中小于4的所有排列只有1和2，就是2*(n-2)!
    // 52比1小的不存在，就是0*(n-3)!
    // 2比5小的是2，就是1*(n-4)!
    // ""比如2小不存在，为0
    int Cantor(string s) {
        int n = s.length(), ret = 0;
        for (int i = 0; i < n; ++i) {
            int cnt = 0;  // 在当前位小于当前排列的排列数量
            for (int j = i+1; j < n; ++j) {
                if (s[j] < s[i]) cnt += 1;
            }
            ret += cnt*fac[n-i-1]; // 康托展开累加
        }
        return ret;
    }

    // 通过康托展开值得到升序序列s的第k个康托展开排列
    // 12345，算康托展开值为61的序列
    // 用 61 / 4! = 2%13，说明在第1位之后比第1位小的数有2个，所以首位为3。
    // 用 13 / 3! = 2%1，说明在第2位之后小于第2位的数有2个，所以第二位为4。
    // 用 1 / 2! = 0%1，说明在第3位之后没有小于第3位的数，所以第三位为1。
    // 用 1 / 1! = 1%0，说明在第4位之后小于第4位的数有1个，所以第四位为5。
    // 最后一位自然就是剩下的数2
    // 通过以上分析，所求排列组合为 34152。
    string deCantor(string s, int k) {
        int n = s.length();
        string ret(n, '0');
        for (int i = n-1; i >= 0; --i) {
            int cnt = k/fac[i];  // 在第i位之后比第i位小的数的数量
            ret[n-1-i] = s[cnt]; // 注意位置
            s.erase(s.begin()+cnt); // 已经用了，删掉原位置这个数
            k %= fac[i];
        }
        return ret;
    }

    string getPermutation(int n, int k) {
        string s(n, '0');
        for (int i = 0; i < n; ++i) s[i] += i+1; // [1:n]
        return deCantor(s, k-1);
    }
};


