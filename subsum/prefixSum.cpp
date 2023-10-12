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
    vector<int> ret(n, 1);
    int prefix = 1, suffix = 1;  // 前缀积和后缀积
    for (int i = 0; i < n; ++i) {
        ret[i] *= prefix;
        ret[n-i-1] *= suffix;
        prefix *= nums[i];
        suffix *= nums[n-i-1];
    }
    return ret;
}


// 560 medium 整数数组nums（有负数），求所有的字串之和为k的连续子串的个数
// preSum[i]-preSum[j-1]=k
// 对于每一个preSum[i]，如果前面有多个符合题意的preSum[j-1]，cnt就可以加上这个数量
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> sumPos;
    sumPos[0] = 1;  // 和为0的情况初始化为1，即前0个数之和
    int preSum = 0, ret = 0;
    for (int& num : nums) {
        preSum += num;
        if (sumPos.count(preSum-k)) {
            ret += sumPos[preSum-k];
        }
        sumPos[preSum] += 1;
    }
    return ret;
}


// 523 medium 判断正整数数组是否含有同时满足下述条件的连续子数组：
// 子数组大小至少为 2 ，且子数组元素总和为 k 的倍数（0 始终视为 k 的一个倍数）。保证全部和在int范围。
// 因为是判断，可以记录<presum, idx>，idx取最早的
// presum[i] - presum[j-1] = n*k
// presum[i]%k = presum[j]%k  因此只需要计算每个下标对应的前缀和%k
bool checkSubarraySum(vector<int>& nums, int k) {
    int n = nums.size(), preSum = 0, ret = 0;
    if (n < 2) return false;
    unordered_map<int, int> sumEarliestPos;
    sumEarliestPos[0] = -1;  // 和为0
    for (int i = 0; i < n; ++i) {
        preSum = (preSum + nums[i]) % k;
        if (sumEarliestPos.count(preSum) && sumEarliestPos[preSum] <= i - 2) { // j-1
            return true;
        } else if (!sumEarliestPos.count(preSum)) {
            sumEarliestPos[preSum] = i;
        }
    }
    return false;
}
// 974 medium 523进阶 整数数组（有负数），求和可被k整除的子串数量 即找出所有 presum%K 中有多少对相等的pair
// 需要注意presum=0的情况，假设有三个不同的{i,j,k}，有pre[i]=pre[j]=pre[k]=0，虽然可以组成3对
// 但忽视了一个情况，就是之间的片段[i+1,j],[j+1,k],[i+1,k]也是0，所以n个0，除了组合数之外，还要+n（1为特殊情况）
int subarraysDivByK(vector<int>& nums, int k) {
    unordered_map<int, int> sumCnt;
    int ret = 0, sum = 0;
    for (int n : nums) {
        int tmp = sum + n;
        // 负数在Cint上取模是和正数关于原点中心对称（且为负数），但我这里需要和正数取模的向下取整保持一致
        if (tmp < 0) sum = tmp-(tmp-k+1)/k*k;
        else sum = tmp%k;
        if (sum == 0) ++ret; // 先把presum为0的全统计了
        sumCnt[sum]++;
    }
    for (auto [n, cnt] : sumCnt) {
        ret += cnt*(cnt-1)/2;  //组合数
    }
    return ret;
}




// 152 medium 数组中乘积最大的非空连续子串，并返回该子数组所对应的乘积。
// dp，两个数组，一个最大，一个最小，然后状态压缩
int maxProduct(vector<int>& nums) {
    int dp1 = nums[0], dp2 = nums[0], ret = nums[0];
    for (int i = 1; i < nums.size(); ++i) {
        int tmp1 = dp1, tmp2 = dp2;
        dp1 = max(tmp1*nums[i], max(nums[i], tmp2*nums[i]));
        dp2 = min(tmp2*nums[i], min(nums[i], tmp1*nums[i]));
        ret = max(ret, dp1);
    }
    return ret;
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