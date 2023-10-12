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
int findMinLength(int n, int m, vector<pair<int,int>> p1, vector<pair<int,int>> p2, vector<int> k) {
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


// 网易校招 一个迷宫，'#'表示障碍物，给了起点和终点，返回通关的最短路径
int mazePath(vector<string>& maze, pair<int,int> src, pair<int,int> dst) {
    int rowNum = maze.size();
    int colNum = maze[0].size();
    const int INF = 0x3f3f3f3f;
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};  // 上右下左，逆时针
    vector<vector<int>> dist(rowNum, vector<int>(colNum, 0x3f3f3f3f));
    queue<pair<int,int>> q;
	q.push(src);
	dist[src.first][src.second] = 0;
	while (q.size()) {
		auto cur = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int nexti = cur.first + dir[i][0];
			int nextj = cur.second + dir[i][1];
            // {nexti,nextj}没有走到过
			if (nexti >= 0 && nexti < rowNum && nextj >= 0 && nextj < colNum && maze[nexti][nextj] != '#' && dist[nexti][nextj] == INF) {
				dist[nexti][nextj] = dist[cur.first][cur.second] + 1;
                if (pair<int,int>{nexti,nextj} == dst) break;
				q.push({nexti, nextj});
			}
		}
	}
	return dist[dst.first][dst.second];
}

// 1926 medium 找到边界的出口的最短路径，起点若在边界上不算
int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
    int row = maze.size(), col = maze[0].size();
    const int INF = 0x3f3f3f3f;
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};  // 上右下左，逆时针
    vector<vector<int>> dist(row, vector<int>(col, INF));
    queue<pair<int,int>> q;
	q.push({entrance[0], entrance[1]});
	dist[entrance[0]][entrance[1]] = 0;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int nexti = cur.first + dir[i][0];
			int nextj = cur.second + dir[i][1];
			if (nexti >= 0 && nexti < row && nextj >= 0 && nextj < col && maze[nexti][nextj] != '+' && dist[nexti][nextj] == INF) {
				dist[nexti][nextj] = dist[cur.first][cur.second] + 1;
                if (nexti == 0 || nexti == row-1 || nextj == 0 || nextj == col-1) {
                    return dist[nexti][nextj];
                }
				q.push({nexti, nextj});
			}
		}
	}
	return -1;
}

// 994 medium 网格中 0 代表空单元格； 1 代表新鲜橘子； 2 代表腐烂的橘子。
// 每分钟，腐烂的橘子周围 4 个方向上相邻的新鲜橘子都会腐烂。
// 返回直到单元格中没有新鲜橘子为止所必须经过的分钟数。如果不可能全部腐烂，返回 -1 。
// 把一开始腐烂的橘子都加入队列
int orangesRotting(vector<vector<int>>& grid) {
    int row = grid.size(), col = grid[0].size();
    const int INF = 0x3f3f3f3f;
    const vector<vector<int>> dir{{-1,0},{0,1},{1,0},{0,-1}};  // 上右下左，逆时针
    vector<vector<int>> dist(row, vector<int>(col, INF));
    queue<pair<int,int>> q;
    int cnt = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (grid[i][j] == 2) {
                q.push({i,j});
                dist[i][j] = 0;
            } else if (grid[i][j] == 1) {
                cnt += 1;
            }
        }
    }
    if (cnt == 0) return 0;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ni = cur.first + dir[i][0];
			int nj = cur.second + dir[i][1];
            if (ni < 0 || ni >= row || nj < 0 || nj >= col) continue;
            if (grid[ni][nj] == 1) {
                grid[ni][nj] = 2;
                dist[ni][nj] = dist[cur.first][cur.second] + 1;
                cnt -= 1;
                if (cnt == 0) return dist[ni][nj];
                q.push({ni,nj});
            }
		}
	}
	return -1;
}


// 美团 21/5/9 笔试4
// 一个n*m的迷宫，*为墙壁，一个机器人只有一发子弹可以破坏墙壁，想从左上角走到右下角，问最短路径
// 和bfs的21/4/18 腾讯笔试5有点类似，都是BFS的搜索过程
// 不同之处在于，打地鼠没有墙，所以在for里面走时除了越界的判断，只需要判断在当前位置判断是否能走到终点（不需要额外信息）
// 但这道题不是，它没有截至时间，如果在state里面写了dis信息，需要有一个储存了nextx和nexty作为下标的dis作为对比
// 即判断下一个位置是否在之前走过，如果走过，那么通过当前策略的dis是否比之前的要好
// 这样就需要一个额外的三维数组去存储位置信息和子弹状态作为下标的已走距离
// 继续为什么打地鼠需不需要额外的DP数组，其实是可以的，只需要将state信息限制到3个，坐标和上一次方向，
// 然后再保存坐标和时间的DP数组储存分数，这样下一个状态如果分数没有预想的高，可以跳过这个，感觉这种方法更快
struct state {
    int gun;
    int x;
    int y;
};
int robotGun(vector<string>& maze) {
    int n = maze.size(), m = maze[0].size();
    const int maxn = n*m;
    queue<state> q;
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(m, vector<int>(2, maxn)));
    dp[0][0][0] = dp[0][0][1] = 0;  // 初始位置有没有开枪都走了0距离
    q.push(state{1,0,0});
    while (!q.empty()) {
        state s = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nextx = s.x+dir[i];
            int nexty = s.y+dir[i+1];
            if (nextx<n && nextx>=0 && nexty<m && nexty>=0) {
                if (maze[nextx][nexty]=='*' && s.gun && dp[nextx][nexty][0]>dp[s.x][s.y][1]+1) {
                    dp[nextx][nexty][0] = dp[s.x][s.y][1]+1;
                    q.push(state{0, nextx, nexty});
                } else if (maze[nextx][nexty]=='.' && dp[nextx][nexty][s.gun]>dp[s.x][s.y][s.gun]+1) {
                    dp[nextx][nexty][s.gun] = dp[s.x][s.y][s.gun]+1;
                    q.push(state{s.gun, nextx, nexty});
                }
            }
        }
    }
    int ret = min(dp[n-1][m-1][0], dp[n-1][m-1][1]);
    return ret==maxn ? -1 : ret;
}



// 21/4/18 腾讯笔试5
// 有一个 n * m 个格子的矩阵mp时不时会冒出一些地鼠。mp[i][j] 表示每过 mp[i][j] 的时间第 i 行第 j 列的格子会冒出一只地鼠。
// 初始时，时间为0，你在第1行第1列格子处，每秒你必须向上下左右四个方向移动一格，不能走出矩阵外。
// 作为一个合格的打地鼠玩家，如果这一秒你从格子 a 走到格子 b ，下一秒你不能从格子 b 走到格子 a 。
// 如果你走到一个格子的时候，这里刚好冒出一只地鼠，你就可以打一下地鼠。
// 你的终点是第n行第m列的格子，给你 t 秒时间，问你最多打几下地鼠。
// 最后你必须到达终点，如果第t秒时你不在终点，算作打了0次地鼠。
// 2<=m,n<=10, 1<=t, mp[i][j]<=1000
// 例子：
// 2 2 6
// 1 1
// 1 1
// 一种行走方案是 (1, 1) -> (1, 2) -> (2, 2) -> (2, 1) -> (1, 1) -> (1, 2) -> (2, 2) ，每步都可以打一下地鼠。
// 所以是6
// 看上去是个四维动态规划问题，时间、方向、地图横坐标和纵坐标各为一维。直接暴力遍历四维数组有些样例通不过
// 本质上是找到一条路径使奖励最大，那么BFS似乎可行，通过维护更新一个玩家状态的队列的方式来求得结果。
// 优化方法见bfs的美团 21/5/9 笔试4
struct State {
    int row;
    int col;
    int time;
    int score;  // t%mp[i][j]==0 表示当前是否可以获得这个点的奖励，隐藏了bool到int的转换
    int direction;  // 上一次到达的方向，(0-1,1-0,2-3,3-2)是错误方向
    State(int _row, int _col, int _time, int _score, int _direction) : 
        row(_row), col(_col), time(_time), score(_score), direction(_direction) {}
};
int WhacAMole(vector<vector<int>> mp, int t) {
    int n = mp.size(), m = mp[0].size();
    queue<State> q;
    int ret = 0;
    q.emplace(1,0,1,(1%mp[1][0])==0,0);  // 先向下走一格
    q.emplace(0,1,1,(1%mp[0][1])==0,1);  // 向右
    while (!q.empty()) {
        State cur = q.front();
        q.pop();
        if (cur.row == n-1 && cur.col == m-1 && cur.time == t) {  // 比较不同路径到达终点的分数
            ret = max(ret, cur.score);
        }
        if (cur.time == t) continue;  // 要么没到终点，出队，要么到了，比较完了，也出队
        for (int i = 0; i < 4; i++) {
            int nextRow = cur.row + dir[i];
            int nextCol = cur.col + dir[i+1];
            int falseDir = 2*(cur.direction>1) + !(cur.direction&1);  // 错误的路径
            if (i != falseDir && nextRow >= 0 && nextRow < n && nextCol >= 0 && nextCol < m 
                && t-cur.time >= n-1-nextRow+m-1-nextCol) {
                q.emplace(nextRow, nextCol, cur.time+1, cur.score+((cur.time%mp[nextRow][nextCol])==0),i);
            }
        }
    }
    return ret;
}


