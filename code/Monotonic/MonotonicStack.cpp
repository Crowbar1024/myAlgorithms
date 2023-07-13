#include "myheader.h"

/*
    单调栈是指栈内元素（栈底到栈顶）都是（严格）单调递增或者单调递减的。

    如果有新的元素入栈，栈调整过程中会将所有破坏单调性的栈顶元素出栈，并且出栈的元素不会再次入栈。
    
    由于每个元素只有一次入栈和出栈的操作，所以单调栈的维护时间复杂度是O(n) 。

    单调递增时，如果当前元素 cur < top()，那么插入栈顶；
    否则，可以遍历找到第一个比 cur 小的数，停止查找，将 cur 插入栈顶。在这次遍历中，出栈的元素单调递减。

    同理，也可以找到最后一个小于 cur 的数，通过从后往前遍历，见962。
*/

// 739 medium 求下一个比当前元素大的元素的位置
vector<int> dailyTemperatures(vector<int>& temperatures)
{
    int n = temperatures.size();
	vector<int> ret(n, 0); // 找不到就返回0
	stack<int> monoStack; // 单调递减，存的是下标
	for (int i = 0; i < n; ++i) {
		while (!monoStack.empty() && temperatures[monoStack.top()] < temperatures[i]) { // 将所有破坏单调递减的栈顶元素出栈
			ret[monoStack.top()] = i - monoStack.top();
			monoStack.pop(); // 对于每一个元素，当它出栈的时候，说明它遇到了自己的next greater element
		}
		monoStack.emplace(i);
	}
	return ret;
}

// 503 medium 类似739，也是找下一个大的，但是是循环数组
vector<int> nextGreaterElements(vector<int>& nums) {
    int n = nums.size();
	vector<int> ret(n, -1); // 找不到就返回-1
	stack<int> mns;
	for (int i = 0; i < n * 2 - 1; ++i) { // n-1的下一个的右边界就是n-2
		while (!mns.empty() && nums[mns.top()] < nums[i % n]) {
			ret[mns.top()] = nums[i % n]; // 返回的是值
			mns.pop();
		}
		mns.emplace(i % n);
	}
	return ret;
}

// 42 hard 接雨水 棍子很粗，占满了格子，雨水落到棍子围成的空间
// 单调递减栈，这样遇到下一个高的，就会形成一个谷，可以填充水
int trap(vector<int>& height)
{
	int n = height.size();
    stack<int> mns; // 递减
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        while (!mns.empty() && height[mns.top()] < height[i]) {
            int h2 = height[mns.top()]; // 一定比两边小
            mns.pop();
            if (mns.empty()) break; // 拿 height[i] 作为下一次的栈顶
            int width = i - mns.top() - 1;
            int h = min(height[mns.top()], height[i]) - h2;
            ret += h * width; // 缺的会补上，一个矩形一个矩形加上去
        }
        mns.emplace(i);
    }
    return ret;
}

// 901 medium 计算nums[i]的左边小于该值的个数，必须是相邻nums[i]的子串
// 暴力：每次添加后往前扫描，On，一共On^2
// 很容易想到维护一个单调递减的栈，每次添加时更新该栈就行，一共On
class StockSpanner {
public:
    StockSpanner() {
        nums = vector<int>(1e4+5, 1);
        idx = 0;
    }
    
    // 因为是单调栈，比该值小的都已经弹出来了
    int next(int price) {
        nums[idx] = price;
        if (stk.empty() || nums[stk.top()] > price) {
            stk.emplace(idx++);
            return 1;
        }
        while (!stk.empty() && nums[stk.top()] <= price) { // 得到左边第一个大于price的值
            stk.pop();
        }
        int lidx = stk.empty() ? -1 : stk.top();
        stk.emplace(idx++);
        return idx - lidx - 1;
    }
private:
    vector<int> nums;
    stack<int> stk; // 单调递减
    int idx;
};

// 84 hard 直方图，最大矩阵面积
int largestRectangleArea(vector<int>& heights) {
    stack<int> monost; // 这次单调递增，还是下标
    int ret = 0;
    heights.emplace_back(0); // 避免一直递增，无法更新，最后放一个哨兵
    for (int i = 0; i < heights.size(); ++i) {
        while (!monost.empty() && heights[monost.top()] >= heights[i]) { // 遇到小的了，开始更新小的之前的最大矩阵面积
            int h = heights[monost.top()]; // 取递增直方块的最长边
            monost.pop();
            int len = monost.empty() ? i  : i - monost.top() - 1; // 因为之前出栈了，所以要-1
            ret = max(ret, h * len);
        }
        monost.emplace(i);
    }
    return ret;
}

// 85 hard 01矩阵中全是1组成的最大矩阵面积
// 利用84的代码，每次遍历一行，对直方图进行一次更新
int maximalRectangle(vector<vector<char>>& matrix)
{
    if (matrix.empty() || matrix[0].empty()) return 0;
    int n = matrix.size(), m = matrix[0].size();
    vector<int> heights(m, 0); // 直方图
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == '0') {
                heights[j] = 0; // 从这列断了，重新计量高度
            } else {
                heights[j] += 1;
            }
        }
        ret = max(ret, largestRectangleArea(heights));
        heights.pop_back(); // 删除哨兵0
    }
    return ret;
}

// 962 medium 若i<j，nums[i]<nums[j]，求最大的j-i
// 最简单的方法就是排序完扫描下标，不过需要注意lambda排的是下标数组，同时要捕获到相关的原值数组
// 如果先入为主用单调递增栈，这种情况无法处理{2,3,4,1,2}，1之前的都会被弹出来。
// 那么令其单调递减，并且建完栈之后，从后往前扫描。
int maxWidthRamp(vector<int>& nums) {
    stack<int> stk;
    int ret = 0, n = nums.size();
    for (int i = 0; i < n; ++i) {
        if (stk.empty() || nums[stk.top()] > nums[i]) { // 一定要从0开始
            stk.emplace(i);
        }
    }
    for (int i = n - 1; i >= 0 && !stk.empty(); --i) { // 为空了就直接推出退出
        while (!stk.empty() && nums[stk.top()] <= nums[i]) { // 找到最后一个小于等于当前值的位置
            ret = max(ret, i - stk.top());
            stk.pop();
        }
    }
    return ret;
}




// hard 一个由数组组成的环，如果数i和j之间有比两端小的数，可以视为i和j可以互相看见
// 相邻一定可以互相看见，求环上可以互相看见的对数
// 一个神奇的点：如果没有重复的数，环的长度为n，那么对数为2n-3
// 选择最高和次高两个点，其他任意一点，必然可以找到顺时针和逆时针恰好大于它的数，2(n-2)
// 然后此高和最高一定可以互相看见，1
// 然后是一般情况，根据上述的情况可以发现，其实是找每个元素左右恰好比它大的数的个数
// 使用单调栈
// 比如 5 2 2 2 3 4 1 2
// 单调栈的意思就是只能压入不大于栈顶的数，否则两者都要弹出
// 这里 5-2*3，压入3
// 那么2的右边恰好比他高的数是3，3个2之间两两互相看见，C(n)(2)，n是栈顶元素的个数
// 然后加上2n，因为每个2的左右恰好比其大的数是3和5
// 5-4-1，压入2，弹出 1 2
// 5-4，4的右边没有比他大的数，左边有恰好比他大的数（它下面），弹出，+1
// 5，5的右边没有，左边也没有
// 所以要选最大的数当栈底，不然万一一个小的数当栈底弹出了，不知道有没有左边比它大的数
// 还有一种情况：5*m-4*n-3k（都>1），弹出k个3，此时还剩5和4，说明3的左右有5和4，得+2k
// 5m-4n，弹出4，实际上4的两边都有5，+2n
int communications(vector<int> a) {
    if (a.empty() || a.size() < 2) return 0;
    int n = a.size();
    int maxIndex = 0;
    for (int i=1; i<n; ++i) {
        maxIndex = a[i]>a[maxIndex] ? i : maxIndex;
    }
    int maxValue = a[maxIndex];
    int index = (maxIndex+1)%n;
    stack<pair<int,int>> s;
    s.push({maxValue, 1});
    int ret = 0;
    while (index != maxIndex) {
        int newValue = a[index];
        while (s.top().first < newValue) {  // 把小于新的数的全部弹出
            int times = s.top().second;
            s.pop();
            ret += times*(times-1)/2 + times*2;
        }
        if (s.top().first == newValue) {
            s.top().second += 1;
        } else {
            s.push({newValue, 1});
        }
        index = (index+1)%n;
    }
    while (!s.empty()) {  // 此时是从大到小的单调栈
        int times = s.top().second;
        s.pop();
        ret += times*(times-1)/2;  // 内部
        if (!s.empty()) {
            ret += times;
            if (s.size() > 1) {
                ret += times;
            } else {
                ret += s.top().second>1 ? times : 0;
            }
        }
    }
    return ret;
}
