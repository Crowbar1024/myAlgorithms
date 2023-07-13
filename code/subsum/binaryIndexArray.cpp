#include "myheader.h"


// 307 medium 数组完成两类查询。1 更新值 2 返回区间和
// 每个节点都保存一定的区间和，比如 13 = 8 + 4 + 1
// [1,13] = [1,8] + [9,12] + [13,13] 从节点13开始（管理1个），再加上节点12（管理4个），再加上节点8（管理8个）
// 即节点i(二进制的最后一个1在第k位，从0下标开始)，管理的nums[i]一共有2^k个元素
// 用数学语言：节点i管理的区间为[i-lowbit(i)+1,i]
// 即 1101->1100->1000，消除尾1的过程（证明查看https://oi-wiki.org/ds/fenwick/）
// 如果是单点更新，后续包括该节点的区间也要更新
// 比如 0101->0110->1000，增加尾1的过程
class NumArray {
private:
    vector<int> bia;  // 树状数组，每个节点存的是特殊的区间和（长度不同），下标从1开始
    vector<int>& nums; // 原始数组，下标从0开始
    inline int lowbit(int x) { return x & (-x); } // 获得x的最后一位1
    void addBIA(int index, int increment) { // 单点更新
        for (int i = index; i <= nums.size(); i += lowbit(i)) {
            bia[i] += increment;
        }
    }
    int prefixSum(int index) { // [1,index]区间查询
        int ret = 0;
        for (int i = index; i > 0; i -= lowbit(i)) ret += bia[i];
        return ret;
    }
public:
    NumArray(vector<int> &nums) : nums(nums), bia(nums.size()+1,0) {
        for (int i = 1; i <= nums.size(); ++i) addBIA(i, nums[i-1]); // 注意下标
    }
    void update(int index, int val) { // 下标从0开始
        int increment = val - nums[index];
        addBIA(index+1, increment); // 区分两个数组的下标
        nums[index] = val;  // 别忘记原始数组要保留变化
    }
    int sumRange(int left, int right) { // [left,right]区间查询，sum(right)-sum(left-1)
        int ret = prefixSum(right+1) - prefixSum(left); // 注意下标
        return ret;
    }
};


// 315 hard 计算数组中每个元素的右边中小于该元素的元素个数
// 用cnt数组记录每种元素的出现频率。将原始数组从左向右遍历，这样cnt更新时，每个桶左边的presum就是求解的值
// 这和hash表边插入边更新是一个思路。
// 这样每次给cnt单点更新，再查询[1,index]，可以使用树状数组
class Solution {
private:
    vector<int> arr; // set{nums}，其实是一个桶
    vector<int> cnt; // 记录arr的每个元素出现的频率
    int n; // 元素的种类数
    int lowbit(int x) { return x & (-x); }
    void Init(vector<int>& nums) {
        arr = nums;
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end()); // 唯一有序
        n = arr.size();
        cnt.resize(n+1, 0);
    }
    void addBIA(int index, int increment) {
        for (int i = index; i <= n; i += lowbit(i)) cnt[i] += increment;
    }
    int prefixSum(int index) {
        int ret = 0;
        for (int i = index; i > 0; i -= lowbit(i)) ret += cnt[i];
        return ret;
    }
public:
    vector<int> countSmaller(vector<int>& nums) {
        Init(nums);
        vector<int> ret(nums.size(), 0); // n不是个数，是种数
        for (int i = nums.size()-1; i >= 0; --i) {
            // 找到nums[i]在arr中的下标，注意arr是0开始，但cnt树状数组是从1开始，所以+1就是cnt的位置
            int index = lower_bound(arr.begin(), arr.end(), nums[i])-arr.begin()+1;
            ret[i] = prefixSum(index-1);  // [1,index-1]区间和，就是当前小于nums[i]的数量
            addBIA(index, 1); // 出现频率+1
        }
        return ret;
    }
};
// 这道题是逆序对数目的进阶版


// 493 上面题目的进阶，和逆序对类似，只不过是统计自己比右边的数的2倍还大的个数
// 离散化+map/set
// 即a[i]>a[j]*2, i < j
// 只要把lower_bound那里改一下，因为统计的是前缀和
// 比如 1 0 1 0 0 0 来了个6
// 按照之前的思路，要加上[1,5]的前缀和，按照这里的题意，只需要统计[1,2]就行了
// 即找到最后一个小于等于2的数，但这个数不一定存在arr中
// 这题使用另一种离散化方法，即用map存
class Solution {
private:
    vector<int> cnt;
    set<int> s;
    int n;  // set size
    map<int,int> m;

    int lowbit(int x) { 
        return x & (-x);
    }

    // nums 1 4 9 4 1
    // set  1 4 9
    // map  1-1 4-2 9-3
    // 这样用map[nums[i]]就能得到其树状数组中的下标
    void discretization(vector<int>& nums) {
        s.insert(nums.begin(), nums.end());
        int i = 0;  
        for (int num : s) {
            m[num] = ++i;
        }
    }

    void Init(vector<int>& nums) {
        discretization(nums);
        n = s.size();
        cnt.resize(n+1, 0);
    }

    void addBIA(int index, int increment) {
        for (int i = index; i <= n; i += lowbit(i)) {
            cnt[i] += increment;
        }
    }

    int prefixSum(int index) {
        int ret = 0;
        for (int i = index; i > 0; i -= lowbit(i)) ret += cnt[i];
        return ret;
    }


public:
    int reversePairs(vector<int>& nums) {
        Init(nums);
        int ret = 0;
        for (int i = nums.size()-1; i >= 0; --i) {
            int index = m[nums[i]];
            // 1 9 4 9 1
            // 9的tmp为5
            // 大于等于5是9，退一个是4
            // 5 5
            // tmp为3，大于等于3是5，是头，不能加
            // -5 -3
            // -5tmp为-2，大于等于-2是-5，退一个是-3
            // 所以，正数和负数的一半再退一个得分开讨论
            // 尽量使用lower，不要用upper
            int tmp = ((long long)nums[i] + 1) / 2;
            if (nums[i] < 0) {
                tmp = nums[i] / 2;
            }
            auto it = s.lower_bound(tmp);
            if (it != s.begin()) {
                it--;
                ret += prefixSum(m[*it]);
            }
            addBIA(index, 1);
        }
        return ret;
    }
};

// 295 hard 两种操作，一个往数组中添加元素，一个求数组的中位数。5e4次数的插入和查询
// 中位数，也可以理解找到第n/2小的数，即频率数组中前缀和为n/2的对应值
class MedianFinder {
private:
    vector<int> cnt;  // 输出频率的树状数组
    static constexpr int maxn = 2e5+5; // 数据范围为[-1e5,1e5]
    static constexpr int OFFSET = 1e5+1; // 数据范围为[-1e5,1e5]，要映射到树状数组，后者下标从1开始
    int n; // 数组中插入元素的个数
    int lowbit(int x) { return x & (-x); }
    void addBIA(int index, int increment) {
        for (int i = index; i < maxn; i += lowbit(i)) cnt[i] += increment;
    }
    int prefixSum(int index) {
        int ret = 0;
        for (int i = index; i > 0; i -= lowbit(i)) ret += cnt[i];
        return ret;
    }
    int getKthMinNum(int k) {
        int l = 1, r = maxn; // [l,r)
        while (l < r) {
            int mid = l+(r-l)/2;
            int tmp = prefixSum(mid);
            if (tmp < k) l = mid+1;
            else r = mid;
        }
        return l;
    }
public:
    MedianFinder() : n(0), cnt(maxn, 0) {}
    void addNum(int num) {
        addBIA(num+OFFSET, 1);
        n++;
    }
    double findMedian() {
        int res1 = getKthMinNum((n+1)/2)-OFFSET; // 奇数个，就是中间
        if (n&1) return res1;
        int res2 = getKthMinNum(n/2+1)-OFFSET;
        return (res1+res2)*0.5;  // 返回类型是double
    }
};
// 当然有优先队列解法



// SCU校赛 求n个正整数组成的序列中，严格上升子序列的个数
// 比如 1 4 1000 1000 1001 中 
// 每个元素算一个 5；两个一组 4+3+2 = 9；三个一组 3+2+2 = 7；四个一组 2；一共23
// 因为结果过大，对MOD=1e9+7取模
// 第一步没得说，排序+离散化
// 1 4 1000 1001
// 用BIA去存放严格上升子序列的个数，因为这符合结合律（加法的和也是）
// query(pos)就是求前pos个dv的数组成的序列中的严格上升子序列的个数
// 假设遍历到了第2个1000，找到1000在dv中的位置是第3个
// m=prefixSum(3-1)是找到以前2个数组成的序列中的严格上升子序列的个数，有 1 4，4这么2个
// 那么每次往树状数组中添加一个数，那么以1000结尾的序列个数有m+1个（1 4 1000, 4 1000, 1000）
// 所以要在第 3 位上加上 m+1
// 想一想，离散化计数前缀和，每次都是add(newN, 1)，即出现次数+1
// 基础的add(pos, inc)，即这个位置上的数本身加变量inc
// 那么根据用BIA去存放严格上升子序列的个数，也要加上增加的严格上升子序列的个数
// 这道题和求右边大于自己的数很像，因为遍历的顺序，让dv对应的树状数组每次的更新都有效
class StrictlyIncreasingSubsequence {
private:
    vector<int> cnt;  // BIA
    vector<int> dv;  // vector nums after discretization
    int n;
    const int MOD=1e9+7;

    void discretization(vector<int>& nums) {
        dv = nums;
        sort(dv.begin(), dv.end());
        dv.erase(unique(dv.begin(), dv.end()), dv.end());
        n = dv.size();
        cnt.resize(n+1, 0);
    }

    int lowbit(int x) { 
        return x & (-x);
    }

    void addBIA(int index, int increment) {
        for (int i = index; i <= n; i += lowbit(i)) {
            cnt[i] = (cnt[i]+increment)%MOD;
        }
    }

    int prefixSum(int index) {
        int ret = 0;
        for (int i = index; i > 0; i -= lowbit(i)) ret = (ret+cnt[i])%MOD;
        return ret;
    }
public:
    int getNumber(vector<int>& nums) {
        discretization(nums);
        for (int num : nums) {
			int pos = lower_bound(dv.begin(), dv.end(), num) - dv.begin() + 1;
			addBIA(pos, prefixSum(pos-1)+1);
		}
		return prefixSum(n);
    }
};
// 这道题有多种变体，现在要求返回的结果长度至少为2，且不能重复
// 只要做简单的改进
// 比如上一道题有2个1000，当遍历到了第2个1000时
// 首先得到前面2个数组成的严格上升子序列个数m，m+1就是加上1000后新的个数
// 然后不能add这个数了，因为前面有了1000，所以需要减掉前面增加的个数，操作如下
// addBIA(pos, m+1-last[pos]);
// last[pos] = m+1;
// 这样保证多个1000只在第一个1000有效
// 注意不能用if省略，因为当前循环内的m+1不一定等于last[pos]
// 最后query[n]-n保证只出现长度大于1的