#include "myheader.h"

// easy 704 二分查找有序，找到了返回下标，找不到返回-1
int search(vector<int>& nums, int target) {
    int n = nums.size(), l = 0, r = n-1;
    while (l <= r) {
        int mid = l+(r-l)/2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) l = mid+1;
        else r = mid-1;
    }
    // 一定能找到，所以剩下就是找不到
    return -1;
}
int search(vector<int>& nums, int target) {
    int n = nums.size(), l = 0, r = n;
    while (l < r) {
        int mid = l+(r-l)/2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) l = mid+1;
        else r = mid-1;
    }
    if (l == n) return -1;
}

// easy 744 char类型，找第一个大的字母
char nextGreatestLetter(vector<char>& letters, char target) {
    int n = letters.size(), l = 0, r = n-1;
    if (letters[n-1] <= target) return letters[0];  // 如果都比它小，找第一个比它小的
    while (l < r) {
        int mid = l+(r-l)/2;
        if (letters[mid] <= target) l = mid+1;
        else r = mid;
    }
    return letters[l];
}

// easy 35 给定一个排序数组和一个目标值，返回其索引。如果不存在，返回按顺序插入的位置
int searchInsert(vector<int>& nums, int target) {
    int n = nums.size(), l = 0, r = n-1;
    while (l <= r) {
        int mid = l + (r-l)/2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) l = mid+1;
        else r = mid-1;
    }
    return l;
}

// easy 1385 找到满足对于arr1[i] ，不存在任何元素 arr2[j] 满足 |arr1[i]-arr2[j]| <= d 的arr1元素个数
// 其实就是检测arr2中第一个大于x的，和最后一个小于x的
int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
    int ret = 0, n = arr2.size();
    sort(arr2.begin(), arr2.end());
    for (int &x : arr1) {
        int pos = lower_bound(arr2.begin(), arr2.end(), x) - arr2.begin();
        bool flag = true;
        if (pos < n) flag &= (arr2[pos]-x > d);  // =类的优先级向来很低
        if (pos > 0) flag &= (x-arr2[pos-1] > d);
        if (flag) ret += 1;
    }
    return ret;
}

// medium 34 非降序序列，找到target的第一个和最后一个
// 一开始想一次二分，发现很困难
vector<int> searchRange(vector<int>& nums, int target) {
    vector<int> ret{-1, -1};
    auto itl = lower_bound(nums.begin(), nums.end(), target);
    auto itr = upper_bound(itl, nums.end(), target);
    if (nums.empty() || itl == nums.end() || itl == itr) return ret;
    ret[0] = itl - nums.begin();
    ret[1] = itr - nums.begin() - 1;
    return ret;
}


// easy 441 n = 1 + 2 + 3 +...+k k可能站不满最后一行，返回前面的行数
int arrangeCoins(int n) {
    int l = 1, r = n;
    while (l < r) {
        int mid = l+(r-l+1)/2;  // 根据收缩方向
        if ((long long) mid*(mid+1) <= (long long) 2*n) l = mid;
        else r = mid-1;
    }
    return l;
}



// 33 medium 一个升序排列的数组（元素唯一），向右移动了一定距离，Ologn找target
// 1 A-B，B部分全部比A小，先找B头。 2 再找target
int search(vector<int>& nums, int target) {
    int len = nums.size();
    int l = 0, r = len-1; // [l,r]
    int p; // B头，即原升序的头
    if (nums[l] < nums[r]) p = 0; // 没有移动
    else {
        while (l < r) { // 由l=mid+1说明不是<=
            int mid = l + ((r - l) >> 1);
            if (nums[mid] < nums[r]) r = mid;  // mid和r都在B
            else l = mid + 1; // mid在A，r在B（元素唯一，所以一定不相等）
        }
        p = l;
    }
    l = 0, r = len;  // 我比较习惯左闭右开
    while (l < r) {
        int mid = l + ((r - l) >> 1);
        int realMid = (mid + p) % len;
        if (target < nums[realMid]) r = mid;
        else if (target > nums[realMid]) l = mid + 1;
        else return realMid;
    }
    return -1;
}
// 一次2分的写法
int search3(vector<int>& nums, int target) {
    int l = 0, r = nums.size()-1;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (target == nums[mid]) return mid;
        if (nums[l] <= nums[mid]) { // l和mid在同侧，r不确定
            if (target >= nums[l] && target < nums[mid]) r = mid - 1; // target 在[l,mid)
            else l = mid + 1; // target 在(mid,r]（mid之前返回了）
        } else { // l和mid不同侧，且nums[l]>nums[mid]
            if (target > nums[mid] && target <= nums[r]) l = mid + 1; // target在(mid,r]
            else r = mid - 1; // target在[l,mid)
        }
    }
    return -1;
}

// 81 medium 同33 但可以重复
// 这道题本来想利用hard的154的思想，但154是求最小值，由于重复，返回的并不一定是B的头的位置
// 所以这道题得写比较复杂的条件判断
// 还是用一个[l,r]去维护一个包含target的范围
// 思考，有多种情况，从最极端的情况开始：
// 1 nums[l]==nums[mid]==nums[r]，那么直接常数的向内缩小
// 2 nums[l]<=nums[mid]: l和mid只可能在同一侧（如果不在同侧，只可能nums[l]==nums[mid]，但此时由于数组的性质，只可能出现1的情况）
// 所以这里要分target在mid左侧和右侧
// 注意的是，如果在左侧，必须加入target >= nums[l]的限制，因为不写有可能target出现在右侧
// 3 以上的else，那么l和mid只可能在不同侧，且nums[l]>nums[mid]
// 所以这里要分target在mid左侧和右侧
// 注意的是，如果在右侧，必须加入target <= nums[r]的限制，因为不写有可能target出现在左侧
// 同时注意写的时候判断左右的优先级，哪边好判断就先写哪边，显然2判断左边，3判断右边好做
// 最后判断条件终止的情况，由于最后是在while里面返回true，如果[4,5]找5，所以最后[5,5]还需要判断一次
// 综上是l<=r
bool search2(vector<int>& nums, int target) {
    int len = nums.size();
    if (len == 0) return false;
    int l = 0, r = len - 1;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (target == nums[mid]) return true;
        if (nums[l] == nums[mid] && nums[r] == nums[mid]) {
            l++;
            r--;
        } else if (nums[l] <= nums[mid]) {
            if (target >= nums[l] && target < nums[mid]) r = mid - 1;
            else l = mid + 1;
        } else {
            if (target > nums[mid] && target <= nums[r]) l = mid + 1;
            else r = mid - 1;
        }
    }
    return false;
}