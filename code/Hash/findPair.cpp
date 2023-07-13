#include "myheader.h"



// 350 easy 返回两数组的交集。
// 返回结果中每个元素出现的次数，应与元素在两个数组中都出现的次数一致（如果出现次数不一致，则考虑取较小值）。可以不考虑输出结果的顺序。
// 最简单的思路，用一个哈希表记录第一个数组每个元素出现的次数，然后遍历第二个数组，把交集的数次数-1
// 同时为了减少空间复杂度，把哈希表用于长度段的数组
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) return intersect(nums2, nums1);
    unordered_map<int, int> m;
    for (const int& n : nums1) m[n]++;
    vector<int> ret;
    for (const int& n : nums2) {
        if (m.count(n) && m[n] > 0) {
            m[n] -= 1;
            ret.emplace_back(n);
        }
    }
    return ret;
}
// easy 349 如果结果要求不重复即unique，就用set统计
// 如果给定的数组已经排好序呢？你将如何优化你的算法？—— 双指针分别指向数组的开头
// 如果 nums1 的大小比 nums2 小，哪种方法更优？——原解法，不过我已经考虑这个问题
// 如果 nums2 的元素存储在磁盘上，内存是有限的，并且你不能一次加载所有的元素到内存中，你该怎么办？
// ——写到多个小文件，然后小文件归并到大文件（排好序），然后双指针遍历


// 1346 easy 查看数组是否存在2倍关系的数，时间要求On
// 正常思路是存进set找，但两个0也算，所以得用map
// 如果不想一开始就全部遍历，需要考虑*2和/2，但/2要考虑奇偶，嫌麻烦，干脆还是先遍历一遍。
bool checkIfExist(vector<int>& arr) {
    unordered_map<int, int> vis;
    for (const int& n : arr)  vis[n] += 1;
    for (const int& n : arr)  {
        int cnt = vis.count(n*2);
        if ((cnt && n) || (!n && vis[0] > 1)) return true;
    }
    return false;
}


// 219 easy 一个数组，是否存在下标i,j 使i,j的绝对差最大为k时，存在nums[i]==nums[j]
// 用一个size为k的窗口，然后每次让新的元素与这个窗口中做匹配，查看是否有值重合
bool containsNearbyDuplicate(vector<int>& nums, int k) {
    unordered_set<int> s;
    for (decltype(nums.size()) i = 0; i != nums.size(); ++i) {
        auto it = s.insert(nums[i]);
        if (!it.second) return true;  // 大小为k的set插入失败，说明插入的这个与前面的有相同的值
        if (s.size() > k) s.erase(nums[i-k]);
    }
    return false;
}
// 220 hard 类似219，一个数组，是否存在下标i,j 使i,j的绝对查至多为k时，存在nums[i]和nums[j]的绝对差至多是t
// 可以这么理解，有个size为k的窗口，出现新的nums[i]时，计算出nums[i]与窗口中某个数的绝对差如果小于等于t，那么返回true
// 设存在这个数x，则|x-nums[i]| <= t 则 -t + nums[i] <= x <= t + nums[i]
// 由于这里需要进行查找，不能使用哈希版本的set
bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
    set<int64_t> s;  // -2^31 <= nums[i] <= 2^31 - 1 所以加减t后可能越界
    int64_t t_64 = static_cast<int64_t>(t);
    for (decltype(nums.size()) i = 0; i != nums.size(); ++i) {
        int64_t numsi_64 = static_cast<int64_t>(nums[i]);
        auto x = s.lower_bound(numsi_64 - t_64);
        if (x != s.end() && *x <= t_64 + numsi_64) return true;
        s.insert(nums[i]);
        if (s.size() > k) s.erase(nums[i-k]);
    }
    return false;
}



// 532 medium 两数之差为k，返回这样的对数，需要注意的是{1,1,5}4，只能有一个{1,5}。但{1,1,5}0，有一个{1,1}
// 为避免自己查自己，使用map统计次数。为防止重复，每次清空次数。
// 虽然是查pair，但每次消掉自己，且对+k和-k的做记录，能保证数对个数不变
int findPairs(vector<int>& nums, int k) {
    unordered_map<int,int> cnt;
    int ret = 0;
    for (const int& n : nums) cnt[n] += 1;
    for (const int& n : nums) {
        if (cnt[n] == 0) continue;  // 之前记录过了
        if (k == 0) {  // 自己重复
            if (cnt[n] > 1) ++ret;
        } else {
            if (cnt[n-k]) ++ret;
            if (cnt[n+k]) ++ret;
        }
        cnt[n] = 0;
    }
    return ret;
}

