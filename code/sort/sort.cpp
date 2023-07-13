#include "myheader.h"




// 冒泡排序，从后往前交换
// 空间复杂度O(1)
// 和插入顺序相反
void bubbleSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) return;
	for (int end = nums.size() - 1; end > 0; end--)
		for (int i = 0; i < end; i++)
			if (nums[i] > nums[i + 1])    // 可以稳定
				swap(nums[i], nums[i + 1]);
}

// 插入排序
// 数组的前面部分是有序序列，每次找到有序序列后面的第一个元素（待插入元素）的插入位置，将有序序列中的插入位置后面的元素都往后移动一位，然后将待插入元素置于插入位置。
void insertionSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) return;
	for (int i = 1; i < nums.size(); i++) {
		// 下面这个过程就是插入
		for (int j = i - 1; j >= 0; j--) {
			if (nums[j] > nums[j + 1]) {    // 可以稳定
				swap(nums[j], nums[j + 1]);
			}
		}
	}
}

// 选择排序 把后面最小的和当前节点交换
// 空间复杂度O(1)
void selectionSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) return;
	// 边界值判断，因为最后一次判断了数组最后两个数的最小值，最后一次不用判定
	for (int i = 0; i < nums.size() - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < nums.size(); j++) {
			// 破坏了稳定性，所以这个算法实现不了稳定性
			minIndex = (nums[minIndex]>nums[j]) ? j : minIndex;
		}
		swap(nums[i], nums[minIndex]); 
	}
}


// 归并排序 三个部分
// 由于需要一个临时数组存放，再merge，所以空间On
void merge(vector<int> &nums, int l, int r, int mid) {
	vector<int> tmp(r - l + 1, 0);
	int i = 0, p1 = l, p2 = mid + 1;
	while (p1 <= mid && p2 <= r) {
		tmp[i++] = (nums[p1] <= nums[p2]) ? nums[p1++] : nums[p2++];    // 可以稳定，如果<就变成了不稳定
	}
	while (p1 <= mid) {
		tmp[i++] = nums[p1++];
	}
	while (p2 <= r) {
		tmp[i++] = nums[p2++];
	}
	for (i = l; i <= r; i++) {
		nums[i] = tmp[i - l];
	}
}

void mergeSortProcess(vector<int> &nums, int l, int r) {
	if (l == r) {
		return;
	}
	int mid = l + ((r - l) >> 1);
	mergeSortProcess(nums, l, mid);
	mergeSortProcess(nums, mid + 1, r);
	merge(nums, l, r, mid);
}

void mergeSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) {
		return;
	}
	mergeSortProcess(nums, 0, nums.size() - 1);
}


// 非递归的归并排序
void mergeSortWithoutRecursion(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) {
		return;
	}
	int len = nums.size();
	int bar = 1;
	while (bar < len) {
		for (int i = 0; i<len; i += bar * 2) {
			int l = i, r = i + 2 * bar - 1;
			int mid = l + ((r - l) >> 1);
			if ((len - i) <= bar) {    // 那块已经招不到与之匹配了，一直保留到最后一步
				break;
			}
			if ((len - i) < 2 * bar) {    // 很大一块与一小块
				r = len - 1;
				mid = i + bar - 1;
			}
			vector<int> tmp(r - l + 1, 0);
			int j = 0, p1 = l, p2 = mid + 1;
			while (p1 <= mid && p2 <= r) {
				tmp[j++] = (nums[p1] < nums[p2]) ? nums[p1++] : nums[p2++];
			}
			while (p1 <= mid) {
				tmp[j++] = nums[p1++];
			}
			while (p2 <= r) {
				tmp[j++] = nums[p2++];
			}
			for (j = l; j <= r; j++) {
				nums[j] = tmp[j - l];
			}
		}
		bar <<= 1;
	}
}



// 快速排序（改成非递归不会有好处，空间复杂度省不下去）（不能稳定，但论文级别可以做到01-stable）
// 时间复杂度分析
// 差的情况：Tn = Tn-1 + On; 好的情况：Tn = 2Tn/2 + On
// 快排常数项低，所以工业常用
// 不稳定排序

// 空间复杂度分析（断点的深度就是空间复杂度，因为递归的断点变量可以复用，同一层树的一个断点释放了，另一个可以复用）
// 好的情况：每次递归展开的断点记录的是中间的位置（与上面一致）
// 912 medium 排序 下面的快排被称为三路快排，因为不需要交换相同值
vector<int> sortArray(vector<int>& nums) {
	if (nums.size() <= 1) return nums;
	quickSort(nums, 0, nums.size()-1);  // 右边界可以取到
	return nums;
}
void quickSort(vector<int>& nums, int l, int r) {
	if (l >= r) return;  // 相等很好理解，大于就说明要么之前的[l,r]中都是大于等于的或者都是小于等于的
	int randomIndex = l+rand()%(r-l+1);  // rand()返回一个[0,max]之间的随机数，所以取值为[l,r]
	swap(nums[r], nums[randomIndex]);  // 将划分数 pivot 放在末尾，每次就比较末尾
	// 最终期望得到 [l,lt] [lt+1,rt-1] [rt,r-1] [r] 小于部分，等于部分，大于部分，划分数。
	// 注意小于/大部分并不是有序的，所以是不稳定的
	int lt = l-1;  // 因为小于部分[l,lt]可能不存在，所以lt这样初始化
	int rt = r;  // 因为大于部分[rt,r-1]可能不存在，所以rt这样初始化
	for (int cur = l; cur < rt; ) {
		// 把小于部分右边界的右边和cur交换，但由于cur始终在lt之前，所以cur左边的数必然小于等于划分数，所以可以前进
		if (nums[cur] < nums[r]) swap(nums[cur++], nums[++lt]);
		// 把大于部分左边界的左边和cur交换，因为不知道该值与划分数的大小关系，cur不动
		else if (nums[cur] > nums[r]) swap(nums[cur], nums[--rt]);
		else cur++;
	}
	swap(nums[rt], nums[r]); // 让划分数回到等于部分，即让大于部分的第一个交换以下，即使大于部分存在，也只是原地交换
	// 四部分变成了三部分 [l,lt] [lt+1,rt] [rt+1,r]
	quickSort(nums, l, lt);
	quickSort(nums, rt+1, r);
}


// 堆排序（系统上的堆：递归堆栈的大小；数据结构上的堆是棵完全二叉树，小根堆和大根堆）
// 大顶堆：子节点的值都大于等于当前节点。用处，建堆之后就可以O1获得最大值
// 工程上不常用：不稳定，常数项比较大
void heapSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) return;
	int n = nums.size();
	// 数组看成是完全二叉树，节点i的左右儿子是2i+1和2i+2。
	// 完全二叉树的性质：n0=n2+1，即叶子节点数是有两个儿子的节点数+1，
	// 而有一个儿子的节点只能是0或1，所以最后一个非叶子节点是int(n/2)-1
	for (int i = n/2-1; i >= 0; --i) heapify(nums, i, n); // 1 建堆：从最后一个非叶子节点开始构造大顶堆 On
	// 2 堆排序：让最大值放到数组最后（根据堆的性质，大顶堆无法判断最小值的位置，只知道最大值的位置），
	// 再调整堆，往复，得到了一个递增的数组 Onlogn
	for (int i = n-1; i >= 0; --i) {
		swap(nums[0], nums[i]); // 将堆顶，即最大的值，与末尾交换
		--n; // 不用遍历数组后面的升序序列
		heapify(nums, 0, n); // 从上往下更新大顶堆
	}
}
void heapify(vector<int> &nums, int i, int n) { // 调节节点i的子树，从上到下，使以i为根的子树是大顶堆
	int l = 2*i+1;
	while (l < n) {
		int largest = (l+1<n && nums[l+1]>nums[l]) ? l+1 : l; // 取左右儿子中最大的那个
		largest = nums[largest]>nums[i] ? largest : i; // 再与父亲比较
		if (largest == i) break; // 该子树符合大顶堆，不用交换
		swap(nums[i], nums[largest]); // 若大于父亲则与其交换
		i = largest;
		l = i*2+1; // 如果是建堆向上，那么一开始一次while就结束了。如果是较高高度发生交换，换下来的较小的节点可能不符合大顶堆的性质，需要继续向下交换
	}
}



// 桶排序分为2种
// 计数排序
// 限正数
void bucketSort(vector<int> &nums) {
	if (nums.empty() || nums.size() == 1) {
		return;
	}
	int bucketSize = nums[0];
	for (int i = 1; i < nums.size(); i++) {
		bucketSize = max(bucketSize, nums[i]);
	}
	vector<int> bucket(bucketSize + 1, 0);
	for (int i = 0; i < nums.size(); i++) {
		bucket[nums[i]]++;
	}
	int i = 0;
	for (int j = 0; j <= bucketSize; j++) {
		while (bucket[j]--) {
			nums[i++] = j;
		}
	}
}

int maxGap(vector<int> nums) {
	if (nums.empty() || nums.size() == 1) {
		return 0;
	}
	int maxNum = INT_MIN;
	int minNum = INT_MAX;
	int len = nums.size();
	for (int i = 0; i < len; i++) {
		maxNum = max(maxNum, nums[i]);
		minNum = min(minNum, nums[i]);
	}
	if (maxNum == minNum) {
		return 0;
	}
	vector<int> haveNum(len + 1, 0), maxBucket(len + 1, 0), minBucket(len + 1, 0);
	int bid = 0;
	for (int i = 0; i < len; i++) {
		bid = (int)((nums[i] - minNum) * len / (maxNum - minNum));
		maxBucket[bid] = haveNum[bid] ? max(maxBucket[bid], nums[i]) : nums[i];
		minBucket[bid] = haveNum[bid] ? min(minBucket[bid], nums[i]) : nums[i];
		haveNum[bid] = 1;
	}
	int lastMax = maxBucket[0];
	int ret = 0;
	for (int i = 1; i <= len; i++) {
		if (haveNum[i]) {
			ret = max(ret, minBucket[i] - lastMax);
			lastMax = maxBucket[i];
		}
	}
	return ret;
}

// 同理可以生成随机字符串
// rand() % (b-a) + a: [a,b)
vector<int> generateRandomArray(int size, int bound) {
	int trueSize = rand() % (size + 1);    // [0, size]
	vector<int> ret(trueSize);
	for (int i = 0; i<trueSize; i++) {
		ret[i] = rand() % (bound + 1);    // [0, bound]
	}
	return ret;
}

bool judgeEqual(vector<int> a, vector<int> b) {
	if ((a.empty() && !b.empty()) || (!a.empty() && b.empty())) {
		return false;
	}
	else if (a.empty() && b.empty()) {
		return true;
	}
	else if (a.size() != b.size()) {
		return false;
	}
	else {
		for (int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				return false;
			}
		}
	}
	return true;
}



int main() {
	srand((unsigned)time(NULL));
	int testTimes = 10, size = 20, bound = 20;
	bool isEqual = false;
	for (int i = 0; i<testTimes; i++) {
		vector<int> arr1 = generateRandomArray(size, bound);
		// int len = arr1.size();
		// vector<int> arr2 = arr1, arr3 = arr1;    // 没涉及指针和引用时浅拷贝和深拷贝没区别
		// bucketSort(arr1);
		int gap = maxGap(arr1);
		printArray(arr1);
		sort(arr1.begin(), arr1.end());
		printArray(arr1);
		printf("%d\n", gap);
		// sort(arr2.begin(), arr2.end());
		// isEqual = judgeEqual(arr1, arr2);
		// if (!isEqual) {
		// 	printArray(arr3);
		// 	printArray(arr1);
		// 	printArray(arr2);
		// 	break;
		// })
	}
	printf("%s\n", isEqual ? "testRight" : "Wrong");
	system("PAUSE");
	return 0;
}