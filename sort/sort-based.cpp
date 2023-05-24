#include "myheader.h"



// 找小和，基于归并排序
// 小和就是每个元素左边所有比他小的数的和，现在要求整个数组的小和的和
// 只要在归并上加一行代码就行
int mergeSort(vector<int> &arr) {
	if (arr.empty() || arr.size() == 1) {
		return 0;
	}
	return mergeSortProcess(arr, 0, arr.size() - 1);
}
int mergeSortProcess(vector<int> &arr, int l, int r) {
	if (l == r) {
		return 0;
	}
	int mid = l + ((r - l) >> 1);
	int sumLeft = mergeSortProcess(arr, l, mid);
	int sumRight = mergeSortProcess(arr, mid + 1, r);
	int sum = sumLeft + sumRight + merge(arr, l, r, mid);
	return sum;
}
// 比如2 4 5   1 5 9两个序列合并
// 左右指针开始滑动，右指针先遍历到5，然后可以发现，2<5，即右指针的包括自己和右边的所有数都大于2
// 利用这个性质，可以快速计算两个序列的小和
int merge(vector<int> &arr, int l, int r, int mid) {
	vector<int> tmp(r - l + 1, 0);
	int sum = 0;
	int i = 0, p1 = l, p2 = mid + 1;
	while (p1 <= mid && p2 <= r) {
		if (arr[p1] < arr[p2]) {
			sum += arr[p1] * (r - p2 + 1);    // 就加了一行代码
			tmp[i++] = arr[p1++];
		}
		else {
            tmp[i++] = arr[p2++];
		}
	}
	while (p1 <= mid) {
		tmp[i++] = arr[p1++];
	}
	while (p2 <= r) {
		tmp[i++] = arr[p2++];
	}
	for (i = l; i <= r; i++) {
		arr[i] = tmp[i - l];
	}
	return sum;
}
// 衍生问题：
// 找逆序对。因为组间的逆序对个数在合并时不会重复计算


// 利用快排的思想找第K大的数
// 因为不是第k小，所以得调换顺序

vector<int> parititionK(vector<int> &arr, int l, int r) {
	int less = l - 1;    // 大于划分数的数组右边界
	int more = r;    // 小于划分数的数组左边界，为了让划分数不影响，不考虑划分数
	// 由于quickSortProcess的操作，划分值一直呆在arr[r]，所以r得保持不变
	// 所以便利的顺序只能从左往右，下标为l
	while(l < more) {
		if (arr[l] > arr[r]) swap(arr[l++], arr[++less]);    // 从左向右遍历的时候已经让l左边的都小于划分数了
		else if (arr[l] < arr[r]) swap(arr[l], arr[--more]);
		else l++;  // 这里让less不动，为的是记录等于划分数的部分
	}
	swap(arr[more], arr[r]);    // 处理划分数
	// 这样数组从[less + 1, more]开始，左右都是小于等于和大于划分数的数，乱序
	return vector<int>{less + 1, more};    // [less+1, more]是等于划分数arr[r]的部分
}

int quickSortProcessK(vector<int> &arr, int l, int r, int K) {
	if (l < r) {
		int randomIndex = l + rand()%(r-l+1);
		swap(arr[r], arr[randomIndex]);  // 让这个随机数成为划分值，并把它放到数组末尾
		vector<int> bound = parititionK(arr, l, r);
		int cntLeft = bound[0], len = bound[1]-bound[0]+1;
		if (cntLeft <= K-1 && cntLeft >= K-len) return bound[0];  // 找到了
		else if (cntLeft > K-1) return quickSortProcessK(arr, l, bound[0]-1, K);
		else return quickSortProcessK(arr, bound[1]+1, r, K);
	}
}

void quickSortProcessK(vector<int> &arr, int l, int r, int K, int& tar) {
	if (l < r) {
		int randomIndex = l + rand()%(r-l+1);
		swap(arr[r], arr[randomIndex]);  // 让这个随机数成为划分值，并把它放到数组末尾
		vector<int> bound = parititionK(arr, l, r);
		int cntLeft = bound[0], len = bound[1]-bound[0]+1;
		if (cntLeft <= K-1 && cntLeft >= K-len) tar = bound[0];  // 找到了
		else if (cntLeft > K-1) quickSortProcessK(arr, l, bound[0]-1, K, tar);
		else quickSortProcessK(arr, bound[1]+1, r, K, tar);
	}
}

int findKth(vector<int> a, int n, int K) {
	int tar = 0;
	quickSortProcessK(a, 0, a.size()-1, K, tar);
	return tar;
}


// 1051 easy 原数组经过非降序排序后，返回新数组与原数组每个位置上不同的数的个数
// 最简单的思路就是时间Onlogn，加空间On
// 然后发现：数组长度不到100，数组每个数都不到100
// 那就可以用空间换时间的，达到On
// 设置大小为101的桶，每个桶存放对应数的数量，这样非空桶的排序就是新数组
// 然后就是让遍历非空桶和原数组
int heightChecker(vector<int>& heights) {
    vector<int> bucket(101, 0);
    for (int n : heights) bucket[n]++;
    int ret = 0, cur = 0;
    for (int n : heights) {
        while (bucket[cur] == 0) cur++;
        if (cur != n) ret++;
        bucket[cur]--;
    }
    return ret;
}