#include "../myheader.h"


/*
    记录UFS在图中除了计算联通分量，是否成环之外，其他的案例
*/

// 1102 medium 左上角走到右下角，一条路径的分数是这条路径上的最小值。存在多条路径，返回最大的分数。
// 直接看了题解，核心思想是扩散时找大的。BFS是从左上角扩散到右下角
// 那么可以用并查集，直接从中间大的开始扩散，直到两端点都扩散到
// 思想类似最小生成树的kruskal算法
struct node {
    int x, y, val;
    node(int _x, int _y, int _val) : x(_x), y(_y), val(_val) {}
    bool operator<(const node& rhs) const {
        return val < rhs.val;
    }
};
int maximumMinimumPath(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size(), ret = min(grid[0][0], grid[m - 1][n - 1]);
    priority_queue<node> pq; // 大顶堆
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            pq.emplace(i, j, grid[i][j]);
        }
    }
    vector<vector<int>> vis(m, vector<int>(n, 0));
    UFS ufs(m * n);
    while (!ufs.isRelative(0, m * n - 1)) { // 两端点没有被扩散到
        node cur = pq.top(); // 要扩散的点
        pq.pop();
        vis[cur.x][cur.y] = 1; // 因为开始不知道从哪点扩散，所以vis更新得写在弹出堆顶时
        ret = min(ret, cur.val); // 弹出节点才是真正的路径
        for (int i = 0; i < 4; ++i) {
            int nx = cur.x + dir[i], ny = cur.y + dir[i + 1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (!vis[nx][ny]) continue; // 只有两点都扩散到了，才能合并
            ufs.unit(cur.x * n + cur.y, nx * n + ny);
        }
    }
    return ret;
}
// 这道题还有并查集的做法，很精妙的思想