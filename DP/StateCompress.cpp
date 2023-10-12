#include "myheader.h"


// 1986 medium 可以用一个框sessionTime装一些数，可以不装满，问最少几个框用掉所有数
// n<15，所以可以用状态压缩DP做
// dp[i]=1101，表示用了第1，3，4个数，状态表达为13
int minSessions(vector<int>& tasks, int sessionTime) {
    int n = tasks.size(), m = 1 << n;
    vector<int> dp(m, 0x3f3f3f3f);
    // 如果该状态代表的数的和能被框装下，标记为1
    for (int i = 1; i < m; ++i) { // 题干中0是无效的
        int state = i, idx = 0, sum = 0; // idx记录state的二进制表示
        while (state) { // 记录当前状态代表的数之和是否能被框装下，比如1101每次拿1，3，4位置的数
            if (state&1) sum += tasks[idx];
            state >>= 1; // 不能用state&(state-1)，因为要知道每个1位置对应的tasks的位置
            ++idx;
        }
        if (sum <= sessionTime) dp[i] = 1; // 能被装下
    }
    // 枚举子集
    for (int i = 1; i < m; i++) {
        if (dp[i] == 1) continue; // 已经是最优解
        int split = i >> 1; // 把枚举次数减半
        for (int j = (i - 1) & i; j > split; j = (j - 1) & i) {
            dp[i] = min(dp[i], dp[j] + dp[i^j]); // 子集+对应的补集，枚举次数减半
        }
    }
    return dp[m-1];
}


// 1601 n栋楼[0:n-1]，有m次请求<x,y>，即从x搬到y。一开始所有楼都是满的，最后需要满足每栋楼员工净变化为0
// 返回最多能满足的请求数。
int maximumRequests(int n, vector<vector<int>>& requests) {
    vector<int> bd(n); // building，每栋楼的人数变化
    int ret = 0, m = requests.size(), num = 1 << m;
    for (int state = 1; state < num; ++state) { // 得遍历完所有状态
        int cnt = __builtin_popcount(state); // 为了剪枝，先使用O1的算法将该状态
        if (cnt <= ret) continue;
        fill(bd.begin(), bd.end(), 0);
        for (int i = 0; i < m; ++i) { // 遍历请求
            if (state & (1 << i)) {
                ++bd[requests[i][0]];
                --bd[requests[i][1]];
            }
        }
        if (all_of(bd.begin(), bd.end(), [](int x) { return x == 0; })) {
            ret = cnt;
        }
    }
    return ret;
}