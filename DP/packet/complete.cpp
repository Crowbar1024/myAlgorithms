#include "myheader.h"


// 完全背包
// 有N种物品（每种无限）和一个容量为V的背包。第i种物品的体积是c[i]，价值是w[i]。求解将哪些物品装入背包可使价值总和最大。
// dp对象：前i种物品可以选择，放入背包容量为j时所能获得的最大价值
// 由于无限，而dp[i][j]=max(dp[i-1][j],dp[i-1][j-K*c[i]])的后者就需要先得到





// 322 easy 给定不同面额的硬币（无限）和一个总金额，计算可以凑成总金额所需的最少的硬币个数。
// dp[i][j] 前i种物品恰好装满体积为j的背包时的最小重量
// 由于是min，所以初始化不能无穷小，得是无穷大，还得防止溢出
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount+1, 0x3f3f3f3f);  // 恰好装满类型
    dp[0] = 0;
    for (int i = 0; i < coins.size(); i++) {
        for (int j = coins[i]; j <= amount; j++) {
            dp[j] = min(dp[j], dp[j-coins[i]]+1);
        }
    }
    return dp[amount] == 0x3f3f3f3f ? -1 : dp[amount];
}


// 518 easy 给定不同面额的硬币（无限）和一个总金额，计算可以组成这个数额的组合数
// dp[i][j] 前i种硬币，可以组成数额j的组合数
// 恰好装满的统计组合数类型
int change(int amount, vector<int>& coins) {
    vector<int> dp(amount+1, 0);
    dp[0] = 1;  // 0种硬币恰好组成金额0就是1种组合
    for (int i = 0; i < coins.size(); i++) {
        for (int j = coins[i]; j <= amount; j++) {
            dp[j] += dp[j-coins[i]];
        }
    }
    return dp[amount];
}
// 39 medium 给定一个无重复正整数的数组和一个目标数，找出数组中所有可以使数字和为目标的组合，数字可以重复。
// 数组长度<=30，每个数<=200，目标<=500
// 相当于518的路径
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<vector<int>>> dp(target+1);
    for (int i = 0; i < candidates.size(); ++i) {
        for (int j = candidates[i]; j <= target; ++j) {
            if (j == candidates[i]) {  // 只有一个元素的情况
                dp[j].push_back({candidates[i]});
            } else {
                for (auto v : dp[j-candidates[i]]) {
                    v.emplace_back(candidates[i]);
                    dp[j].emplace_back(v);
                }
            }
        }
	}
    return dp[target];
}
// 说实话我一开始没想到DFS，不过这是官方解。

// 279 medium 找到若干个完全平方数（可以重复）使得它们的和等于 n。你需要让组成和的完全平方数的个数最少。
// 硬币问题。要求硬币数量最少。
// 这里换一下遍历顺序也行
int numSquares(int n) {
    vector<int> dp(n+1, 0x3f3f3f3f);
    dp[0] = 0;
    for (int i = 1; i <=n; ++i) {
        for (int j = 1; j*j <= i; ++j) {
            dp[i] = min(dp[i], dp[i-j*j]+1);
        }
    }
    return dp[n];
}


// 139 medium s是否可以由wordDict中的word组成（word可以重复）
// s = "applepenapple", wordDict = ["apple", "pen"]
// 相当于挑选硬币组成，可以无限选，所以是完全背包
// dp[j]表示s的前j个字母是否由wordDict中的word组成
// 如果当dp[j]为真时，且s[j:i]为wordDict中的一个，那么dp[i]也为真
bool wordBreak(string s, vector<string>& wordDict) {
    unordered_set<string> Dict(wordDict.begin(), wordDict.end());
    vector<bool> dp(s.size()+1, false);
    dp[0] = true;  // 哨兵，这样才能进行更新
    for (int i = 1; i <= s.size(); ++i) {
        for (int j = i-1; j >= 0; --j) {
            if (dp[j]) {
                string tmp = s.substr(j, i-j);
                if (Dict.find(tmp) != Dict.end()) {
                    dp[i] = true;
                    break;
                }
            }
        }
    }
    return dp[s.size()];
}