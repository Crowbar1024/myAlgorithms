#include "myheader.h"

// 组合问题的特点
// 同一层的决策树的节点可选择数量越来越少
// 体现：for的个数本质上是当前节点的儿子个数（每个儿子是路径的一个节点）
// 而组合的dfs基本传进去的是i+1，这意味着每个儿子的儿子数量会越来越少

// 当指定某个数量的组合时，往往需要剪枝。

// 当原始的集合中存在重复元素，但又要求结果不能重复时，需要排除父节点下的相邻重复的儿子节点。
// 或者原始的集合中不存在重复元素，但又要求结果中可以存在重复数字时，dfs的下一层依然是这一层


// 78 medium 返回nums的全部子集，共2^n个
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ret;
        vector<int> snap;
        dfs(nums, 0, ret, snap);
        return ret;
    }
    void dfs(vector<int>& nums, int pos, vector<vector<int>>& ret, vector<int>& snap) {
        ret.push_back(snap);
        for (int i = pos; i < nums.size(); i++) {
            snap.push_back(nums[i]);
            dfs(nums, i+1, ret, snap);
            snap.pop_back();
        }
    }
};

// 77 medium [1:n]中k个数的全部组合 关键：剪枝
class Solution {
public:
    vector<vector<int>> ret;
    vector<int> subset;
    int len, cnt;
    vector<vector<int>> combine(int n, int k) {
        len = n, cnt = k;
        dfs(1);
        return ret;
    }
    void dfs(int pos) {
        if (subset.size() == cnt) {
            ret.emplace_back(subset);
            return;
        }
        // 一共需要cnt个数，而subset.size()是前面路径的长度，for循环长度是兄弟节点的个数，是需要被限制的
        // 因为兄弟中老幺是一条直线，没有多儿子的情况，所以可以计算老幺是谁
        // 老幺包括自己，还需要cnt-subset.size()个元素，
        // 所以n+1-(k-subset.size())处为当前决策树的这一层的最右节点老幺（+1是老幺自己要算进去）
        for (int i = pos; i <= len+1-cnt+subset.size(); ++i) {
            subset.emplace_back(i);
            dfs(i+1);
            subset.pop_back();
        }
    }
};

// 90 medium 返回子集，nums中有重复元素，但比如[1,2,2]的子集中不能有两个{1,2}
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ret;
        vector<int> snap;
        dfs(nums, 0, ret, snap);
        return ret;
    }
    void dfs(vector<int>& nums, int pos, vector<vector<int>>& ret, vector<int>& snap) {
        ret.push_back(snap);
        for (int i = pos; i < nums.size(); i++) {
            if (i > pos && nums[i] == nums[i-1]) continue;
            snap.push_back(nums[i]);
            dfs(nums, i+1, ret, snap);
            snap.pop_back();
        }
    }
};


// 216 medium 找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> ret;
        vector<int> subset;
        dfs(n, k, 1, ret, subset, 0);
        return ret;
    }
    void dfs(int n, int k, int pos, vector<vector<int>>& ret, vector<int>& subset, int sum) {
        if (sum > n) return;
        if (subset.size() == k) {
            if (sum == n) ret.emplace_back(subset);
            return;
        }
        for (int i = pos; i <= 10-k+subset.size(); ++i) {
            sum += i;
            subset.emplace_back(i);
            dfs(n, k, i+1, ret, subset, sum);
            sum -= i;
            subset.pop_back();
        }
    }
};

// 39 medium 给定一个无重复正整数的数组和一个目标数，找出数组中所有可以使数字和为目标的组合，结果中数字可以重复。
// 因为数字可以重复，所以dfs时不用进入下一层，依然进入这一层
class Solution {
public:
    vector<vector<int>> ret;
    vector<int> subset;
    int sum = 0;
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        if (candidates.empty()) return ret;
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, 0);
        return ret;
    }
    void dfs(vector<int>& candidates, int target, int pos) {
        if (sum == target) {
            ret.emplace_back(subset);
            return;
        }
        for (int i = pos; i < candidates.size(); ++i) {
            if (sum + candidates[i] > target) break;  // 后面会更大，没必要再试
            sum += candidates[i];
            subset.emplace_back(candidates[i]);
            dfs(candidates, target, i);  // 数字可以重复，所以还进入这一层
            subset.pop_back();
            sum -= candidates[i];
        }
    }
};
// 显然有DP的方法，是518的全部路径

// 40 medium 数字选择不再重复，但结果不能重复，比如[1,2,2]凑3不能有两个{1,2}
class Solution {
public:
    vector<vector<int>> ret;
    vector<int> subset;
    int sum = 0;
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        if (candidates.empty()) return ret;
        sort(candidates.begin(), candidates.end());
        dfs(candidates, target, 0);
        return ret;
    }
    void dfs(vector<int>& candidates, int target, int pos) {
        if (sum > target) return;
        if (sum == target) {
            ret.emplace_back(subset);
            return;
        }
        for (int i = pos; i < candidates.size(); ++i) {
            if (sum + candidates[i] > target) break;
            if (i > pos && candidates[i] == candidates[i-1]) continue;  // 跳过这个兄弟
            sum += candidates[i];
            subset.emplace_back(candidates[i]);
            dfs(candidates, target, i+1);
            subset.pop_back();
            sum -= candidates[i];
        }
    }
};



// 491 medium 找到所有nums的不同的非递减子序列（允许重复），子序列的长度至少是2。
// 有“不同”的要求，这意味{1,2,2}不能返回两个{1,2}
// 因为nums不是有序的，无法通过相邻比较来去除上述情况，所以需要一个额外的数据结构
// 1 用数组统计所有可能出现的数字（数据仅在[-100,100]，可行），int myset[201] = {0};
// 2 使用哈希表
// 由于只是统计父节点的所有子节点是否算过，所以这个数据结构在每次dfs需要clear
class Solution {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> ret;
        vector<int> snap;
        dfs(0, nums, snap, ret);
        return ret;
    }
    void dfs(int pos, vector<int>& nums, vector<int> &snap, vector<vector<int>> &ret) {
        if (snap.size() >= 2) ret.emplace_back(snap);
        if (pos == nums.size()) return;
        unordered_set<int> myset;
        for (int i = pos; i < nums.size(); ++i) {
            if (!snap.empty() && nums[i] < snap.back()) continue;  // 保证非递减
            if (i > pos && myset.count(nums[i]) == 1) continue;  // 避免重复
            snap.emplace_back(nums[i]);
            myset.insert(nums[i]);
            dfs(i+1, nums, snap, ret);
            snap.pop_back();
        }
    }
};



