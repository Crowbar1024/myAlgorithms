#include "myheader.h"




// 128 medium nums未排序，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。
// [100,4,200,1,3,2]-->[1, 2, 3, 4] return 4
// 要求时间复杂度 O(n)
// 肯定不能排序，根据题意只能使用哈希表
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int ret = 0;
    for (const int& num : s) {
        // 没有前缀，所以只有连续的序列的第一个才能进入下面的循环
        if (!s.count(num-1)) {
            int len = 1;
            int suffix = num;
            while (s.count(++suffix)) ++len;
            ret = max(ret, len);
        }
    }
    return ret;
}


// HJ41 medium 称砝码 给出所有砝码的重量（互不相同）和数量
// 现在要用这些砝码去称物体的重量(放在同一侧)，问能称出多少种不同的重量。（称重重量包括 0）
// 和子集很像。也和位运算很像。
// 砝码种类和每种数量均小于等于10，所以以上都可以做。
// 这里试试set
// {0} -> {0,1} -> {0,1,2}  每个砝码进来，所有+上该砝码重量
int getWeightNum(vector<int>& weight, vector<int>& cnt) {
    vector<int> w;
    for (int i = 0; i < weight.size(); ++i) {
        w.insert(w.end(), cnt[i], weight[i]);
    }
    unordered_set<int> s{0};
    for (const int& n : w) {
        unordered_set<int> tmp(s);
        for (const int& tn : tmp) s.insert(tn+n);
    }
    return s.size();
}