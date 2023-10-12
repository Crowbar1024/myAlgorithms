#include "myheader.h"

// 307 medium 给你一个数组 nums，请你完成两类查询。
// 其中一类查询要求更新 nums 下标对应的值
// 另一类查询要求返回 nums 中 [left, right] 的元素之和
// 经典线段树
// 线段树是一种非常灵活的数据结构，它可以用于解决多种范围查询问题，比如在对数时间内从数组中找到最小值、最大值、总和、最大公约数、最小公倍数
// 一般用数组实现简单易懂，举个例子，数组[0,5]
// 叶子节点[6,11]，下面对父节点统计
// [4,5]st[5] = st[10]+st[11], [2,3]st[4] = st[8]+st[9], [0,1]st[3] = st[6]+st[7]
// [2,5]st[2] = st[4]+st[5]
// [0,5]st[1] = st[2]+st[3]
// 可见对于任意长度n的数组，叶子节点下标从n开始，头节点的下标就是1，即有n-1个非叶子节点
// 再想一想，如果n为奇数，那么第1个叶子节点的计算会跳到倒数第二层，并且还是右儿子
// 综上，奇数下标为右儿子，偶数下标为左儿子。
// 如果下标从0开始，应该会全部相反。
class NumArray {
    vector<int> st;  // segmentTree
    int n;
public:
    // 用整数数组 nums 初始化对象
    // 假设头节点的在数组中的下标是1，那么如果索引 i 处的元素不是一个叶节点，那么其左子节点和右子节点分别存储在索引为 2i 和 2i+1 的元素处。
    // 每个非叶子节点保存了[l,r]的信息
    // 从叶节点开始，用输入数组初始化它们。然后逐步向上移动到更高一层来计算父节点的和，直到最后到达线段树的根节点。
    // 我看官方使用resize来初始化的
    NumArray(vector<int> &nums) : n(nums.size()), st(vector<int>(2*n, 0)) {
        for (int i = n; i < 2*n; ++i) {
            st[i] = nums[i-n];
        }
        for (int i = n-1; i > 0; --i) {
            st[i] = st[2*i] + st[2*i+1];
        }
    }

    // 将 nums[index] 的值更新为 val
    // 当我们更新数组中某个索引 ii 处的元素时，我们需要重建线段树，因为一些树节点上的和值也会随之产生变化。我们将再次使用自下而上的方法。
    // 因为只有一个点变了，这个点的父亲节点无论如何都只会增加一个增量extra，所以一路往上就行了
    void update(int index, int val) {
        int pos = index + n;
        int extra = val - st[pos];  // 增值
        while(pos > 0){
            st[pos] += extra;
            pos >>= 1;
        }
    }


    // 返回 nums 中 [left, right] 的元素之和
    // 首先得到叶子下标[l,r]
    // 如果l是左节点，那么很好，l不用动
    // 否则，需要把下标l的值加上去，因为l的左兄弟不在范围内，需要缩小l的范围
    // 同理r是左儿子就要缩小r的范围
    int sumRange(int left, int right) {
        int l = left + n, r = right + n, sum = 0;
        while (l <= r) {
            if (l & 1) {
                sum += st[l];
                ++l;
            }
            if (!(r & 1)) {
                sum += st[r];
                --r;
            }
            l >>= 1;
            r >>= 1;
        }
        return sum;
    }
};