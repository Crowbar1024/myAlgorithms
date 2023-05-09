#include "myheader.h"

// 325 medium 209改 找到和等于 k 的最长连续子数组长度，没有返回0
// 因为存在负数，只能试试presum了。因为求最长的[i,j] 使得presum[i]-presum[j]=k，所以使用一个map记录最早的presum[j]
int maxSubArrayLen(vector<int>& nums, int k) {
    int n = nums.size(), ret = 0;
    long sum = 0;
    vector<long> preSum(n, 0); // 前缀和可能越界
    unordered_map<long, int> pos;
    pos[0] = -1; // 记录前缀和为0的位置，因为要最早
    for (int i = 0; i < n; ++i) {
        sum += nums[i];
        if (pos.count(sum - k) > 0) {
            ret = max(ret, i - pos[sum-k]);
        }
        if (pos.count(sum) == 0) { // 因为求最长，记录最早的值
            pos[sum] = i;
        }
    }
    return ret;
}



// 238 medium 返回数组 answer ，其中 answer[i] 等于 nums 中除 nums[i] 之外其余各元素的乘积。
// 题目数据保证数组 nums之中任意元素的全部前缀元素和后缀的乘积都在 32 位 整数范围内。
// 请不要使用除法，且在 O(n) 时间复杂度内完成此题。
// 如果使用额外空间复杂度，可以用两个数组统计前缀和后缀的积
// 但可以用一次循环同时更新
vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    int prefix = 1, suffix = 1;  // 前缀积和后缀积
    for (int i = 0; i < n; ++i) {
        res[i] *= prefix;
        res[n-i-1] *= suffix;
        prefix *= nums[i];
        suffix *= nums[n-i-1];
    }
    return res;
}


// 152 medium 数组中乘积最大的非空连续子串，并返回该子数组所对应的乘积。
// dp，两个数组，一个最大，一个最小，然后状态压缩
int maxProduct(vector<int>& nums) {
    int dp1 = nums[0], dp2 = nums[0], res = nums[0];
    for (int i = 1; i < nums.size(); ++i) {
        int tmp1 = dp1, tmp2 = dp2;
        dp1 = max(tmp1*nums[i], max(nums[i], tmp2*nums[i]));
        dp2 = min(tmp2*nums[i], min(nums[i], tmp1*nums[i]));
        res = max(res, dp1);
    }
    return res;
}



// 贝壳21/5/8 笔试4
// fruitWeight为每个水果的重量
// K次查询（互相独立），每次查询给出一个s（polling的元素），问是否满足所有水果的重量等于s；
// 若不满足条件，可以从以下两个策略选择：
// 1 把所有重量超过平均数的水果扔掉；
// 2 把所有重量小于等于平均数的水果扔掉；
// 问是否可以通过执行若干次策略使条件满足。
// 这道题的难点在于k的范围<=10^5，而水果的数量也是这个范围。所以每次查询的复杂度必须是O1。
// 所以用一个哈希表去统计可以通过这2个策略的所有水果重量
// 前缀和+哈希+BFS
void fruitPolling(vector<int>& fruitWeight, vector<int>& query) {
    int n = fruitWeight.size();
    vector<ll> presum(n,0);  // 水果重量的前缀和
    sort(fruitWeight.begin(), fruitWeight.end());
    for (int i = 0; i < n; i++) {
        presum[i] += fruitWeight[i];
        presum[i] += i>0 ? presum[i-1] : 0;
    }
    unordered_set<ll> weightSet; // 所有重量的可能性，这样查询只要查这个哈希表就行
    weightSet.insert(0);
    queue<pair<int,int>> q;
	q.emplace(0,n-1);
    while (!q.empty()) {
        auto range = q.front();
		q.pop();
        int l = range.first, r = range.second;
        ll sum = l>0 ? presum[r]-presum[l-1] : presum[r]; // [l,r]的水果之和
        weightSet.insert(sum);
        int avg = sum/(r-l+1); // 这次的平均数
        // 如果[l,r]的水果重量一致，那么只能都扔掉，这样重量为0，而0已经插入过了。
        if (fruitWeight[l]==avg && fruitWeight[r]==avg) continue;
        // 分割[l,r]区间
        int pos = upper_bound(fruitWeight.begin()+l, fruitWeight.begin()+r, avg) - fruitWeight.begin();
        q.push(pair<int,int>(l,pos-1));
        q.push(pair<int,int>(pos,r));
    }
    for (int i = 0; i < query.size(); ++i) {
        if (weightSet.count(query[i])) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
}