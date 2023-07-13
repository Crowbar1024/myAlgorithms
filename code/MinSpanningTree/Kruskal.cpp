#include "myheader.h"

/*
    生成树：一个连通图的生成树是指一个连通子图，它含有图中全部n个顶点，但只有足以构成一棵树的n-1条边。
    一颗有n个顶点的生成树有且仅有n-1条边，如果生成树中再添加一条边，则必定成环。

    最小生成树：在连通图的所有生成树中，所有边的代价和最小的生成树，称为最小生成树。（说明是有权边）

    Kruskal
    伪代码
    1. 将所有边的按权重从小到大排序。
    2. 从小到大遍历这些边。当遍历边<u,v>时,如果点u和v属于两棵规模小的最小生成树，则将两点合并成一个规模大的最小生成树。
    3. 总费用加上这条边的费用。
    
    技术细节
    1. 排序可以用直接排序，或者使用最小堆
    2. 利用并查集检测顶点是否之间被访问过
    
*/


// 1135 medium 最小生成树板子题 [1:n]城市，返回连接所有城市的最低成本。如果无法连接所有 n 个城市，返回 -1。
int minimumCost(int n, vector<vector<int>>& connections) {
    if (connections.size() < n - 1) return -1; // 最小生成树的边数为n-1
    sort(connections.begin(), connections.end(), [](vector<int>& lhs, vector<int>& rhs) {
        return lhs[2] < rhs[2];
    });
    UFS ufs(n);
    int ret = 0;
    for (auto& conn : connections) {
        if (ufs.isRelative(conn[0] - 1, conn[1] - 1)) continue; // 输入是从1开始
        ufs.unit(conn[0] - 1, conn[1] - 1);
        ret += conn[2];
    }
    return ufs.getCount() > 1 ? -1 : ret;
}





// 1584 medium 一堆点，问这些点组成的最小生成树的边长的和，边长是曼哈顿距离
struct edge {
    int x, y; // 点的两个坐标
    int len;
    edge(int _x, int _y, int _len) : x(_x), y(_y), len(_len) {}
    bool operator<(const edge& rhs) const {
        return len > rhs.len; // 小顶堆
    }
};
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    priority_queue<edge> pq;
    for (int i = 0; i < n-1; ++i) {  // 把所有的边存进小顶堆，共有n(n-1)/2条
        for (int j = i+1; j < n; ++j) {
            pq.emplace(edge{i, j, 
                abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1])});
        }
    }
    UFS ufs(n); // 利用并查集检测顶点是否之间被访问过，默认序号小的为头
    int ret = 0;
    while (!pq.empty()) {
        int curLen = pq.top().len, curx = pq.top().x, cury = pq.top().y; // 获得当前最短的边
        pq.pop();
        int rootx = ufs.find(curx);
        int rooty = ufs.find(cury);
        if (rootx != rooty) {  // 边是新边，需要加入当前的最小生成树
            ret += curLen;
            ufs.unit(rootx, rooty);
            if (ufs.cnt[rootx] == -n) break; // 集合已经包含了n个节点
        }
    }
    return ret;
}
// Prims解法
// 1 随机挑一个初始点当做初始点集合A
// 2 寻找边集中与集合A最近的点，加入点集，边集加入这条边
// 3 继续挑选与这个集合有连接的还没被访问的点……
// 在这道题中就是把步骤2改改，把所有未访问过的点放到小顶堆中，找到其中边最小的点
// 然后从中把
// 由于需要找到边集中与集合A最近的点，所以之前的边与每次找到的v的距离需要保留在堆中
// 这样有两种情况，即新的点到上一个点的距离是否位于堆顶
// 1如果位于，如果元素唯一，最简单，直接用；如果相等，要考虑是否被访问过
// 2不是，比如 ab-100 ac-1 bc-2
// c作为初始点，下一个点是a，然后把与a相连的点放入堆中发现，堆顶是bc-2，即是之前剩下的b
// 当然堆顶的点被访问过，自然不能用
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    priority_queue<pair<int,int>> pq;
    int ret = 0;
    int v = 0;  // initial point
    vector<bool> isVisited(n);
    for (int i = 1; i < n; ++i) {
        isVisited[v] = true;
        for (int j = 0; j < n; ++j) {
            if (!isVisited[j]) {
                int len = abs(points[j][0]-points[v][0]) + abs(points[j][1]-points[v][1]);
                pq.push({-len, j});  // 取负数是因为默认大顶堆
            }
        }
        while (isVisited[pq.top().second]) pq.pop();
        ret -= pq.top().first;
        v = pq.top().second;
        pq.pop();
    }
    return ret;
}
// 这种解法运用到了堆，因为会把所有边都统计，所以时间复杂度为n^2 * 2logn，前者为边数，后者为堆的运算一次的复杂度
// 由于会计算到全部边，所以改进的方法是用一个n维的数组去统计最短距离
// minDist[j]是当前节点j到点集A的最短距离
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    int ret = 0;
    vector<int> minDist(n, INT_MAX);
    int v = 0;  // initial point
    for (int i = 1; i < n; ++i) {
        minDist[v] = INT_MAX-1;  // 需要与之不同，因为需要让未访问过的和已经访问并确定值的区别开
        int adjCur = v;
        for (int j = 0; j < n; ++j) {
            if (minDist[j] != INT_MAX-1) {  // 依然是未被访问过的含义
                int len = abs(points[j][0]-points[v][0]) + abs(points[j][1]-points[v][1]);
                minDist[j] = min(minDist[j], len);  // 每次更新节点与集合A的最短距离
                adjCur = minDist[j] < minDist[adjCur] ? j : adjCur;  // 更新出所有点中与当前节点最近的那个
            }
        }
        ret += minDist[adjCur];
        v = adjCur;
    }
    return ret;
}


