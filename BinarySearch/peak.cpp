#include "myheader.h"

/*
    找山峰

*/

// 162 medium 用对数时间找到有序数组任意一个peak
// 核心思想是构建[l,r]的范围，这俩边界就大于外面的，然后不断缩小
int findPeakElement(vector<int>& nums) {
    int l = 0, r = nums.size() - 1;
    while (l < r) {
        int midl = l + ((r - l) >> 1);
        int midr = midl + 1;
        if (nums[midl] < nums[midr]) l = midr;
        else r = midl;
    }
    return l;
}

// 852 medium 只有一个山峰，严格递增递减，和162一样
int peakIndexInMountainArray(vector<int>& arr) {
    int n = arr.size(), l = 0, r = n-1;
    while (l < r) {  // 根据题意，不用相等
        int midl = l + (r-l)/2;
        int midr = midl+1;
        if (arr[midl] < arr[midr]) l = midr;
        else r = midl;
    }
    return l;
}