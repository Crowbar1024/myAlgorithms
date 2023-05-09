#include "myheader.h"




// 376 medium 如果连续数字之间的差严格地在正数和负数之间交替，则数字序列称为摆动序列。
// 第一个差（如果存在的话）可能是正数或负数。仅有一个元素或者含两个不等元素的序列也视作摆动序列。
// 返回nums中作为摆动序列的最长子序列的长度。（可删）
// 关键：当前节点和前一个节点的差，前一个节点和前前一个节点的差，异号。比如 1 4 2，则当前节点可以作为摆动序列的一个节点
// 所以为了避免{2,5} {5,2}这种只有两个点的情况，默认第一个点double，直接删除第一个点，保留第0个点
int wiggleMaxLength(vector<int>& nums) {
    int curDiff = 0, preDiff = 0;
    int res = 1;  // 第一个点默认是峰
    for (int i = 1; i < nums.size(); ++i) {
        curDiff = nums[i] - nums[i-1];
        // preDiff为0是初始值的情况，赋值之后不为0
        if ((curDiff > 0 && preDiff <= 0) || (curDiff < 0 && preDiff >= 0)) {
            ++res;
            preDiff = curDiff;
        }
    }
    return res;
}







