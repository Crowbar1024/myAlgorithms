#include "myheader.h"

// 我认为双指针问题有很多适用场景
// A 若干个有序数组，这种情况有点像滑动窗口
// B 无序数组，进行排列


// 167 升序数组元素唯一，一定可以找到两个数的和是target，那么返回任意的两个
vector<int> twoSum(vector<int>& numbers, int target) {
    int l = 0, r = numbers.size() - 1;
    while (l < r && numbers[l] + numbers[r] != target) {
        if (numbers[l] + numbers[r] > target) r--;
        else l++;
    }
    return vector<int>{l + 1, r + 1};
}


// 532 medium 两数之差为k，返回这样的对数，需要注意的是{1,1,5}4，只能有一个{1,5}。但{1,1,5}0，有一个{1,1}
// 双指针，因为排序后，{l,r}都是从左往右走，如果r找到了，l需要走到下一个非重复的位置，同时保证L<r
// 所以边界调节比较复杂
int findPairs(vector<int>& nums, int k) {
    sort(nums.begin(), nums.end());
    int ret = 0, n = nums.size();
    for (int l = 0, r = 1; l < n-1; ++l) {
        if (l == 0 || nums[l] != nums[l-1]) {  // 跳过l相等的情况
            // 小于才能走，如果写!=，r容易
            while (r<n && (l>=r || nums[r]-nums[l]<k)) ++r;
            // {1,1,3,4,5}0 当l=3，r=4时，上面的while触发不了，所以相等的条件一定得写
            if (r < n && nums[r]-nums[l] == k) ++ret;
        }
    }
    return ret;
}
// 用哈希表的话，也是一道经典例子，见HASH文件夹


// 977 非降序排列的数组，使其平方后再排序
// 当然可以先平方消耗On，再排序消耗Onlogn
// 但显然会有更好的解，只能是On，那么只能是双指针 A场景
// 虽然这不是纯粹的双指针，左边的不动而已
vector<int> sortedSquares(vector<int>& A) {
    int tail = A.size() - 1;
    while (tail > 0) {
        if (abs(A[tail]) > abs(A[0])) A[tail] = A[tail] * A[tail];
        else A[tail - 1] = exchange(A[tail], A[0] * A[0]);
        tail--;
    }
    A[0] = A[0] * A[0];
    return A;
}



// 1089 easy 给你一个长度固定的整数数组 arr，请你将该数组中出现的每个零都复写一遍，并将其余的元素向右平移。
// 双指针，分别指向旧数组和新数组的末尾，用新指针去重写数组，注意只有新指针指向原来数组时才有写操作
// 新指针每次往前跳时，需要判断是否进入了原数组长度，这样就能重写了
void duplicateZeros(vector<int>& arr) {
    int l1 = arr.size();
    int l2 = l1 + count(arr.begin(), arr.end(), 0);
    for (int i = l1 - 1, j = l2 - 1; i >= 0, j >= 0; i--, j--) {
        if (j < l1) {
            arr[j] = arr[i];  // 任何数都要重写一次
        }
        if (arr[i] == 0) {
            j--;  // 新指针指0时，往向前多跳一下
            if (j < l1) {
                arr[j] = arr[i];  // 是0且进入范围后多重写一次
            }
        }
    }
}

// 905 easy 将 nums 中的的所有偶数元素移动到数组的前面，后跟所有奇数元素。
// 首尾不断交换元素就行


// 校赛 一串数，问有多少三元组可以组成三角形
// 技巧是，排序后，如果[l,r]满足条件，那么[l,r-1]的最短边都满足
int triangleCount(vector<int> nums) {
	int cnt = 0;
	sort(nums.begin(), nums.end());
	// i为最大边，l为最小边，r为中间边
	// 固定i，找[l,r]的组合
	for (int i=nums.size()-1; i>1; --i) {
		int l = 0, r = i-1;
		while (l < r) {
			if (nums[l]+nums[r] > nums[i]) {
				cnt += r-l;  // 三条边：[l:r-1], r, i
				r--;
			} else {
				l++;
			}
		}
	}
	return cnt;
}