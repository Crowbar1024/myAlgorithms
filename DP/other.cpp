#include "myheader.h"







// 85 hard 01图中全是1组成的最大矩阵面积
// 这道题最大的难点在于想到用DP去处理
// 创新点是三个DP数组
// height[i][j]：第i行第j列若为1，往上的连续1的数量，就是以当前点的高度为高度的矩阵高度
// leftmost[i][j]，以height[i][j]为高度的矩阵的左边界
// rightmost[i][j]，以height[i][j]为高度的矩阵的右边界
// 这样以height[i][j]为高度的矩阵的面积 = (rightmost[i][j]-leftmost[i][j]+1)*height[i][j]
// 状态压缩，从上到下遍历，leftmost必须从左往右，rightmost必须从右往左
int maximalRectangle(vector<vector<char>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return 0;
    int n = matrix.size(), m = matrix[0].size();
    vector<int> leftmost(m, 0);
    vector<int> rightmost(m, m-1);
    vector<int> height(m, 0);
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        int tmp = m-1;  // 记录第i行从右往左遍历时的每一个从0变过来的1的位置
        for (int j = m-1; j >= 0; --j) {
            if (matrix[i][j] == '1') {
                rightmost[j] = min(rightmost[j], tmp);  // 记录高度为weight[j]的矩形的右边
            } else {
                tmp = j-1;
                rightmost[j] = m-1;
            }
        }
        tmp = 0;
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == '1') {
                height[j] += 1;
                leftmost[j] = max(leftmost[j], tmp);
                ret = max(ret, (rightmost[j]-leftmost[j]+1)*height[j]);
            } else {
                height[j] = 0;
                tmp = j+1;
                leftmost[j] = 0;
            }
        }
    }
    return ret;
}




// poj 1037
// n个木棒，长度为[1:n]，构成一高一低的栅栏（前两个既可以上升也可以下降），问按照字典序的第k个序列
// up[i][j]，长度为j的栅栏，头为i，且一开始是升序的长度
// down[i][j]，长度为j的栅栏，头为i，且一开始是降序的长度
// 根据定义有：down[i][j] = \sum_{k=1}^{k=i-1}up[k][j-1]
// 类似可以得到up[i][j]，但为了计算简单，可以发现，up[i][j]=down[j-i+1][j] 可以理解为Cni=Cn(n-i)，即对称
vector<int> DecorativeFence(int n, int k) {
    vector<vector<int>> up(n+1, vector<int>(n+1, 0));
    vector<vector<int>> down(n+1, vector<int>(n+1, 0));
	up[1][1] = down[1][1] = 1;
	for(int j=2; j<=n; ++j) {
		for(int i=1; i<=j; ++i) {
            // down[i][j] = \sum_{k=1}^{k=i-1}up[k][j-1]
            // 则 down[i][j]-down[i-1][j] = up[i-1][j-1]
			down[i][j] = down[i-1][j] + up[i-1][j-1];
		}
        for(int i=1; i<=j; ++i) {
			up[i][j] = down[j-i+1][j];
		}
	}
    vector<int> ret;
    // 现在up[i][n],down[i][n]储存了所有的情况，k肯定是位于其中的一个区间，即通过不断减，k<=up/down[i][n]其中的某个数
    int flag, v = 0;
    vector<int> s(n);
	for(int i=1; i<=n; ++i) s[i]=i;  // 用来储存每个数
	for(int j=n; j>=0; --j) {
		if (j==n) {
			for (int i=1; i<=n; i++) {  
				if (k<=down[i][j]) {  // up[i][n]的字典序在down[i][n]之后
					v = i;  // k位于开头为v的降序情况组合中
                    flag=0;
                    break;
				}
				k -= down[i][j];
				if (k<=up[i][j]) {
					v = i;
                    flag = 1;
                    break;
				}
				k -= up[i][j];
			}
		} else {
			if (flag) {
				for (int i=v; i<=j; ++i) {
					if (k<=down[i][j]) {
						v=i;
                        break;
					}
					k -= down[i][j];
				}
			} else {
				for (int i=1; i<v; ++i) {
					if (k<=up[i][j]) {
						v = i;
                        break;
					}
					k -= up[i][j];
				}
			}
			flag ^= 1;
		}
		ret.emplace_back(s[v]);
        // 因为我每次取的v其实只是剩下的一堆数的编号，比如剩下41，是在down[i][2]中取
		for(int i=v; i<j; ++i) s[i] = s[i+1];  // 删除s[v]
	}
}

  