#include "myheader.h"


// 215 medium 第K大的数
// 类似的题目很多很多，题干要求On，这里采用快排
int findKthLargest(vector<int>& nums, int k) {
    return quickSort(nums, 0, nums.size()-1, k);
}
// 虽然感觉这是nlogn的，但《算法导论》9.2：期望为线性的选择算法证明是n
int quickSort(vector<int>& nums, int l, int r, int k) {
	if (l == r) return nums[l];  // 不存在大于的情况，会被return的if else 排除
	int randomIndex = l+rand()%(r-l+1);
	swap(nums[r], nums[randomIndex]);
	int lt = l-1;
	int rt = r;
	for (int cur = l; cur < rt; ) {
		if (nums[cur] > nums[r]) swap(nums[cur++], nums[++lt]);  // 第K大，说明逆序
		else if (nums[cur] < nums[r]) swap(nums[cur], nums[--rt]);
		else cur++;
	}
	swap(nums[rt], nums[r]);
    // 第k大，此时序号就是k-1
    if (k-1 >= lt+1 && k-1 <= rt) return nums[rt];
    else if (k-1 <= lt) return quickSort(nums, l, lt, k);
	else return quickSort(nums, rt+1, r, k);
}


// 75 medium 元素都是0/1/2，排成三堆一样的
// 借鉴快排的思想，两个屏障指针lt和rt
void sortColors(vector<int>& nums) {
    int lt = -1, rt = nums.size();
	for (int i = 0; i < rt; ++i) {
		// 同快排一样，右边界换过来的不知道大小，所以i不前进
		while (i < rt && nums[i] == 2) swap(nums[i], nums[--rt]);
		// 因为i前进的方向，保证i位置是1，所以i需要前进
		if (nums[i] == 0) swap(nums[i], nums[++lt]);
	}
}
