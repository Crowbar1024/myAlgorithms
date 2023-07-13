#include "myheader.h"


// 62 medium {1,1}->{m,n}，每次只能向下或者向右移动一步，问总共有多少条不同的路径？
// 显然可以用组合思想求解，起点和终点定了，那么一共走了m+n-2步，然后从中选择m-1步是向下的
// 利用杨辉三角，C_n^m=C_(n-1)^(m-1)+C_(n-1)^(m)
// 1 (第0行)
// 1 1
// 1 2 1 (第2行 偶数行)
// 1 3 3 1
// 1 4 6 4 1
// 也可以发现，若是仅仅求一次组合数的话，还不如用DP
int uniquePaths(int m, int n) {
    n = m+n-2, m = m-1, m = min(m,n-m); // 现在求C_n^m
    // 理论上需要遍历全部的C_n^n，但每行都是对称的，所以可以只迭代一半。也可以不全遍历，每一行只迭代到m
    vector<vector<int>> c(n+1, vector<int>(m+1, 1));
    for (int i = 2; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (!(i&1) && j == i/2) { // 偶数行有奇数个值，中间值需要单独考虑
                c[i][j] = c[i-1][j-1] * 2; // 因为偶数行的中间值无法让上一奇数行中间两个赋值，所以double一下
            } else {
                c[i][j] = c[i-1][j-1] + c[i-1][j];
            }
        }
    }
    return c[n][m];
}




// 返回所有小于非负整数 n 的质数的数量
// 注意：从 i*i 开始标记，比如5，5*2 5*3 5*4都已经标记过了
int countPrimes(int n) {
    int ret = 0;
    vector<bool> isPrime(n, true);
    for (int i = 2; i < n; ++i) {
        if (isPrime[i]) {
            ret += 1;
            if ((long long) i*i >= n) continue;
            for (int j = i*i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    return ret;
}

// NC132/139 约瑟夫环
// 有n个人，从第一个人开始数，数到第q个人，让他挂掉，然后接着接龙，看谁活到最后
// 算法来源，具体数学中，数学归纳法
// 这个版本是序号从0开始，所以如果从1开始，只需要+1
int JosephRing(int n, int q) {
    int rescurID = 0;
	for (int i=2; i<=n; ++i) {
        rescurID = (rescurID+q)%i;
    }
    return rescurID;
}

// 给出一个多边形的全部顶点，判断(ax,bx)这个点是否唉多边形内部
// 需要注意的是，给定的其实是多边形的顺时针的边（因为单纯给出顶点，可以有多个形状不同的多边形）
// 采用射线法，往右的射线与多边形的边的交点的个数，若为奇，在内部，否则不在
// 这种方法在经过多边形的拐点时失效
// 2 连线角之和为360，当不为凸时，有些位置失效
// 我自己的想法是由于拐点的存在，判断这两条边是否一上一下，是的话算交点（本质上这样就是一条边）
struct edge {
    pair<double,double> p1;
    pair<double,double> p2;
};
bool isPoiWithinPoly(pair<double,double> p, vector<edge>& e) {
    int intersection = 0;
    for (int i = 0; i < e.size(); ++i) {
        edge tmp = e[i];
        if (tmp.p1.first > tmp.p2.first) {  // 这是下面判断边交点的前提
            pair<double,double> tmpPoint = tmp.p1;
            tmp.p1 = tmp.p2;
            tmp.p2 = tmpPoint;
        }
        if (isRayIntersectsSegment(p, e, i)) ++intersection;
    }
    return intersection&1 ? true : false;
}
bool isRayIntersectsSegment(pair<double,double> x, vector<edge>& e, int i) {
    pair<double,double> a = e[i].p1;
    pair<double,double> b = e[i].p2;
    if (a.second==b.second)  // 与射线平行或者重合
        return false;
    if (a.second>x.second && b.second>x.second)  // 在射线上边
        return false;
    if (a.second<x.second && b.second<x.second) // 在射线下边
        return false;
    if (a.second == x.second && b.second != x.second) {
        if ((e[i-1].p1.second-x.second)*(a.second-x.second) < 0)  // 上下两边
            return true;
        else return false;
    }
    if (a.second == x.second && b.second != b.second) {
        i = i > 0 ? i : e.size();
        if ((e[(i-1)%e.size()].p1.second-x.second)*(a.second-x.second) < 0)  // 上下两边
            return true;
        else return false;  //  不需要检测b.second == x.second，上一个循环已经测试过了
    }
    if (b.second==x.second && a.second>x.second)  // 交点为a
        return false;
    if (b.first<x.first && b.second<x.second)  // 在射线左边
        return false;
    double xseg = a.first-(a.first-b.first)*(b.second-x.second)/(b.second-a.second);  // 交点
    if (xseg<x.first)  // 交点在射线起点的左侧
        return false;
    return true;
}



// 169 easy(hard) 找多数元素，即出现次数大于int(n/2)的元素，且必然存在
// 要求时间On，空间O1
// 有了这个要求之后，就会变得十分复杂
// 我自己想了使用快排思想去找，当{lt+1,rt}落在int(n/2)两侧就能说明那个数为多数元素，否则递归
// 我认为平均复杂度符合题意，哈哈哈哈哈，官方题解却没有这个思路！
// 后来学了摩尔投票法，即相同的+1，不同的-1，为0换新的。
int majorityElement(vector<int>& nums) {
    int ret = nums[0], cnt = 1;
    for (int i = 1; i < nums.size(); ++i) {
        if (cnt == 0) ret = nums[i];
        if (nums[i] != ret) --cnt;
        else ++cnt;
    }
    return ret;
}    
// 229 medium(hard) 找出现次数大于int(n/3)的元素
vector<int> majorityElement(vector<int>& nums) {
    int element1 = 0, element2 = 0; // 候选人
    int vote1 = 0, vote2 = 0; // 票数
    for (auto & num : nums) {
        if (vote1 > 0 && num == element1) { //如果该元素为第一个元素，则计数加1
            vote1++;
        } else if (vote2 > 0 && num == element2) { //如果该元素为第二个元素，则计数加1
            vote2++;
        } else if (vote1 == 0) { // 选择第一个元素
            element1 = num;
            vote1++;
        } else if (vote2 == 0) { // 选择第二个元素
            element2 = num;
            vote2++;
        } else { //如果三个元素均不相同，则相互抵消1次
            vote1--;
            vote2--;
        }
    }
    int cnt1 = 0, cnt2 = 0; // 票数
    for (auto & num : nums) {
        if (vote1>0 && num==element1) cnt1++;
        if (vote2>0 && num==element2) cnt2++;
    }
    vector<int> ret;
    if (vote1>0 && cnt1>nums.size()/3) ret.emplace_back(element1);
    if (vote2>0 && cnt2>nums.size()/3) ret.emplace_back(element2);
    return ret;
}


// shuffle算法
class Solution {
private:
    vector<int> nums;
public:
    Solution(vector<int>& nums) {
        this->nums = nums;
    }
    vector<int> reset() {
        return this->nums;
    }
    vector<int> shuffle() {
        vector<int> ret = nums;
        for (int i = nums.size()-1; i >= 0; --i) {
            int idx = rand()%(i+1);
            swap(ret[i], ret[idx]);
        }
        return ret;
    }
};