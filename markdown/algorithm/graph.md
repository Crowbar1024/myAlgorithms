### 常用数据结构

```cpp
vector<vector<int>> grid; // 网格

vector<vector<int>> adg; // 邻接表

```

### 统计连通分量的方式
1. DFS + 网格
```cpp
int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size(), res = 0;
    vector<bool> isVisit(n, false);
    for (int i = 0; i < n; ++i) {   
        if (!isVisit[i]) {
            dfs(isConnected, isVisit, n, i);
            ++res;
        }
    }
    return res;
}
void dfs(vector<vector<int>>& isConnected, vector<bool>& isVisit, int n, int i) {
    for (int j = 0; j < n; ++j) { // 自j==i也会进入，然后那次的dfs会马上退出
        if (!isVisit[j] && isConnected[i][j]) {
            isVisit[j] = true;
            dfs(isConnected, isVisit, n, j);
        }
    }
}
```

2. DFS + 邻接表
```cpp
int findCircleNum(int n, vector<vector<int>>& adj) {
    vector<bool> isVisit(n, false);
    int res = 0;
    for (int i = 0; i < n; ++i) {
        if (!isVisit[i]) {
            dfs(n, adj, isVisit, i);
            ++res;
        }
    }
    return res;
}
void dfs(int n, vector<vector<int>>& adj, vector<bool>& isVisit, int i) {
    isVisit[i] = true;
    for (int j : adj[i]) {
        if (!isVisit[j]) dfs(n, adj, isVisit, j);
    }
}
```

常见的dfs顺序，判断写在前面，isvisit更新写在前面，这样可以将更新nx,ny和main里面的更新的写成一个。
```cpp
void dfs(int m, int n, vector<vector<char>>& board, int x, int y) {
    if (x < 0 || x > m-1 || y < 0 || y > n-1) return;
    if (board[x][y] != 'O') return; // A说明遍历过了，X不用遍历
    board[x][y] = 'A';
    for (int i = 0; i < 4; ++i) {
        int nx = x+dir[i], ny = y+dir[i+1];
        dfs(m, n, board, nx, ny);
    }
}
```



### 方向
二维数组压缩成一维
```cpp
const vector<int> dir{0,-1,0,1,0};  // 左上右下 逆时针
void dfs(int x, int y) {
    for (int i = 0; i < 4; ++i) {
        int nx = x+dir[i], ny = y+dir[i+1];
    }
    ...
}
```
反方向
```cpp
int j = (i+2)%4;  // 利用逆时针
```
二维数组保存位置信息和其他信息（高度，深度），不用自建结构，使用序列化
```cpp
q.push(height[i][j], i*n+j); // 根据列数生成一个序列化值
auto cur = q.top();
int nx = cur.second/n;
int ny = cur.second%n;
```