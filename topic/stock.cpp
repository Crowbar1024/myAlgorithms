#include "myheader.h"

// 121 easy prices表示股票每天价格。选择某一天买，并选择在未来卖（一共一次），算你所能获取的最大利润。
// 如果你不能获取任何利润，返回0 
// 怎么贪，最大利润，肯定首先要选最小的天。然后动态统计结果。
int maxProfit(vector<int>& prices) {
    int minprice = INT_MAX, ret = 0;
    for (int i = 0; i < prices.size(); ++i) {
        minprice = min(minprice, prices[i]);
        ret = max(ret, prices[i]-minprice);
    }
    return ret;
}

// 122 medium 股票价格，每天只能买/卖一支股票，无限次交易，计算最大利润
// 记录上升部分就行
int maxProfit(vector<int>& prices) {
    int ret = 0;
    for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] > prices[i-1]) {
            ret += prices[i] - prices[i-1];
        }
    }
    return ret;
}

// 123 hard 最多可以完成两笔交易（可以一次）。不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。
// 4种状态的DP
// 正常情况 3 2 1 4 2 6，1-4 2-6
// 特殊情况 1 2 3 4 5 6 1-6
// buy1和sell1记录着单次最大的利润区间，如果是特殊情况，第二次买卖相当于发生在同一天
int maxProfit(vector<int>& prices) {
    int buy1 = -prices[0], buy2 = -prices[0]; // f[i][0] f[i][1] 第一次和第二次买的最大利润
    int sell1 = 0, sell2 = 0; // f[i][2] f[i][3] 第一次和第二次卖的最大利润
    for (int i = 1; i < prices.size(); ++i) { // DP
        buy1 = max(buy1, -prices[i]);
        sell1 = max(sell1, prices[i]+buy1);
        buy2 = max(buy2, sell1-prices[i]);
        sell2 = max(sell2, prices[i]+buy2);
    }
    return sell2;
}

// 188 hard k次交易的最大利润，不能同时参与多笔交易，一摸一样，变成数组
// 2k种状态的DP
int maxProfit(int k, vector<int>& prices) {
    vector<int> buy(k, -prices[0]);
    vector<int> sell(k, 0);
    for (int i = 1; i < prices.size(); ++i) { // DP
        buy[0] = max(buy[0], -prices[i]);
        sell[0] = max(sell[0], prices[i]+buy[0]);
        for (int j = 1; j < k; ++j) {
            buy[j] = max(buy[j], sell[j-1]-prices[i]);
            sell[j] = max(sell[j], prices[i]+buy[j]);
        }
    }
    return sell[k-1];
}

// 714 medium 买卖一次交易需要手续费，可以无限次交易
// 贪心策略：每次能赚钱就买，然后更新下一次买的价格，但为了避免多交手续费，下一次价格要减去手续费
int maxProfit(vector<int>& prices, int fee) {
    int ret = 0;
    int l = prices[0];
    for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] > l+fee) {  // 可以买，但为了节省手续费，期望后面遇到更大的
            ret += prices[i]-l-fee;  // 先买了
            l = prices[i]-fee;  // 遇到更大的，为了避免多交手续费
        } else if (prices[i] >= l && prices[i] <= l+fee) {  // 不卖，不够大到支付上一次的手续费
            continue;
        } else {  // 够小了，在这个价位买
            l = prices[i];
        }
    }
    return ret;
}

// 309 medium 卖完的冷冻期为1天，第二天不能买 DP
// 3种第i天结束之后的状态，这样才能明确当天有没有卖掉股票
int maxProfit(vector<int>& prices) {
    int a1 = -prices[0]; // f[i][0] 第i天结束后，持有股票的最大利润。如果该天买，说明前一天处于f[i-1][2]；没有买，说明前一天结束后还处于f[i-1][0]
    int a2 = 0; // f[i][1] 第i天结束后没有股票，但处于冷冻期的最大利润。说明前一天结束后是f[i-1][0]，该天卖了股票
    int a3 = 0; // f[i][2] 第i天结束后没有股票，也不处于冷冻期的最大利润。说明前一天结束后处于f[i-1][1]或f[i-1][2]
    for (int i = 1; i < prices.size(); ++i) {
        int tmp1 = max(a1, a3-prices[i]);
        int tmp2 = a1+prices[i];
        int tmp3 = max(a2, a3);
        a1 = tmp1;
        a2 = tmp2;
        a3 = tmp3;
    }
    return max(a2, a3);
}