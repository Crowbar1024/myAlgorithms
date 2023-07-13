#include "myheader.h"

/*
    单调队列主要是为了求滑动窗口最大/最小值。
    
    单调队列是deque。具体而言，我们会在单调队列的队尾pop和append，会在队首pop。

    从头到尾递减：可以求滑动窗口内的最大值。
    原理分析：首先规定队首的元素是我们需要的滑动窗口内的最大值。
    1 在入队时，若该元素nums[i]比队尾元素小，直接从队尾入队仍能保持单调性。
              若该元素nums[i]比队尾元素大，那么要将队尾元素不停pop，直到队尾元素比该元素大（满足单调性），将该元素从队尾入队。
    2 当队头已经不在滑动窗口时（i-k+1是滑动窗口左边界），队首出队。
    4 当形成一个窗口时就收集答案。
*/

// hard 239 有一个大小为k的滑动窗口从数组nums的最左侧移动到数组的最右侧。
// 板子题
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq;
    for (int i = 0; i < k; ++i) {
        while (!dq.empty() && nums[i] >= nums[dq.back()]) {
            dq.pop_back();
        }
        dq.emplace_back(i);
    }
    vector<int> ret{nums[dq.front()]};
    for (int i = k; i < nums.size(); ++i) {
        while (!dq.empty() && nums[i] >= nums[dq.back()]) {
            dq.pop_back();
        }
        dq.emplace_back(i);
        if (dq.front() <= i-k) {
            dq.pop_front();
        }
        ret.emplace_back(nums[dq.front()]);
    }
    return ret;
}

