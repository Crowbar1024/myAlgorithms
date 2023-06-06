#include "myheader.h"




// 51 hard N皇后 N×N 的棋盘，让你放置 N 个皇后，使得它们不能互相攻击
// 皇后可以攻击同一行、同一列、左上左下右上右下四个方向的任意单位
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> ret;
        vector<string> snap(n, string(n,'.'));
        dfs(snap, n, 0, ret); // 从第0行开始dfs
        return ret;
    }
    void dfs(vector<string>& snap, int n, int pos, vector<vector<string>>& ret) {
        if (pos == n) {
            ret.emplace_back(snap);
            return;
        }
        for (int i = 0; i < n; i++) { // 每行挑选一个试试
            if (isValid(pos, i, n, snap)) {
                snap[pos][i] = 'Q';
                dfs(snap, n, pos+1, ret); // 进入下一行
                snap[pos][i] = '.';
            }
        }
    }
    bool isValid(int row, int col, int n, vector<string>& snap) {
        for (int i = 0; i < row; i++) { // 列遍历，不需要行遍历，因为每次在第pos行里选择一个
            if (snap[i][col] == 'Q') return false;
        }
        for (int i = row-1, j = col+1; i >= 0 && j < n; i--, j++) { // 右上方向遍历（左下还没更新）
            if (snap[i][j] == 'Q') return false;
        }
        for (int i = row-1, j = col-1; i >= 0 && j >= 0; i--, j--) { // 左上方向遍历
            if (snap[i][j] == 'Q') return false;
        }
        return true;
    }
};



// 37 hard 解数独 保证只有唯一解
// 如果有唯一解的话，仿效8皇后void dfs是不对的，因为每次都会更新board，错的会把对的覆盖掉
// 所以有一些小技巧，比如改成bool dfs，只要{0,0}位置某个值是对的，那么直接结束。
class Solution {
public:
    void solveSudoku(vector<vector<char>>& board) {
       dfs(board);
    }
    bool dfs(vector<vector<char>>& board) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != '.') continue; // 之前该位置已经遍历过
                for (char k = '1'; k <= '9'; ++k) { // 每个值都试试
                    if (isValid(board, i, j, k)) {
                        board[i][j] = k;
                        if (dfs(board)) return true; // 全部遍历过一遍
                        board[i][j] = '.';
                    }
                }
                return false; // 如果该位置填啥都是错的，说明之前的分配就是错的
            }
        }
        return true; // 全部都填满了会运行到这儿
    }
    bool isValid(vector<vector<char>>& board, int row, int col, char val) {
        for (int i = 0; i < 9; ++i) { // 列遍历
            if (board[i][col] == val) return false;
        }
        for (int j = 0; j < 9; ++j) { // 行遍历
            if (board[row][j] == val) return false;
        }
        int r = (row/3)*3, c = (col/3)*3;
        for (int i = r; i < r+3; ++i) { // 九宫格遍历
            for (int j = c; j < c+3; ++j) {
                if (board[i][j] == val) return false;
            }
        }
        return true;
    }
};
// 如果继续void dfs，那么需要用一个标志位记录是否是第一次更新完
class Solution {
private:
    bool line[9][9]; // line[i][j]：数字j在第i行已经出现过
    bool column[9][9];
    bool block[3][3][9];
    bool valid;
    vector<pair<int, int>> spaces;
public:
    void dfs(vector<vector<char>>& board, int pos) {
        if (pos == spaces.size()) { // 之前记录的没有填写的位置都填满了
            valid = true; // 避免找到正确答案后又开始dfs
            return;
        }
        auto [i, j] = spaces[pos];
        for (int digit = 0; digit < 9 && !valid; ++digit) { // 如果更新过程中valid更新了，终止
            if (!line[i][digit] && !column[j][digit] && !block[i / 3][j / 3][digit]) {
                line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = true;
                board[i][j] = digit + '0' + 1;
                dfs(board, pos + 1);
                line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = false;
            }
        }
    }
    void solveSudoku(vector<vector<char>>& board) {
        memset(line, false, sizeof(line));
        memset(column, false, sizeof(column));
        memset(block, false, sizeof(block));
        valid = false;
        for (int i = 0; i < 9; ++i) { // 初始化有数字和需要填的位置
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') {
                    spaces.emplace_back(i, j);
                } else {
                    int digit = board[i][j] - '0' - 1; // 下标从0开始
                    line[i][digit] = column[j][digit] = block[i / 3][j / 3][digit] = true;
                }
            }
        }
        dfs(board, 0); // 开始从第一个没有填写数字的位置开始dfs
    }
};
/*
way 2 
1. 一维压缩
首先确定下标，都是从0开始，出现j表示出现'0'+j+1。
line[i][j]=1，第i行出现j。优化为line[i]=1<<j，第i行的数的第j位为1。j最大为8，所以最大数是0x1ff
line[i][digit] = true;
改成
line[i] ^= (1 << digit); (flip，^是为了恢复false)
2. 寻找0
(line[i] | column[j] | block[i / 3][j / 3])中第k位如果为1，表示k已经出现过了。
现在要找到第pos个需要填的位置上其他可以用的数，所以需要先取反，让其他没有出现过的数为1。因为符号位的影响，需要& 0x1ff
所以现在得到了该位置上需要遍历的其他数的集合mask
int mask = ~(line[i] | column[j] | block[i / 3][j / 3]) & 0x1ff;
3. 遍历集合
即每次从集合的末尾读一个1
for (; mask && !valid; mask &= (mask - 1)) { // 删除最后一个1
    int digitMask = mask & (-mask); // 最后一个1
    int digit = __builtin_ctz(digitMask); // 最后一个1的尾0数，相当于digit
    flip(i, j, digit);
    board[i][j] = digit + '0' + 1;
    dfs(board, pos + 1);
    flip(i, j, digit);
}
*/
// way 3 在初始化时先确定几个好判断的位置，先填进去确定的数字
class Solution {
private:
    int line[9];
    int column[9];
    int block[3][3];
    bool valid;
    vector<pair<int, int>> spaces;
public:
    void flip(int i, int j, int digit) {
        line[i] ^= (1 << digit);
        column[j] ^= (1 << digit);
        block[i / 3][j / 3] ^= (1 << digit);
    }
    void dfs(vector<vector<char>>& board, int pos) {
        if (pos == spaces.size()) {
            valid = true;
            return;
        }
        auto [i, j] = spaces[pos];
        int mask = ~(line[i] | column[j] | block[i / 3][j / 3]) & 0x1ff;
        for (; mask && !valid; mask &= (mask - 1)) {
            int digitMask = mask & (-mask);
            int digit = __builtin_ctz(digitMask);
            flip(i, j, digit);
            board[i][j] = digit + '0' + 1;
            dfs(board, pos + 1);
            flip(i, j, digit);
        }
    }
    void solveSudoku(vector<vector<char>>& board) {
        memset(line, 0, sizeof(line));
        memset(column, 0, sizeof(column));
        memset(block, 0, sizeof(block));
        valid = false;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != '.') {
                    int digit = board[i][j] - '0' - 1;
                    flip(i, j, digit);
                }
            }
        }
        while (true) { // 没有也没关系
            int modified = false;
            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 9; ++j) {
                    if (board[i][j] == '.') {
                        int mask = ~(line[i] | column[j] | block[i / 3][j / 3]) & 0x1ff;
                        if (!(mask & (mask - 1))) {
                            int digit = __builtin_ctz(mask);
                            flip(i, j, digit);
                            board[i][j] = digit + '0' + 1;
                            modified = true;
                        }
                    }
                }
            }
            if (!modified) {
                break;
            }
        }
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') {
                    spaces.emplace_back(i, j);
                }
            }
        }
        dfs(board, 0);
    }
};
// 36 medium 判断一个数独是否有效
// 37的way2优化一下就行
bool isValidSudoku(vector<vector<char>>& board) {
    vector<int> line(9, 0);
    vector<int> column(9, 0);
    vector<vector<int>> block(3, vector<int>(3, 0));
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == '.') continue;
            int digit = board[i][j] - '1';
            int bitPos = 1 << digit;
            if ((line[i] & bitPos) || (column[j] & bitPos) || (block[i/3][j/3] & bitPos)) return false;
            line[i] |= bitPos; // 不用恢复
            column[j] |= bitPos;
            block[i/3][j/3] |= bitPos;
        }
    }
    return true;
}

// 332 hard 一组pair，每组类似["JFK", "LGA"]，是src和dst的关系，出发点是"JFK"
// 存在多条路径，但取字典序最小的那条（就是dst小的那个）
// 如果用map<string,string>来统计路径，无法回避某个key对应多个value的情况
// 所以增加一个维度，使用map<string,map<string,int>>，最后一个int是到dst的次数
class Solution {
public:
    unordered_map<string, map<string, int>> myMap;  // 要求dst有序
    vector<string> ret;
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        for (auto &ticket : tickets) {
            myMap[ticket[0]][ticket[1]] += 1;  // 到dst的路径+1
        }
        ret.emplace_back("JFK");
        dfs(tickets);
        return ret;
    }
    // 由于最终只有一条路径是对的，没有必要遍历完，所以用bool保证中途是对的话，直接不跑了
    bool dfs(vector<vector<string>>& tickets) {
        if (ret.size() == tickets.size()+1) return true;
        // 遍历res末尾元素可以到达的dst
        // key不能改，而且是引用。没有const就不能引用，得是复制
        for (pair<const string, int> &dst : myMap[ret.back()]) {
        // for (auto &dst : myMap[ret.back()]) {  // 也行
            if (dst.second) {
                ret.emplace_back(dst.first);
                dst.second -= 1;
                if (dfs(tickets)) return true;
                dst.second += 1;
                ret.pop_back();
            }
        }
        return false;  // 中间发小找不到dst的会跑到这一步
    }
};