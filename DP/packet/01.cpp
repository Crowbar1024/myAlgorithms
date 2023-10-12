#include "myheader.h"



// 01背包
// 有N件物品和一个容量为V的背包。第i件物品的体积是c[i]，价值是w[i]。求解将哪些物品装入背包可使价值总和最大。
// 约束：每件物品只有1件
// dp对象：前i件物品可以选择，放入背包容量为j时所能获得的最大价值
// 状态转移：dp[i][j]=max{dp[i-1][j],dp[i-1][j-c[i]]+w[i]}
int packet01(vector<int> c, vector<int> w, int n, int v) {
    vector<vector<int>> dp(n+1, vector<int>(v+1, 0));
    for (int i = 0; i < n; i++)
        for (int j = v; j >= c[i]; j--)  // 没有压缩前方向无所谓
            dp[i][j] = max(dp[i-1][j], dp[i-1][j-c[i]] + w[i]);
    return dp[n][v];
}
// 压缩后内循环次序反过来是为了保证dp[i,j]是由上一层的dp[i-1][j-nums[i]]递推而来
int packet01(vector<int> c, vector<int> w, int n, int v) {
    vector<int> dp(v+1, 0);
    for (int i = 0; i < n; i++)
        for (int j = v; j >= c[i]; j--)
            dp[j] = max(dp[j], dp[j-c[i]] + w[i]);
    return dp[v];
}

// 压缩的初始化问题
// 一些问题要求背包全部装满，有些没有这个要求。其不同的实现就是体现在初始化上。
// 1 如果是恰好装满，初始化需要做以下两个操作：
// vector<int> dp(v+1, INT_MIN);  // 0种物品不可能装满大小为j的背包
// dp[0] = 0; // 0种物品装满0大小背包的价值是0
// 这样可以通过dp[v]是否>0表明最后装满了没。
// 2 没有恰好装满的要求，只希望总价值最大，只需要
// vector<int> dp(v+1, 0);  // 0种装大小为j的背包只有一种价值，就是0


// 同时在之后的题目可以发现，硬币相关的题目，由于背包大小肯定会类比为金钱额度，所以体积是硬币面额。
// 但作为质量那个部分既可以是1也可以是硬币的面额，无所谓
// 一个数组也符合这种情况，比如 416


// 常数级优化
int packet01(vector<int> c, vector<int> w, int n, int v) {
    vector<int> dp(v+1, 0);
    int sum = accumulate(c.begin(), c.end(), 0);
    for (int i = 0; i < n; i++) {
        sum -= c[i];  // 注意c是体积数组，从0开始，c[0]不是0
        int lo = max(v-sum, c[i]);
        for (int j = v; j >= lo; j--) {
            dp[j] = max(dp[j], dp[j-c[i]]+w[i]);
        }
    }
    return dp[v];
}



// 416 easy 一堆数，是否可以将其分为两堆数，使和相同
// 核心思路：和相同，说明和就是sum/2，即转换成了是否可以找到一个数组，恰好其和为sum/2。和找硬币一样
bool canPartition(vector<int>& nums) {
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum & 1) return false;
    int v = sum / 2;
    vector<int> dp(v+1, INT_MIN);  // 恰好装满
    dp[0] = 0;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = v; j >= nums[i]; j--)
            dp[j] = max(dp[j], dp[j-nums[i]] + 1);  // 1 换成 nums[i] 也行
    }
    return dp[v] > 0 ? true : false;
}
// 还可以利用bitset

// 1049 medium stones为石头重量，每次从中选出任意两块石头，然后将它们一起粉碎。
// 如果重量相同，就都没了；如果不同，小的没了，大的会减去小的重量再扔回去。
// 最后要么只会剩下一块石头。返回此石头最小的可能重量。要么没有石头剩下，就返回0。
// 关键在于推导出这样的关系（归纳法）：最后石块的重量 = sum_{i=0}^{n-1} k_i*stones[i]  (k_i={-1,1})
// 因为最后重量要尽可能的贴近0，所以这道题又转化成了01背包问题，找到一个和尽可能贴近sum/2的序列
int lastStoneWeightII(vector<int>& stones) {
    int sum = accumulate(stones.begin(), stones.end(), 0);
    int v = sum/2;
    vector<int> dp(v+1, 0);  // 不要求恰好装满
    for (int i = 0; i < stones.size(); ++i) {
        for (int j = v; j >= stones[i]; --j) {
            dp[j] = max(dp[j], dp[j-stones[i]]+stones[i]);
        }
    }
    // 因为向下取整，结果非负，所以奇偶不影响
    return sum-2*dp[v];
}



// SCU2018校赛决赛A n个物品，给了每个的体积和重量，给了一个背包的体积V
// 现在想要让背包装的物品的重量最重，同时让背包尽可能的装满，返回此时的最重重量和对应的占用体积
// dp[i][j] 装了前i个物品到恰好装满体积为j的背包的重量
// 明显，最后肯定会从dp[n][minV~V]中便利出最大的重量
// 最后需要得到最后一行的所有数，即dp[j]，恰好装满体积为j的背包所对应的最重重量
vector<int> maxWeightPacket(vector<int>& v, vector<int>& w, int V) {
    int n = v.size();
    vector<int> dp(V+1, INT_MIN);
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = V; j >= v[i]; j--) {
            dp[j] = max(dp[j], dp[j-v[i]]+w[i]);
        }
    }
    auto it = max_element(dp.begin(), dp.end());
    int maxW = *it;
    int maxV = distance(dp.begin(), it);
    return vector<int>{maxV, maxW};
}


// PAT n个硬币，如果恰好可以选若干个硬币（非无限），使和为m，那么返回其中硬币金额序列作为字典序最小的那个序列
// 恰好装满类型。要求返回序列，那么需要额外数组record去记录路径
// record[i][j]=true 说明此时的更新是取了第i个物品能恰好使和为j
// 正常思路是拿dp[m]从后往前遍历，一个一个减硬币，
// 如果想要字典序最小，那么先减的是最小的，所以让coins逆序排列
vector<int> shortestSeqPacket(vector<int>& coins, int m) {
    int n = coins.size();
    sort(coins.rbegin(), coins.rend());
    vector<int> dp(m+1, INT_MIN);
    dp[0] = 0;
    vector<vector<int>> record(n, vector<int>(m+1, 0));
    int sum = accumulate(coins.begin(), coins.end(), 0);
    for (int i = 0; i < n; i++) {
        sum -= coins[i];
        int lo = max(m-sum, coins[i]);  // 尝试了常数优化
        for (int j = m; j >= lo; j--) {
            if (dp[j] < dp[j-coins[i]]+1) {  // dp[j-coins[i]]+coins[i]也行
                dp[j] = dp[j-coins[i]]+1;
                record[i][j] = 1;  // 说明此时的更新是取了第i个物品
            }
        }
    }
    vector<int> seq;
    if (dp[m] < 0) return seq;  // 说明没有恰好等于m的硬币组合
    for (int i = n-1, j = m; i >= 0 && j >= 0; --i) {
        if (record[i][j] == 1) {
            seq.push_back(coins[i]);
            j -= coins[i];
        }
    }
    return seq;
}