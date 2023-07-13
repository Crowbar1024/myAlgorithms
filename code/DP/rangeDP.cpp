#include "myheader.h"



// 312 hard 一组气球分数，每戳爆第i个，获得nums[i-1]*nums[i]*nums[i+1]的分数，0和n+1分数可以视为虚拟的1
// 存在一组戳爆的顺序，使获得的分数最大，返回这个分数。（很想矩阵相乘复杂度）
// 定义dp[i][j]为i和j之间（不包括i和j）获得的最大分数，返回dp[0][n+1]，这也限定了i和j的范围
// 所以需要两端存储哨兵1
// 初始化：全为0
// 假设k是[i,j]之间最后一个被戳爆的气球，那么根据dp的定义，最后获得的分数是ball[i]*ball[k]*ball[j]
// 如果是第一个，显然不符合子问题的求解过程，因为这样岂不是子问题需要父问题先解出来？
// 所以dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] + ball[i]*ball[k]*ball[j])
// 画出状态转移图，发现最终结果是在右上角，是左边的一部分和下边的一部分生成，所有情况出现在右上半部分
// 于是就从左向右，从下往上迭代。
int maxCoins(vector<int>& nums) {
    int n = nums.size();
    vector<int> ball(n+2, 1);
    for (int i=1; i<=n; i++) {
        ball[i] = nums[i-1];
    }
    vector<vector<int>> dp(n+2, vector<int>(n+2, 0));
    for (int i=n; i>=0; i--) {
        for (int j=i+1; j<=n+1; j++) {
            for (int k=i+1; k<j; k++) {
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] + ball[i]*ball[k]*ball[j]);
            }
        }
    }
    return dp[0][n+1];
}



// 1547 hard 切纸带
// n是纸袋的长度，cuts是要剪的位置，每次剪一次，剪的成本就是当前剪的长度，求最低的剪的成本
// 反过来理解，全部剪完后就是一段段小的纸带，然后求拼起来的最小成本
// 和312不同，这次是连续两个
// dp[i][j]是纸带i-纸带j拼起来的最小成本，k是i与j之间最后两条纸带拼起来的位置
// 由于这里我选择了左闭右闭，所以k就是第i条纸带与第j条之间的，比如01，k就是1
// dp[i][j] = min(dp[i][j], dp[i][k]+dp[k][j]+sum[j]-[i-1])
// 注意由于sum[i-1]的存在，i需要从1开始取，所以dp还是从1开始靠谱一点
// 最终返回dp[1][m]，m为总片数，由下边和左边生成，所以迭代顺序从左往右，从下往上
int minCost(int n, vector<int>& cuts) {
    sort(cuts.begin(), cuts.end());  // 别忘了排序
    int m = cuts.size()+1;
    vector<int> nums(m+1, 0);
    vector<int> preSum(m+1, 0);
    for (int i = 1; i <= m; ++i) {
        if (i == 1) {
            nums[i] = preSum[i] = cuts[i-1];
        } else if (i < m) {
            nums[i] = cuts[i-1]-cuts[i-2];
            preSum[i] = cuts[i-1];
        } else {
            nums[i] = n-cuts[i-2];
            preSum[i] = n;
        }
    }
    vector<vector<int> > dp(m+1, vector<int>(m+1, 0));  // dp[i][i] = 0
    for (int i = m-1; i >= 1; --i) {
        for (int j = i+1; j <= m; ++j) {
            dp[i][j] = 0x3f3f3f3f;
            for (int k = i+1; k <= j; ++k) {
                dp[i][j] = min(dp[i][j], dp[i][k-1]+dp[k][j]+preSum[j]-preSum[i-1]);
            }
        }
    }
    return dp[1][m];
}

// 1000 hard 1547的进阶，k个石头拼起来的成本是其之和，求最小成本
// 1547是2个石头，312是3个石头，这里是K个石头
// 思考可以发现，如果有n个石头，最后会剩下 (n-1)%(k-1)+1个石头剩下
// dp[i][j][m] 表示第i个石头和第j石头之间合并成m个的最小成本
// 增加约束，因为可以[i,j]可以合并，所以len>=k
// 能合并成m个意味着可以合并成1和m-1（必须这样分解，子问题才能有解）
// l表示最后1堆和m-1堆的最小成本的中间那个
// dp[i][j][m] = min(dp[i][j][m], dp[i][l][1]+dp[l+1][j][m-1])
// 所以i<=l<j。同时由于可以将[i,l]分成一堆，这意味着(l-i)%(k-1)==0，所以l的step就是k-1
// 如果m==k，最后可以合并成一个，dp[i][j][1]=dp[i][j][m]+preSum[j]-preSum[i-1]
// 迭代顺序与上面一致，从下往上，从左到右
int mergeStones(vector<int>& stones, int k) {
    int n = stones.size();
    // 最后没有k个石头用来合并
    if ((n-1)%(k-1) != 0) {
        return -1;
    }
    vector<int> preSum(n+1, 0);
    for (int i = 1; i <= n; ++i) {
        preSum[i] = preSum[i-1] + stones[i-1];
    }
    vector<vector<vector<int>>> dp(n+1, vector<vector<int>>(n+1, vector<int>(k+1, 0)));
    // 写成先len的形式
    for (int len = k; len <= n; ++len) {
        for (int i = n-len+1; i >= 1; --i) {
            int j = i+len-1;
            for (int m = 2; m <= k; ++m) {
                dp[i][j][m] = 0x3f3f3f3f;
                for (int l = i; l < j; l += k-1) {
                    dp[i][j][m] = min(dp[i][j][m], dp[i][l][1]+dp[l+1][j][m-1]);
                }
            }
            dp[i][j][1] = dp[i][j][k] + preSum[j] - preSum[i-1];
        }
    }
    return dp[1][n][1];
}
// 进行状态压缩
// dp[i][j]表示尽可能多合并[i,j]之间的最低成本
// 继续之前的定义，分成左右两个部分，左边一定是1堆，但右边这里没有限制
// 因为反正最后要合并，只要保证左边一定可以成一堆就可以了
int mergeStones(vector<int>& stones, int k) {
    int n = stones.size();
    // 最后没有k个石头用来合并
    if ((n-1)%(k-1) != 0) {
        return -1;
    }
    vector<int> preSum(n+1, 0);
    for (int i = 1; i <= n; ++i) {
        preSum[i] = preSum[i-1] + stones[i-1];
    }
    vector<vector<int>> dp(n+1, vector<int>(n+1, 0));
    for (int len = k; len <= n; ++len) {
        for (int i = n-len+1; i >= 1; --i) {
            int j = i+len-1;
            dp[i][j] = 0x3f3f3f3f;
            for (int l = i; l < j; l += k-1) {
                dp[i][j] = min(dp[i][j], dp[i][l]+dp[l+1][j]);
            }
            if ((j-i)%(k-1) == 0) {
                dp[i][j] += preSum[j] - preSum[i-1];
            }
        }
    }
    return dp[1][n];
}