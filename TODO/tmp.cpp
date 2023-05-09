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