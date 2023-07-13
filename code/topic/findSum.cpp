#include "myheader.h"


// 1 easy 两数之和为target。返回这对数的下标
// 需要避免自己查自己，所以不能用用nums直接初始化。边搜索边插入
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int,int> m;
    for (int i = 0; i < nums.size(); ++i) {
        if (m.count(target-nums[i]) > 0) {
            return {m[target-nums[i]], i};
        }
        m[nums[i]] = i;
    }
    return {};
}

// 15 medium 三数之和为0，返回所有情况。 答案中不可以包含重复的三元组。
// 显然是不能unique的。排序后，固定第一个，后面两个双指针
vector<vector<int>> threeSum(vector<int>& nums) {
    int n = nums.size();
    if (n < 3) return {};
    sort(nums.begin(), nums.end());
    vector<vector<int>> ret;
    for (int i = 0; i < n-2; ++i) {  // 第一个
        if (i > 0 && nums[i] == nums[i-1]) continue; // 去重
        if (nums[i]+nums[n-2]+nums[n-1] < 0) continue; // i太小
        else if (nums[i]+nums[i+1]+nums[i+2] > 0) break; // i太大
        int l = i+1, r = n-1; // 后面两个
        while (l < r) {
            if (nums[l]+nums[r] < -nums[i]) ++l;
            else if (nums[l]+nums[r] > -nums[i]) --r;
            else {
                ret.emplace_back(vector<int>{nums[i],nums[l],nums[r]});
                ++l; --r; // 开始下一次循环
                while (l < r && nums[l] == nums[l-1]) ++l; // 去重
                while (l < r && nums[r] == nums[r+1]) --r;
            }
        }
    }
    return ret;
}

// 16 medium 最接近的三数之和，可大可小。有唯一解，返回这三个数的和。
// -1000 <= nums[i] <= 1000
// -1e4 <= target <= 1e4
// 因为是接近，所以每次结果都要保留。只有一个解，判重也没必要了
int threeSumClosest(vector<int>& nums, int target) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    int ret = 1e5; // 要很大或很小，第一次更新要中
    for (int i = 0; i < n-2; ++i) {  // 第一个
        if (i > 0 && nums[i] == nums[i-1]) continue; // 去重
        int l = i+1, r = n-1; // 后面两个
        while (l < r) { // 不管怎么遍历，都是在靠近target
            int sum = nums[l]+nums[r]+nums[i];
            if (sum < target) ++l;
            else if (sum > target) --r;
            else return target;
            if (abs(ret-target) > abs(sum-target)) ret = sum;
        }
    }
    return ret;
}

// 18 medium 四数之和，和三数之和一样的条件
// 1 <= nums.length <= 200 -1e9 <= nums[i] <= 1e9 -1e9 <= target <= 1e9
// 思路和15也一样，注意根据范围，nums[i]+nums[j]+nums[l]+nums[r]有可能越界，做一下处理
vector<vector<int>> fourSum(vector<int>& nums, int target) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    vector<vector<int>> ret;
    for (int i = 0; i < n-3; ++i) {  // 第1个
        if (i > 0 && nums[i] == nums[i-1]) continue;
        if ((long) nums[i]+nums[n-3]+nums[n-2]+nums[n-1] < target) continue;
        if ((long) nums[i]+nums[i+1]+nums[i+2]+nums[i+3] > target) break;
        for (int j = i+1; j < n-2; ++j) {  // 第2个
            if (j > i+1 && nums[j] == nums[j-1]) continue;
            if ((long) nums[i]+nums[j]+nums[n-2]+nums[n-1] < target) continue;
            if ((long) nums[i]+nums[j]+nums[j+1]+nums[j+2] > target) break;
            int l = j+1, r = n-1;
            while (l < r) {
                if ((long) nums[l]+nums[r]+nums[i]+nums[j] < target) ++l;
                else if ((long) nums[l]+nums[r]+nums[i]+nums[j] > target) --r;
                else {
                    ret.emplace_back(vector<int>{nums[i],nums[j],nums[l],nums[r]});
                    ++l; --r;
                    while (l < r && nums[l] == nums[l-1]) ++l;
                    while (l < r && nums[r] == nums[r+1]) --r;
                }
            }
        }
    }
    return ret;
}

// 454 medium 四数之和 数组长度都是 n ，请你计算有多少个元组 nums1[i] + nums2[j] + nums3[k] + nums4[l] == 0
// n <= 200
// -2^28 <= nums1[i], nums2[i], nums3[i], nums4[i] <= 2^28
// 分开了，如果继续双指针，那是n^3logn级别，没有必要。
// 所以考虑哈希表存两个数组的和的所有可能。
int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
    unordered_map<int,int> tb;
    for (const int& n1 : nums1) {
        for (const int& n2 : nums2) {
            tb[n1+n2] += 1;
        }
    }
    int cnt = 0;
    for (const int& n3 : nums3) {
        for (const int& n4 : nums4) {
            if (tb.count(-n3-n4)) cnt += tb[-n3-n4];
        }
    }
    return cnt;
}