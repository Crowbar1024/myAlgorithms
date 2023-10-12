#include "myheader.h"


// 658 medium 升序，找到k个与x最接近的数字，如果距离相同，小的优先，返回这k个数字
// 我的做法：很笨的利用lower_bound，然后用while去做
// while的思路就是我当前窗口的左值如果距离比它自己右滑后窗口右值的距离大，才能右移
// 实际上我已经解决了最困难的一步，就是滑动窗口的移动判断
// 所以这个问题就转换成了找窗口的左值，直接用二分去搜索它就完了
// 至于边界问题，已经有一定经验了，根据缩减情况，肯定是l<r，返回l，不需要进一步分析
vector<int> findClosestElements(vector<int>& arr, int k, int x) {
    int l = 0, r = arr.size() - k;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (x - arr[mid] > arr[mid + k] - x) l = mid + 1;
        else r = mid;
    }
    return vector<int>(arr.begin() + l, arr.begin() + l + k);
}

// easy（二分medium） 1539 严格升序正序列，请你找到这个数组里第 k 个缺失的正整数。
// [2,3,4,7,11], k = 5
// i  a[i] 缺失数p[i]
// 0   2   1
// 1   3   1
// 2   4   1
// 3   7   3
// 4   11  6
// 明显k在p[3]和p[4]之间，那么就是找到第一个大于等于 k 的 p[i]，返回 k-p[i-1]+a[i-1]
// 考虑边界情况：k < a[0]，返回k
// k > p[n-1]，返回 k-p[i]+a[i]
// 如果要构建数组 p，感觉得不偿失，因为这导致复杂度On，所以只在二分时进行判断
// 显然有 p[i]=a[i]-i-1，这样返回 k+i
// 但这样就无法考虑右边界，所以设置一个哨兵，即 r=arr[n-1]，这是不可能达到的
int findKthPositive(vector<int>& arr, int k) {
    if (k < arr[0]) return k;
    int n = arr.size(), l = 0, r = arr[n-1];
    while (l < r) {
        int mid = l+(r-l)/2;
        int x = mid>=n ? INT_MAX : arr[mid];  // 注意arr[mid]可能取不到，设置一个较大的数
        if (x-mid-1 < k) l = mid+1;
        else r = mid;
    }
    int ret = k+l;
    return ret;
}



// 300 medium 最长严格上升子序列的长度
// 有DP解法
// 核心：建立一个升序数组，每次把新的数放到大于等于这个数的位置上
// 如果要找非降序的话，就用upper_bound
// 注意这种二分的解法是找不到路径的，DP可以找到路径，虽然这样时间复杂度就上去了
int lengthOfLIS(vector<int>& nums) {
    vector<int> ret;
    for (int i = 0; i != nums.size(); i++) {
        auto it = lower_bound(ret.begin(), ret.end(), nums[i]);
        if (it == ret.end()) ret.push_back(nums[i]);
        else *it = nums[i];
    }
    return ret.size();
}
// DP解法O(n^2)
// 以i结尾的最长上升非连续子序列 dp[i] = max(dp[j]+1,dp[i]) for all nums[j] < nums[i]


// 287 medium 数组长度为n+1，是由[1,n]中的数组成（可以不用完），其中一个数字重复了多次，找到这个重复的数字
// 由于不一定用到了所有的n，不能用求和的形式去做
// 这是一道经典数组题，而且给出了[1,n]这个取值范围，那么试试二分，用[l,r]不断缩小的方法去找目标
// 有了mid，可以数出所有小于等于m的数，显然如果数量比mid大，那么目标在[l,mid]
// 否则，目标显然在[mid+1,r]
// 根据这个缩放条件，利用经验自然是l<r && return l
// 时间复杂度nlogn
int findDuplicate(vector<int>& nums) {
    int l = 1, r = nums.size() - 1;    // [1,n]
    while (l < r) {
        int mid = l + (r - l) / 2, cnt = 0;
        for (int x : nums) cnt += x <= mid;
        if (cnt > mid) r = mid;
        else l = mid + 1;
    }
    return l;
}
// 还可以用离散数学去做，他的时间复杂度是On，简单来说就是利用链表去检查环的起点的思想，可见142


 
// 154 hard 数组形式同153，即前进升序数组，分A,B段，A的都比B大，但是元素可以重复。用二分找最小值，
// 核心思想：不断更新[l,r]的取值范围，可以通过mid和任意一边界进行对比来缩短范围，这里选择r，所nums[r]要取得到
// 注意找到的并不一定是B的头
int findMin(vector<int>& nums) {
    int l = 0, r = nums.size() - 1;
    if (nums[l] < nums[r]) return nums[0];
    while (l < r) {
        int mid = l + ((r - l) >> 1);
        if (nums[mid] < nums[r]) r = mid;  // 这种情况和153不变，mid有可能是要找的值
        else if (nums[mid] > nums[r]) l = mid + 1;  // 这种情况和153要区分，153直接else，因为==情况在153不可能存在
        // 相等时有两种情况：
        // 1: 445('1'11)2 
        // 2: 3(3334'3'3)3
        // 即mid有可能出现在B的头的左边和右边，似乎难以倍速的缩放范围
        // 所以可以考虑常量级别的缩小[l,r]
        else r--;  // 同时，由于这个写法，最后返回的l并不一定是B的头
        // 因为如果是11131，第一次r--，就抛弃了B的头的情况
    }
    return nums[l];
}

// 4 hard 两者都是严格升序数组，找到两者合并后的中位数。因为要求时间复杂度Olog(n+m)，则必须使用二分。
// 为了避免奇偶导致的各种边界问题，将数组各个数之间加入'#'（如果为空，就只有一个'#'）
// 这样总共有2(m+n)+2个数，总的个数是偶数
// 假设中位数的线将两个数组给自一分为二，将划线定义为两个'#'，这样线的两边各有（m+n）个数
// 假设nums1的划线左边有i个，则四个部分的长度有
// (i) # (2m-i) 这样i的取值范围[0,2m]
// (j) # (2n-j)
// 根据中位数的性质：i+j = 2m+2n-i-j --> j = m+n-i 为了避免j是负数，由i的取值范围得n>=m
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int m = nums1.size(), n = nums2.size();
    if (m > n) return findMedianSortedArrays(nums2, nums1); // 避免j是负数
    int lo = 0, hi = 2*m; // 二分i
    double ret;
    while (lo <= hi) { // 相等时，res还没算出来，所以还得跑
        int i = lo + (hi - lo) / 2;
        int j = m + n - i;
        int l1 = (i == 0) ? INT_MIN : nums1[(i-1)/2]; // i部分的右边界，是数字。因为i部分的左边界是#（除了为空），所以一般是偶数
        int l2 = (j == 0) ? INT_MIN : nums2[(j-1)/2]; // j部分的右边界
        int r1 = (i == 2*m) ? INT_MAX : nums1[i/2]; // 2m-i的左边界
        int r2 = (j == 2*n) ? INT_MAX : nums2[j/2]; // 2n-j的左边界
        if (l1 > r2) hi = i - 1; // i的大小太大了，注意l1和r1比较没有意义
        else if (l2 > r1) lo = i + 1; // j的大小太大了，即i太小了
        else {
            ret = (min(r1, r2) + max(l1, l2)) / 2.0;
            break;
        }
    }
    return ret;
}


// 887 hard K个蛋，N层楼，问测试多少次找到使鸡蛋恰好破的楼层
// 经典的鸡蛋问题，基本做法是用递归，思路就是自顶向下，这样会搜索所有的情况
// 引入二分的思想，定义dp[i][j]:测试i次，有j个蛋可以确定的最大楼层高度
// 如果鸡蛋破了，可以测试dp[i-1][j-1]，即dp[i-1][j-1]以下的楼层（包括）
// 没破，可以测试dp[i-1][j]，即dp[i-1][j]往上的楼层（不包括）
// 综上：dp[i][j] = dp[i-1][j] + (dp[i-1][j-1] + 1)
// 最后的dp[i][K]会等于N，返回最后的i
// 状态压缩，上边的直接用外循环的上一个dp[j]，左上的直接用外循环的上一个dp[j-1]
// 由于左上的选择，这里要反向遍历
// 这道题也包含二分的思想，时间复杂度O(KlogN)
// 所以这道题的核心就是让dp的计算结果当成是二分的对象，比较考智商的题目
int superEggDrop(int K, int N) {
    vector<int> dp(K+1, 0);
    int i = 0;
    for (; dp[K] < N; i++) {
        for (int j = K; j > 0; j--) {
            dp[j] = dp[j] + dp[j-1] + 1;
        }
    }
    return i;
}

// 410 hard 把非负的数组分成m个连续字串，选择“m个字串中最大的字串和”最小的情况，返回这个最大值
// 和719很像，已知字串和的取值范围，那么显然可以采用二分法，这里二分这个最大字串和的值
// 那么结果就是当二分到某个值，找出能恰好小于等于这个值的字串的个数是m时，那么这个值就是要找的
// splitArrayHelp: 查看当前被二分的最大和是否能满足条件
// 返回true，有2种情况：
// 1：恰好有m个连续子串的最大和都小于等于target，target有可能取大也有可能恰好
// 2：不到m个就已经取满了所有满足条件的连续子串，显然target取大了
// 返回false，说明target小了
// 根据缩放的经验，这里l<r return l
int splitArray(vector<int>& nums, int m) {
    int r = accumulate(nums.begin(), nums.end(), 0);
    int l = *max_element(nums.begin(), nums.end());
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (splitArrayHelp(mid, nums, m)) r = mid;
        else l = mid + 1;
    }
    return l;
}
bool splitArrayHelp(int target, vector<int>& nums, int m) {
    int cnt = 1, total = 0;    // BUG1: cnt要初始化为1
    for (int n : nums) {
        total += n;
        if (total > target) {
            total = n;
            cnt++;
        }
        if (cnt > m) return false;
    }
    return true;
}


// 354 hard
// 矩阵覆盖问题，不可以旋转
// 先排序后二分，复杂度O(nlogn)
// 需要注意的是排序要让第一维相同的，第二维从高到低，否则后面使用lowerbound会增加无效的矩形
// 下面是除了写lambda的另一种方法
static bool comp(vector<int>&a, vector<int>&b){
    return a[0]<b[0] || (a[0]==b[0] && a[1]>b[1]);
}
int maxEnvelopes(vector<vector<int>>& envelopes) {
    sort(envelopes.begin(), envelopes.end(), [](const auto& x, const auto& y){ return x[0] < y[0] ||
        (x[0] == y[0] && x[1] > y[1]); });  // 注意写在sort里面时不需要捕获
    vector<int> d2;
    for (int i=0; i<envelopes.size(); ++i) {
        int y = envelopes[i][1];
        auto pos = lower_bound(d2.begin(), d2.end(), y);
        if (pos == d2.end()) {
            d2.emplace_back(y);
        } else {
            *pos = y;
        }
    }
    return d2.size();
}

// HDU 1007 平面内的点与点之间的最小距离
// 暴力On^2，On不太可能，那么就是Onlogn，用到分治
// 二叉树底层计算的距离都是临点的距离
struct point {
	double x;
	double y;
};
double minDis(vector<point>& v) {
    sort(v.begin(), v.end(), [](point& a, point& b){ return a.x<b.x; });
    double ret = find(0, v.size()-1, v);
    return ret/2;
}
double dist(int a, int b, vector<point>& v) {
	return sqrt((v[a].x-v[b].x)*(v[a].x-v[b].x) + (v[a].y-v[b].y)*(v[a].y-v[b].y));
}
double find(int l, int r, vector<point>& v) {
	if (l+1 == r) return dist(l, r, v); // 二叉树最底层，两个节点一个距离
	else if (l+2 == r) return min(dist(l,l+1,v), min(dist(l,r,v),dist(l+1,r,v))); // 二叉树最底层，三个节点三个距离
	int mid = (l+r) >> 1;
	double d = min(find(l, mid, v), find(mid+1, r, v)); // 左右分区的最短距离
    // 然后考虑跨区间的最短距离
    vector<int> cc; // 可能属于最小距离的点
	int cnt = 0;
	for (int i=l; i<=r; i++) // mid是左分区的右边界，那么把直线x距离小于d的放进cc才有意义
		if (v[mid].x-d <= v[i].x && v[i].x <= v[mid].x+d )
            cc.emplace_back(i);
    int cnt = cc.size();
	sort(cc.begin(), cc.end(), [v](int a, int b){ return v[a].y<v[b].y; }); // cc的节点根据y坐标排序
	for (int i=0; i<cnt-1; i++) {
		for (int j=i+1; j<cnt; j++) {
			if (v[cc[j]].y-v[cc[i]].y >= d) break; // 后面j会离i更远
			d = min(d, dist(cc[i], cc[j], v));
		}
	}
	return d;
}


// 百度笔试，2023春招补录，研发A卷2
// AC 个人感觉medium
// 给了一个[1:n]的排列，计算该排列中的排列数
// [4,2,1,3,5] 中有 {1},{2,1,3},{4,2,1,3},{4,2,1,3,5}
int getPermutationCount(vector<int>& arr) {
    int n = arr.size();
    vector<int> nums(n+1);
    vector<int> pos(n+1);
    for (int i = 1; i <= n; ++i) {
        nums[i] = arr[i-1];
        pos[nums[i]] = i;
    }
    int mid = find(nums.begin()+1, nums.end(), 1)-nums.begin(); // 1的位置
    int l = mid, r = mid, cnt = 1; // 1已经找到了
    for (int x = 2; x <= n && l >= 1 && r <= n; ++x) {
        int px = pos[x];
        if (px > r) r = px;
        else if (px < l) l = px;
        // 在[l,r]中说明不用更新
        if (r-l+1 == x) {
            cnt++;
        }
    }
    return cnt;
}

// 1231 hard 将数组分成K+1份，K保证不会大于等于数组长度，所以每份长度至少为1
// 选择其中最小的一份是自己的。如何分，让自己的最大？返回这个最大值
// 二分的对象是自己的那份，因为其上下界很容得到。
// 然后判断是否可行，这一步比较巧妙，因为有了最小份的大小，如果超过其大小的数量超过K+1，那么必然可行
int maximizeSweetness(vector<int>& sweetness, int k) {
    int sum = accumulate(sweetness.begin(), sweetness.end(), 0);
    if (!k) return sum;
    int r = sum / k; // 取得到
    int l = 0;
    while (l < r) { // 相等直接返回l就行
        int mid = l + (r - l + 1) / 2; // 避免l不收缩导致的循环
        if (canSplit(mid, sweetness, k + 1)) { // 能分，说明还能更大
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    return l;
}
bool canSplit(int minSum, vector<int>& nums, int n) {
    int count = 0, tmpSum = 0;
    for (int num : nums) {
        tmpSum += num;
        if (tmpSum >= minSum) {
            ++count;
            tmpSum = 0;
        }
    }
    return count >= n;
}



int minimumSwap(string s1, string s2) {
    int n = s1.size();
    int p1 = 0, p2 = 0;
    for (int i = 0; i < n; ++i) {
        if (s1[i] != s2[i] && s1[i] == 'x') {
            ++p1;
        } else if (s1[i] != s2[i] && s1[i] == 'y') {
            ++p2;
        }
    }
    if ((p1 + p2) & 1) {
        return -1;
    } if (!p1) {
        return p2 / 2;
    } else if (!p2) {
        return p1 / 2;
    }
    return (p1 + 1) / 2 + (p2 + 1) / 2;
}