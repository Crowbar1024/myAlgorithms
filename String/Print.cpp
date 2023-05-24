#include "myheader.h"


// 给出一个矩阵（也可能是向量）的zigzag输出
// 即输出a[0,0] a[1,0] a[0,1] a[0,2] a[1,1]...
// 核心思想是确定对角线的两个顶点，让{r1,c1}为对角线右上角的端点
void printMatrixZigzag(vector<vector<int>> m) {
    int row = m.size(), col = m[0].size();
    int r1 = 0, c1 = 0 , r2 = 0, c2 = 0, direction = 1;
    while (r1 < row) {
        printDiagLine(m, r1, c1, r2, c2, direction);
        r1 = (c1 == col - 1) ? r1 + 1 : r1;
        c1 = (c1 == col - 1) ? c1 : c1 + 1;
        c2 = (r2 == row - 1) ? c2 + 1 : c2;  // 注意顺序，避免影响
        r2 = (r2 == row - 1) ? r2 : r2 + 1;
        direction *= -1;
    }
}
void printDiagLine(vector<vector<int>> m, int r1, int c1, int r2, int c2, int direction) {
    for (int i = 0; i <= c1 - c2; i++) {
        if (direction == 1) {  // 左下方向
            printf("%d ", m[r1 + i][c1 - i]);
        } else {  // 右上方向
            printf("%d ", m[r2 - i][c2 + i]);
        }
    }
}

// 6 medium  s写成以从上往下、从左到右进行 Z 字形排列。行数为numRows(r)
// 首先思考每个小周期内，第1列为r个元素，接下来是r-2列的1个元素，一个小周期是t=2r-2个元素
// 如果用额外的空间，比如一个二维数组模拟，那么需要计算列数的上界（向上取整）。
// 其实用一个动态string的数组即可。因为[i][j]后面的[i][j+k]中间的空间可以省略。
// 如果不用空间，找规律。就是利用小周期长度的性质，假设r=4
// 第一行都是 0 t 2t...
// 第二行都是 1 t-1, t+1 2t-1, 小周期的两个可以计算出来{i+j, t+j-i}
// 第三行都是 2 t-2, t+2 2t-2,
// 最后一行  3  t+3  2t+3
string convert(string s, int numRows) {
    int n = s.size(), r = numRows;
    if (r == 1 || r >= n) return s;
    int t = 2*r-2;
    string ret;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; i+j < n; j += t) {  // j为每个小周期对应的第一个下标
            ret += s[i+j];  // 每个小周期的第1个
            // 不能是第一行和最后一行（长度为1）
            if (i > 0 && i < r-1 && t+j-i < n) {
                ret += s[t+j-i];  // 每个小周期的第2个
            }
        }
    }
    return ret;
}

// 59 medium 生成n*n的矩阵，可以顺时针输出[1,n*n]
// 比如{0,0}和{m-1,n-1}两点可以确定画一个圈或者线
vector<vector<int>> generateMatrix(int n) {
    vector<vector<int>> m(n, vector<int>(n, 0));
    int k = 1;  // 遍历的值
    int r1 = 0, c1 = 0, r2 = n-1, c2 = n-1;  // {r1,c1} {r2,c2}
    while (r1 <= r2 && c1 <= c2) {
        if (r1 == r2) {  // 只剩一个点
            m[r1][r1] = k;
        } else {
            for (int j = c1; j < c2; j++) m[r1][j] = k++;
            for (int i = r1; i < r2; i++) m[i][c2] = k++;
            for (int j = c2; j > c1; j--) m[r2][j] = k++;
            for (int i = r2; i > r1; i--) m[i][c1] = k++;
        }
        r1++; c1++; r2--; c2--;
    }
}
// 54 medium 顺时针输出矩阵
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    int row = matrix.size(), col = matrix[0].size();
    vector<int> m(row*col, 0);
    int r1 = 0, c1 = 0;
    int r2 = row-1, c2 = col-1;
    int k = 0;
    while (r1 <= r2 && c1 <= c2) {
        if (r1 == r2) {  // 只剩一行
            for (int j = c1; j <= c2; j++) m[k++] = matrix[r1][j];
        } else if (c1 == c2) {  // 只剩一列
            for (int i = r1; i <= r2; i++) m[k++] = matrix[i][c1];
        } else {
            for (int j = c1; j < c2; j++) m[k++] = matrix[r1][j];
            for (int i = r1; i < r2; i++) m[k++] = matrix[i][c2];
            for (int j = c2; j > c1; j--) m[k++] = matrix[r2][j];
            for (int i = r2; i > r1; i--) m[k++] = matrix[i][c1];
        }
        r1++; c1++; r2--; c2--;
    }
    return m;
}


// 48 medium 方阵顺时针旋转90输出之，要求空间复杂度O1
// 核心思想：旋转可以通过2次翻转得到
// 斜翻：根据主对角线（撇）或者副对角线（捺）
// 正翻：上下或者左右
// 顺时针90/逆时针270，主对角线翻转(i,j)->(n-1-j,n-1-i)+上下翻转(i,j)->(n-1-i,j)
// 顺时针180/逆时针180，主对角线翻转+副对角线翻转(i,j)->(j,i)
// 顺时针270/逆时针90，主对角线翻转+左右翻转(i,j)->(i,n-1-j)
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            swap(matrix[i][j], matrix[n-1-j][n-1-i]); // 主对角线镜像
        }
    }
    for (int i = 0; i < n/2; ++i) {
        for (int j = 0; j < n; ++j) {
            swap(matrix[i][j], matrix[n-1-i][j]); // 上下翻转
        }
    }
}