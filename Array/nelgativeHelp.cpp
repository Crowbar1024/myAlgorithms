#include "myheader.h"

// NC30 medium 给定一个无重复元素的整数数组nums，请你找出其中没有出现的最小的正整数
// 要求空间复杂度 O(1)，时间复杂度 O(n)
// 数组长度为n，那么缺失的数不是1-n，就是n+1
// 因为是寻找缺失的最小正整数，所以可以先把数组中的负数和零移除出查找范围，将其改为n+1。
// 以[-2,3,4,1,5]为例，改成[6,3,4,1,5]
// 然后再遍历数组，如果nums[i]小于等于n，将nums[i]-1对应的下标上的值改为负数，表示该位置的正整数已经出现
// 对于上面的数组，n=5：
// i=0时，abs(nums[i])=6，没有更改
// i=1时，abs(nums[i])=3,将数组下标为2的数值改为负数，即[6,3,-4,1,5]，表示3已经出现过了
// i=2时，abs(nums[i])=4,将数组下标为3的数值改为负数，即[6,3,-4,-1,5]，表示4已经出现过了
// i=3时，abs(nums[i])=1,将数组下标为0的数值改为负数，即[-6,3,-4,-1,5]，表示1已经出现过了
// i=4时，abs(nums[i])=5,将数组下标为4的数值改为负数，即[-6,3,-4,-1,-5]，表示5已经出现过了
// 然后再遍历数组，遇到的第一个大于零的数，证明该下标位置的数字没有出现过，返回下标+1
// 否则证明1-n的数都出现过，返回n+1即可。
int minNumberDisappeared(vector<int>& nums) {
    int len = nums.size();
    for (int &n : nums) {
        if (n <= 0) n = len+1;
    }
    for (int &n : nums) {
        if (abs(n) <= len) {
            // 一直都是负数
            nums[abs(n)-1] = -1*abs(nums[abs(n)-1]);
        }
    }
    for (int i = 0; i < len; ++i) {
        if (nums[i] > 0) return i+1;
    }
}

// 448 easy 数组中长度为n，每个数范围为 [1,n]，有些数出现了多次，找出[1,n]中所有没有出现过的数。
// 根据题意，设新数组b为 [1,2,...,n]。原数组 nums[i] 对应的数就应该位于数组b的 nums[i]-1 位置上
// 让其为负，表示数组b的这个位置上的数已经出现过。
vector<int> findDisappearedNumbers(vector<int>& nums) {
    for (int i = 0; i < nums.size(); i++) {
        int x = abs(nums[i])-1;  // 由于出现过一次的数会置符，所以要加绝对值
        nums[x] = nums[x] > 0 ? -nums[x] : nums[x];   
    }
    vector<int> ret;
    for(int i = 0; i < nums.size(); i++)
        if(nums[i] > 0) ret.push_back(i+1);
    return ret;
}

// 442 medium 大小为n的数组的所有整数都在范围 [1,n] 内，且每个整数出现一次或两次。请你找出所有出现两次的整数
// 如果按照448的思路，为正表示从没在 [1,n] 中出现的数，第二次为负表示出现两次
vector<int> findDuplicates(vector<int>& nums) {
    vector<int> ret;
    for (int i = 0; i < nums.size(); i++) {
        int x = abs(nums[i])-1;
        if (nums[x] > 0) {
            nums[x] = -nums[x];
        } else {
            ret.emplace_back(x+1);
        }
    }
    return ret;
}




