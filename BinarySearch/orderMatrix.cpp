#include "myheader.h"

// 240 medium 矩阵每行每列都是严格升序排列，判断是否能找到target
// 这里利用给定的数据结构，从右上角或者左下角开始找
// 比如从左下角，往右都是比它大的，往上都是比它小的 O(m+n)
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int m = matrix.size(), n = matrix[0].size();
    for (int i = m-1, j = 0; i >= 0 && j < n; ) {
        if (matrix[i][j] < target) j++;
        else if (matrix[i][j] > target) i--;
        else return true;
    }
    return false;
}
// 显然还存在O(log(m+n))的解法
// 对行一次二分，再对列一次二分，得到的是左下角一块，然后循环
bool searchMatrix(vector<vector<int>>& matrix, int target) {
    int i = 0, j = matrix[0].size() - 1;
    // 这个while的条件判断有点意思，把两种终止情况进行先货再取反的操作
    while (i < matrix.size() && j >= 0) {
        if ((j = searchColumn(matrix, i, j, target)) == -1) break;
        if ((i = searchRow(matrix, i, j, target)) == matrix.size()) break;
        if (matrix[i][j] == target) {
            return true;
        }
    }
    return false;
}
// 返回的是第 i 行 [0,j] 中第一个大于 target 的前一个数
int searchColumn(vector<vector<int>> &matrix, int i, int j, int target) {
    int l = 0, r = j;
    while (l < r) {
        int x = (l + r) / 2;
        if (target < matrix[i][x]) {
            r = x - 1;
        } else if (target > matrix[i][x]) {
            l = x + 1;
        } else {
            l = r = x;
        }
    }
    // 如果都大于 target，有两种情况
    // 1：l为0，说明整行都找不到，让l置为-1，即不存在
    // 2：第l-1列才是想要的
    if (matrix[i][l] > target) --l;
    return l;  // 如果都小于 target，会返回right
}
// 返回的是第 j 列 [i,m-1] 中第一个大于 target 的前一个数
int searchRow(vector<vector<int>> &matrix, int i, int j, int target) {
    int l = i, r = matrix.size() - 1;
    while (l < r) {
        int x = (l + r) / 2;
        if (target < matrix[x][j]) {
            r = x - 1;
        } else if (target > matrix[x][j]) {
            l = x + 1;
        } else {
            l = r = x;
        }
    }
    // 如果都小于 target，有两种情况
    // 1：l为m-1，说明整列都找不到，让l置为m，即不存在
    // 2：第l+1行才是想要的
    if (matrix[l][j] < target) ++l;
    return l;
}


// 378 medium n*n的矩阵，每行每列都是严格升序排列，找出第k小的数，注意矩阵中也有可能存在相同的数，比如左下角和右上角
// 第k小，允许重复，先想用优先队列做。根据这个数据结构，可以轻易找到较小的数，那么可以用小顶堆来做。
// 拿第一列做初始化。每次把队头取出来，即删去当前最小的数，放进去队头同行的下一个元素
// 持续k-1次，这样使矩阵的前k-1小的元素都被删除了，那么剩下的队头就是target
struct point {
    int row;
    int col;
    int value;
    bool operator<(const point& a) const {
        return value > a.value;  // 这样队头是最小的
    }
};
int kthSmallest(vector<vector<int>>& matrix, int k) {
    int m = matrix.size();
    priority_queue<point> pq;
    for (int i = 0; i < m; i++) pq.push(point{i, 0, matrix[i][0]});
    while (--k) {
        point tmp = pq.top();
        pq.pop();
        if (++(tmp.col) < m) pq.push(point{tmp.row, tmp.col, matrix[tmp.row][tmp.col]});
    }
    point ret = pq.top();
    return matrix[ret.row][ret.col];
}
// 如果要用二分法，根据经验，二分的对象就是数，统计所有小于等于这个数的数的个数，计数法
int kthSmallest(vector<vector<int>>& matrix, int k) {
    int n = matrix.size();
    int l = matrix[0][0], r = matrix[n-1][n-1];
    while (l < r) {
        int x = l + (r - l) / 2;
        int cnt = 0;  // 小于等于mid的数的个数
        // 利用矩阵的排序，从右上角（左下角也OK）统计，小于等于mid的是矩阵左上角那部分
        for (int i = 0, j = n-1; i < n; i++) {
            while (j >= 0 && matrix[i][j] > x) j--;
            cnt += j + 1;
        }
        if (cnt < k) l = x + 1;
        else r = x;  // 因为允许重复，当cnt > k时，不一定对应的mid值就不是第k小的数
    }
    return l;
}

// 668 hard m*n的乘法表，返回表中第k小的数字。二分+计数
// 乘法表符合严格升序的性质，并且还有很多多余的性质
// 比如第i行比x小的数的个数为 min(int(x/i),n)
// 所以当i > x/n时，只有int(x/i)个数符合题意
// 小于等于x的数的个数就等于 int(x/n)*n + sum_{i=int(x/n)+1}^{m}{int(x/i)}
int findKthNumber(int m, int n, int k) {
    int l = 1, r = m*n;
    while (l < r) {
        int x = l + (r-l)/2;
        int cnt = x/n*n;
        for (int i = x/n+1; i <= m; ++i) cnt += x/i;
        if (cnt < k) l = x + 1;
        else r = x;
    }
    return l;
}


// 719 hard 返回n维nums中两两（不能相同）之差的绝对值（不能相同）中k小的数
// 同378的二分计数做法
// 可以发现行严格递增，列严格递减。所以{i,j}如果小于等于k，说明其左边和下边都小于等于k，所以让j往右再试试
// e.g {1,4,5,10}
//     4    5    10
// 1   3    4    9
// 4        1    6
// 5             5
int smallestDistancePair(vector<int>& nums, int k) {
    sort(nums.begin(), nums.end());
    int n = nums.size();
    int l = 0, r = nums[n-1]-nums[0];  // 0虽然取不到，但取更小依然能遍历出所有可能的情况
    while (l < r) {
        int x = l + (r-l)/2;
        int cnt = 0;
        for (int i = 0, j = 1; i < n-1; ++i) {
            while (j < n && nums[j]-nums[i] <= x) ++j;
            cnt += j - i - 1;  // -1是因为j跑到了恰好符合题意的位置的下一个
        }
        if (cnt < k) l = x + 1;
        else r = x;
    }
    return l;
}


// 786 hard 递增数组由1和若干质数组成，且其中所有整数互不相同。求两两组成（i<j）的第k个最小的分数
// e.g {1,2,3,5}
//     2    3    5
// 1  1/2  1/3  1/5 
// 2       2/3  2/5
// 3            3/5
// 同719，但每行都是递减，每列递增。所以遍历到[i,j]小于等于x，表示j之后的都小于x，直接跳到下一行的第j列
// 而且这里我跳跃是写在for循环的最后，与之前不同
// 最后注意浮点数
vector<int> kthSmallestPrimeFraction(vector<int>& arr, int k) {
    int n = arr.size();
    double l = 0, r = 1;
    const double eps = 1e-8;
    int a, b;  // 返回值
    while (r - l > eps) {
        double x = (l+r)/2;
        int cnt = 0;
        for (int i = 0, j = 1; i < n-1 && j < n; ++j) {
            if (abs(arr[i]*1.0/arr[j]-x) < eps) {
                a = arr[i];
                b = arr[j];
            }
            if (arr[i]*1.0/arr[j] <= x) {
                cnt += n-j;
                ++i; --j;  // 因为for循环还要+1，这里要减-1，相当于j=i
            }
        }
        if (cnt < k) l = x;  // 都是分数，不能+1
        else r = x;
    }
    return vector<int>{a, b};
}
