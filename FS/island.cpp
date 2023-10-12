#include "myheader.h"

// 200 medium 岛屿数量 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
// BFS
int numIslands(vector<vector<char>>& grid) {
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};
    int m = grid.size(), n = grid[0].size();
    int ret = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '1') {
                queue<pair<int,int>> q;
                q.emplace(i,j);
                grid[i][j] = '0';
                while (!q.empty()) {
                    auto cur = q.front(); q.pop();
                    for (int i = 0; i < 4; ++i) {
                        int nx = cur.first+dir[i][0], ny = cur.second+dir[i][1];
                        if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                        if (grid[nx][ny] == '1') {
                            q.emplace(nx,ny);
                            grid[nx][ny] = '0'; // 使用消元法减少used的使用。
                        }
                    }
                }
                ret += 1; // 一个联通分量遍历完了
            }
        }
    }
    return ret;
}
// DFS一样可以做
class Solution {
public:
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};
    int m, n;
    int numIslands(vector<vector<char>>& grid) {
        m = grid.size(), n = grid[0].size();
        int ret = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == '1') {
                    grid[i][j] = '0';
                    dfs(grid, i, j);
                    ret += 1;
                }
            }
        }
        return ret;
    }
    void dfs(vector<vector<char>>& grid, int x, int y) {
        for (int i = 0; i < 4; ++i) {
            int nx = x+dir[i][0], ny = y+dir[i][1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (grid[nx][ny] == '1') {
                grid[nx][ny] = '0';
                dfs(grid, nx, ny);
            }
        }
    }
};


// 695 medium 最大岛屿面积 连通图的最大图的节点数
// 此时dfs需要返回值，当然也可以继续void，使用两个snap和res变量记录即可，这里试试有返回值的写法
class Solution {
public:
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};
    int m, n;
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        m = grid.size(), n = grid[0].size();
        int ret = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) {
                    grid[i][j] = 0;
                    ret = max(ret, dfs(grid, i, j));
                }
            }
        }
        return ret;
    }
    int dfs(vector<vector<int>>& grid, int x, int y) {
        int snap = 1;
        for (int i = 0; i < 4; ++i) {
            int nx = x+dir[i][0], ny = y+dir[i][1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (grid[nx][ny] == 1) {
                grid[nx][ny] = 0;
                snap += dfs(grid, nx, ny);
            }
        }
        return snap;
    }
};
// 栈循环写法
int maxAreaOfIsland(vector<vector<int>>& grid) {
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};
    int m = grid.size(), n = grid[0].size();
    int ret = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1) {
                grid[i][j] = 0;
                int snap = 1;
                stack<pair<int,int>> st;
                st.emplace(i,j);
                while (!st.empty()) {
                    auto cur = st.top(); st.pop();
                    for (int i = 0; i < 4; ++i) {
                        int nx = cur.first+dir[i][0], ny = cur.second+dir[i][1];
                        if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                        if (grid[nx][ny] == 1) {
                            grid[nx][ny] = 0;
                            snap += 1;
                            st.emplace(nx,ny);
                        }
                    }
                }
                ret = max(ret, snap);
            }
        }
    }
    return ret;
}

// 827 hard 最大人工岛 grid是n*n。最多只能将一格 0 变成 1。返回执行此操作后，grid 中最大的岛屿面积是多少？
// grid经过一次DFS后把每个连通图变成序号（不能是面积，因为这样可能会出现自己连自己的情况）。并用一个额外数组记录每个序号对应的岛屿面积
// 经过第二次DFS把海洋格子遍历
class Solution {
public:
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};
    int m, n;
    int cnt;  // 序号
    int largestIsland(vector<vector<int>>& grid) {
        m = grid.size(), n = grid[0].size();
        cnt = 0;
        vector<vector<int>> seqv(m, vector<int>(n, 0));  // 序号数组
        vector<int> sqv;  // 每个岛屿面积
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) {
                    grid[i][j] = 0;
                    cnt += 1;
                    seqv[i][j] = cnt;
                    sqv.emplace_back(dfs(grid, seqv, i, j));
                }
            }
        }
        if (sqv.empty()) return 1;  // 没有岛屿
        int ret = *max_element(sqv.begin(), sqv.end());
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (seqv[i][j] == 0) {  // 海水
                    int tmp = 1;
                    unordered_set<int> used;
                    for (int k = 0; k < 4; ++k) {
                        int ni = i+dir[k][0], nj = j+dir[k][1];
                        if (ni < 0 || ni >= m || nj < 0 || nj >= n) continue;
                        if (seqv[ni][nj] == 0) continue;  // 该方向没有岛屿
                        int seq = seqv[ni][nj];
                        if (used.count(seq)) continue;
                        used.insert(seq);
                        tmp += sqv[seq-1];  // 从1开始
                    }
                    ret = max(ret, tmp);
                }
            }
        }
        return ret;
    }
    int dfs(vector<vector<int>>& grid, vector<vector<int>>& seqv, int x, int y) {
        int sq = 1;
        for (int i = 0; i < 4; ++i) {
            int nx = x+dir[i][0], ny = y+dir[i][1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (grid[nx][ny] == 1) {
                grid[nx][ny] = 0;
                seqv[nx][ny] = cnt;
                sq += dfs(grid, seqv, nx, ny);
            }
        }
        return sq;
    }
};

// 463 easy 岛屿周长 只有一个岛屿，计算其周长
// 若为1的格子旁边有n个0，那么其周长为n
// 或者靠近n个边界，周长+n
// 所以只要统计出界和临近海水数量就行
class Solution {
public:
    const vector<int> dir{0,-1,0,1,0};
    int m, n;
    int islandPerimeter(vector<vector<int>>& grid) {
        m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] == 1) {
                    grid[i][j] = 2;
                    return dfs(grid, i, j);
                }
            }
        }
        return 0;  // 没有岛屿
    }
    int dfs(vector<vector<int>>& grid, int x, int y) {
        int ret = 0;
        for (int i = 0; i < 4; ++i) {
            int nx = x+dir[i], ny = y+dir[i+1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) {
                ret += 1;
            } else if (grid[nx][ny] == 0) {
                ret += 1;  // 同一格海水可以重复计算
            } else if (grid[nx][ny] == 1) {
                grid[nx][ny] = 2;  // 避免陆地一直回溯
                ret += dfs(grid, nx, ny);
            }
        }
        return ret;
    }
};

// 934 medium 两座岛，都是1，求岛间的最短距离
// 先要用DFS把一个岛变成2，然后将所有的2加入一个队列，BFS每次取出当前的全部节点，将临近的0变成-1
// 向外扩散，类似树的层次遍历
int shortestBridge(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    queue<pair<int,int>> que;
    int jud = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == 1) {
                grid[i][j] = 2;
                que.emplace(i, j);
                dfs(grid, m, n, i, j, que);
                jud = 1;
                break; // 一个连通分量就够了
            }
        }
        if (jud == 1) break;
    }
    int ret = 0; // 圈扩大的次数
    while (!que.empty()) {
        int cnt = que.size();
        while (cnt--) {
            auto cur = que.front(); // 这里不能写引用，不然会导致heap-use-after-free
            // 其实很明显，我之后直接pop了，有些编译器会把cur直接给free掉，所以找不到了
            que.pop();
            for (int i = 0; i < 4; ++i) {
                int nx = cur.first + dir[i], ny = cur.second + dir[i+1];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n || grid[nx][ny] == 2) {
                    continue;
                } else if (grid[nx][ny] == 0) {
                    grid[nx][ny] = -1;
                    que.emplace(nx, ny);
                } else if (grid[nx][ny] == 1) {
                    return ret;
                }
            }
        }
        ret += 1; // 一圈完成了
    }
    return -1;
}
void dfs(vector<vector<int>>& grid, int m, int n, int x, int y, queue<pair<int,int>>& que) {
    for (int i = 0; i < 4; ++i) {
        int nx = x + dir[i], ny = y + dir[i+1];
        if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
        if (grid[nx][ny] == 1) {
            grid[nx][ny] = 2;
            que.emplace(nx, ny);
            dfs(grid, m, n, nx, ny, que);
        }
    }
}



// PAT 1091 给定一个三维数组box，0表示正常1表示有肿瘤，肿瘤块的大小大于等于t才算作是肿瘤，求肿瘤的总体积
// 三维连通图
class BattleOverCities {
private:
    const vector<vector<int>> dir{{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-1,0},{0,0,-1}};
    struct Node {
        int x, y, z;
    };
    vector<vector<vector<int>>> box;
    int n,m,l;  // 长宽高
    int t;
    int tNum, tSize;
    void init(vector<vector<vector<int>>>& organ, int tumourSize) {
        t = tumourSize;
        n = organ.size();
        m = organ[0].size();
        l = organ[0][0].size();
        box = organ;
        tNum = tSize = 0;
    }
    void bfs(int x, int y, int z) {
        int cnt = 1;
        queue<Node> q;
        box[x][y][z] = 0; // 消元法
        q.push({x,y,z});
        while (!q.empty()) {
            auto tmp = q.front();
            for (int i = 0; i<6; i++) {
                int nx = tmp.x + dir[i][0];
                int ny = tmp.y + dir[i][1];
                int nz = tmp.z + dir[i][2];
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && nz >= 0 && nz < l && box[nx][ny][nz]) {
                    cnt++;
                    box[nx][ny][nz] = 0;
                    q.push({nx, ny, nz});
                }
            }
            q.pop();
        }
        if (cnt >= t) tSize += cnt;
    }
public:
    vector<int> getTumourSize(vector<vector<vector<int>>>& organ, int tumourSize) {
        init(organ, tumourSize);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < l; k++) {
                    if (box[i][j][k]) {
                        bfs(i,j,k);
                        tNum++;
                    }
                }
            }
        }
        return vector<int>{tNum, tSize};
    }
};