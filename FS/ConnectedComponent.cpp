#include "../myheader.h"

/*
    知识：
    1. DFS和BFS和并查集都可以处理连通图
    2. 当n个互相分立的连通分量需要变为连通图的时候，只需要添加n-1个路线，成为连通图

*/

// 547 medium 给了一个 n*n 的邻接矩阵（不是迷宫，不用上下左右走），求图中连通图的数量
int findCircleNum_DFS(vector<vector<int>>& isConnected) {
    int n = isConnected.size(), ret = 0;
    vector<bool> isVisit(n, false);
    for (int i = 0; i < n; ++i) {   
        if (!isVisit[i]) { // 不用遍历j，反正dfs的for会遍历。也不用初始化，因为一定会遍历到isConnected[i][j]自己
            dfs(isConnected, isVisit, n, i);
            ++ret;
        }
    }
    return ret;
}
void dfs(vector<vector<int>>& isConnected, vector<bool>& isVisit, int n, int i) {
    for (int j = 0; j < n; ++j) {
        if (!isVisit[j] && isConnected[i][j]) {
            isVisit[j] = true;
            dfs(isConnected, isVisit, n, j);
        }
    }
}
// BFS
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


// 130 medium 矩阵由若干字符 'X' 和 'O' ，找到所有被 'X' 围绕的区域，并将这些区域里所有的 'O' 用 'X' 填充。
// 注意边界上的 O 永远不会被 X 填充。因为边界之外没有X
// 所以这道题需要先从边界开始遍历，将靠近边的O的集合变成 A。然后遍历全局，把所有的0变成X，因为此时的O一定被X包围。
void solve(vector<vector<char>>& board) {
    int m = board.size(), n = board[0].size();
    if (m == 1 || n == 1) return;
    for (int i = 0; i < m; ++i) {
        dfs(m, n, board, i, 0); // 左边界
        dfs(m, n, board, i, n-1); // 右边界
    }
    for (int j = 0; j < n; ++j) {
        dfs(m, n, board, 0, j); // 上边界
        dfs(m, n, board, m-1, j); // 下边界
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] == 'A') board[i][j] = 'O'; // 恢复
            else if (board[i][j] == 'O') board[i][j] = 'X';
        }
    }
}
void dfs(int m, int n, vector<vector<char>>& board, int x, int y) {
    if (x < 0 || x > m-1 || y < 0 || y > n-1) return;
    if (board[x][y] != 'O') return; // A说明遍历过了，X不用遍历
    board[x][y] = 'A';
    for (int i = 0; i < 4; ++i) {
        int nx = x+dir[i], ny = y+dir[i+1];
        dfs(m, n, board, nx, ny);
    }
}

// 924 hard graph为邻接表，initial为污染的节点，污染会随连接扩散
// 现在可以将一个污染节点清理，但后续还可能会被污染，问清理哪里节点，可以使得最后正常的节点个数最多？
// 如果有多个，返回最小的那个。
// 显然，如果一个联通分量内有多个污染节点，无论清理哪个，最终都会被污染。
// 所以，只有当那些独占一个联通分量的污染节点，清理后才能全部恢复正常。
// 那么就转化成了怎么求数量最多的联通分量的问题
// DFS，染色
int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
    int n = graph.size(), color = 0;
    vector<int> colors(n, -1);
    for (int i = 0; i < n; ++i) {
        if (colors[i] == -1) paint(graph, i, colors, color++);
    }
    vector<int> colorCnt(color, 0); // 每种颜色的个数
    for (int i = 0; i < n; ++i) {
        colorCnt[colors[i]] += 1;
    }
    vector<int> colorInfectCnt(color, 0); // 每种颜色的感染节点个数
    for (int cur : initial) {
        colorInfectCnt[colors[cur]] += 1;
    }
    int maxCnt = 0, ret = n;
    for (int cur : initial) {
        int curColorInfectCnt = colorInfectCnt[colors[cur]]; // 当前颜色的感染节点个数
        if (curColorInfectCnt > 1) continue; // 不是独占
        int curColorCnt = colorCnt[colors[cur]]; // 当前节点的颜色数量
        if (curColorCnt > maxCnt) { // 此时是正数
            ret = cur;
            maxCnt = colorCnt[colors[ret]];
        } else if (curColorCnt == maxCnt && cur < ret) {
            ret = cur;
        }
    }
    if (ret == n) { // 这种情况就是所有的连通分量里面的污染节点都超过1
        ret = *min_element(initial.begin(), initial.end());
    }
    return ret;
}
void paint(vector<vector<int>>& graph, int cur, vector<int>& colors, int color) {
    colors[cur] = color;
    // 需要注意，这里不能填cur+1，因为这是递归，0->3 3->2，如果3从4开始，就G了
    for (int next = 0; next < graph.size(); ++next) {
        if (graph[cur][next] == 1 && colors[next] == -1) paint(graph, next, colors, color);
    }
}