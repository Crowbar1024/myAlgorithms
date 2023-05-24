#include "../myheader.h"

// 547 medium 给了一个 n*n 的邻接矩阵，求图中连通图的数量
int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size(), ret = 0;
    vector<bool> isVisit(n, false);
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (isVisit[i]) continue;
        q.emplace(i);
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            isVisit[cur] = true; // 设置的时间点顺序要注意
            for (int j = 0; j < n; ++j) {
                if (!isVisit[j] && isConnected[cur][j]) {
                    q.emplace(j);
                }
            }
        }
        ++ret;
    }
    return ret;
}
