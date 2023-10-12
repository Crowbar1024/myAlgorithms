#include "../myheader.h"

// 1102 medium 左上角走到右下角，一条路径的分数是这条路径上的最小值。存在多条路径，返回最大的分数。
// 直接看了题解，设置一个大顶堆，每次把当前的能达到的下一个位置放到大顶堆，然后走到堆顶。
// 这种就是BFS，扩散到终点，能保证一路上都是相对最大的值。
// 因为每次向右下方扩散，每次到达的一圈，取其中的最大值与当前路径最小值比较，这样才能得到想要的结果
struct node {
    int x, y, val;
    node(int _x, int _y, int _val) : x(_x), y(_y), val(_val) {}
    bool operator<(const node& rhs) const {
        return val < rhs.val;
    }
};
int maximumMinimumPath(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    int ret = min(grid[0][0], grid[m - 1][n - 1]); // 路径最小值中的最大值
    priority_queue<node> pq; // 大顶堆
    pq.emplace(0, 0, grid[0][0]); // 不放坐标，放二维对应的一维值
    vector<vector<int>> vis(m, vector<int>(n, 0));
    vis[0][0] = 1;
    while (!pq.empty()) {
        node cur = pq.top(); // 每次从大顶堆中取到最大的，可以保证该点一定能到达
        pq.pop();
        ret = min(ret, cur.val); // 弹出节点才是真正的路径
        if (cur.x == m - 1 && cur.y == n - 1) return ret;
        for (int i = 0; i < 4; ++i) {
            int nx = cur.x + dir[i], ny = cur.y + dir[i + 1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (vis[nx][ny]) continue;
            pq.emplace(nx, ny, grid[nx][ny]);
            vis[nx][ny] = 1;
        }
    }
    return ret;
}
// 这道题还有并查集的做法，很精妙的思想