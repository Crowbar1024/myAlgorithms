#include "myheader.h"

// 2596 medium n*n棋盘，从{0,0}出发的棋子马，可以走8个方向，访问棋盘上的每个格子恰好一次
// 棋盘中的数字就是走的序号[0,n*n)，判断序号是否正确
vector<vector<int>> horseDir{{-2,-1},{-2,1},{2,-1},{2,1},{1,2},{-1,2},{1,-2},{-1,-2}};
bool checkValidGrid(vector<vector<int>>& grid) {
    if (grid[0][0] != 0) return false; // 左上角是出发点
    int n = grid.size();
    return dfs(grid, 0, 0, n);
}
bool dfs(vector<vector<int>>& grid, int x, int y, int n) {
    if (grid[x][y] == n*n-1) return true;
    for (int i = 0; i < 8; ++i) {
        int nx = x+horseDir[i][0], ny = y+horseDir[i][1];
        if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
        if (grid[nx][ny] == grid[x][y]+1) return dfs(grid, nx, ny, n);
    }
    return false;
}

// 美团笔试21/5/9 1
// 一个n*m（都<500）的迷宫，给出若干条路径p1{x,y}->p2{u,v}（x<=u,y<=v），距离为k
// 问从(0,0)到右下角最短的路径长度是多少？
// 这道题先入为主使用dijkstra这种算法，但一思考，总结点数为25000，On^2算法肯定挂
// 一看条件，其实简单的DFS就可以解决，因为根据条件，可以理解为一棵树向右下角生成
// 给每个点设置一个值，简单起见，就是从左到右，从上到下第几个
int findMinLength(int n, int m, vector<pair<int,int>> p1, vector<pair<int,int>> p2, vector<int> k, int n, int m) {
    int t = n*m, q = p1.size();
    vector<vector<int>> path(t), pathDis(t);
    for (int i = 0; i < q; ++i) {
        int a1 = (p1[i].first-1)*m + p1[i].second-1;  // 第1点的值
        int a2 = (p2[i].first-1)*m + p2[i].second-1;  // 第2点的值
        path[a1].push_back(a2);  // 说明a1 和 a2相邻
        pathDis[a1].push_back(k[i]);  // a1和a2的距离
    }
    vector<int> dis(t, INF);
    dis[0] = 0;
    dfs(path, pathDis, 0, dis);
    return dis[t-1]==INF ? -1 : dis[t-1];
}
void dfs(vector<vector<int>>& path, vector<vector<int>>& pathDis, int pos, vector<int>& dis) {
    for (int i = 0; i < path[pos].size(); ++i) {
        int x = path[pos][i];
        dis[x] = min(dis[x], dis[pos]+pathDis[pos][i]);
        dfs(path, pathDis, x, dis);
    }
}


// HJ43 走迷宫，0可以走，1不能走，左上角起始点，右下角终点，有且只有一条路径，返回该路径
// 可以用maze本身的设置节省一个used数组。
// 因为只有一条，而DFS会全部遍历完，所以用BFS+记录路径方向的做法，在main里面直接退出
class Solution {
public:
    int row, col;
    vector<vector<int>> maze, preDir; // 记录方向
    vector<pair<int,int>> ret;
    int main() {
        // freopen("input.txt","r",stdin);
        while (cin >> row >> col) {
            maze = vector<vector<int>>(row, vector<int>(col, 0));
            preDir = vector<vector<int>>(row, vector<int>(col, 0));
            ret.clear();
            for (int i = 0; i < row; ++i) {
                for (int j = 0; j < col; ++j) {
                    cin >> maze[i][j];
                }
            }
            queue<pair<int,int>> q;
            q.emplace(0,0);
            maze[0][0] = 1;
            preDir[0][0] = -1;
            while (!q.empty()) {
                auto cur = q.front(); q.pop();
                if (cur.first == row-1 && cur.second == col-1) break;
                for (int i = 0; i < 4; ++i) {
                    int nx = cur.first + dir[i], ny = cur.second + dir[i+1];
                    if (nx < 0 || nx >= row || ny < 0 || ny >= col) continue;
                    if (maze[nx][ny] == 0) {
                        q.emplace(nx,ny);
                        preDir[nx][ny] = i;
                        maze[nx][ny] = 1; // 扩散的写法，不会回退
                    }
                }
            }
            int x = row-1, y= col-1;
            while (preDir[x][y] != -1) {
                ret.emplace_back(x,y);
                int i = (preDir[x][y]+2)%4;  // 反方向，走回去
                x += dir[i];
                y += dir[i+1];
            }
            ret.emplace_back(0,0);
            for (int i = ret.size()-1; i >= 0; --i) {
                printf("(%d,%d)\n", ret[i].first, ret[i].second);
            }
        }
        return 0;
    }
};


// 329 hard 矩阵中的最长严格递增路径 等价于滑雪，不能斜着滑
// 此时dfs需要是int返回值
class Solution {
public:
    int m, n;
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> dist(m, vector<int>(n, 0));  // 默认为0，这样如果为1，说明该店周围都大于等于它
        int ret = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                ret = max(ret, dfs(matrix, dist, i, j));
            }
        }
        return ret;
    }
    int dfs(vector<vector<int>>& matrix, vector<vector<int>>& dist, int x, int y) {
        if (dist[x][y] > 0) return dist[x][y]; // 体现了初始化为0的价值
        for (int i = 0; i < 4; i++) {
            int nx = x+dir[i], ny = y+dir[i+1];
            if (nx<0 || nx>=m || ny<0 || ny>=n) continue;
            if (matrix[nx][ny] > matrix[x][y]) {
                dist[x][y] = max(dist[x][y], dfs(matrix, dist, nx, ny)+1);  // 四个不同方向取最大
            }
        }
        if (dist[x][y] == 0) return dist[x][y]=1; // 四个方向都走不了，设置1
        return dist[x][y];
    }
};