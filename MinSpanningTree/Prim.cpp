#include "myheader.h"

/*
    
    Prim 算法流程
    1 选任一节点D作为起始点，顶点A、B、E和F通过单条边与D相连。A是距离D最近的顶点，因此将A加入最小生成树
    2 寻找下一个距离D或A最近的顶点。将该顶点加入最小生成树。
    3 重复该过程，直到全部加入进来
*/



// 1135 medium 最小生成树板子题 [1:n]城市，返回连接所有城市的最低成本。如果无法连接所有 n 个城市，返回 -1。
int minimumCost(int n, vector<vector<int>>& connections) {
    if (connections.size() < n - 1) return -1; // 最小生成树的边数为n-1
    vector<vector<pair<int,int>>> adjVal(n + 1); // 下标从1开始， adj[i][j] = {j,cost}
    for (vector<int>& conn : connections) {
        adjVal[conn[0]].emplace_back(conn[1], conn[2]);
        adjVal[conn[1]].emplace_back(conn[0], conn[2]);
    }
    if (adjVal[1].empty()) return -1; // 把1作为起始点，若1本身就是一个孤岛，说明无法联通
    auto cmp = [](pair<int,int>& lhs, pair<int,int>& rhs) {
        return lhs.second > rhs.second;
    };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> minHeap(cmp);
    for (pair<int,int>& nextPoint : adjVal[1]) { // 把起始点的相邻点存入小顶堆
        minHeap.emplace(nextPoint);
    }
    vector<int> vis(n + 1, 0); // 下标从1开始
    vis[1] = 1; // 起始点也算
    int ret = 0, visCnt = 1;
    while (!minHeap.empty()) {
        auto [closestPoint, minlen] = minHeap.top(); // 得到与当前最小生成树最近的点
        minHeap.pop();
        if (vis[closestPoint]) continue;
        ret += minlen;
        ++visCnt;
        if (visCnt == n) return ret;
        vis[closestPoint] = 1;
        for (pair<int,int>& nextPoint : adjVal[closestPoint]) {
            minHeap.emplace(nextPoint);
        }
    }
    return -1;
}