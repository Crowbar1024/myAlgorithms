#include "myheader.h"

// 860 easy 每一杯柠檬水的售价为 5 美元。顾客排队购买你的产品，一次购买一杯，然后向你付 5 美元、10 美元或 20 美元。
// 你必须给每个顾客正确找零，注意，一开始你手头没有任何零钱。判断能否给每位顾客正确找零
bool lemonadeChange(vector<int>& bills) {
    int d5 = 0, d10 = 0;  // 可以找出去的5和10的个数。不需要统计20块的，因为找不出去
    for (int bill : bills) {
        if (bill == 5) d5++;
        else if (bill == 10) {
            if (d5) d5--, d10++;
            else return false;
        } else {
            if (d10 && d5) d10--, d5--;
            else if (d10 && !d5) return false;
            else if (!d10 && d5 >= 3) d5 -= 3;
            else return false;
        }
    }
    return true;
}

// POJ 1017 1*1、2*2、3*3、4*4、5*5、6*6，填充6*6的箱子，a是不同小正方形的数量，求最少需要多少个箱子能装满小正方形
// a[5] 6*6：1个产品放在1个箱子里
// a[4] 5*5：1个产品要占用1个箱子，用1*1的箱子可以填充（11个填满1箱）
// a[3] 4*4：1个产品要占用1个箱子，剩余空间用2*2和1*1的箱子填充（先填充2*2（最多5个填满），再填充1*1）
// a[2] 3*3：4个产品可以填满1个箱子，假如有不满1个箱子的，分情况用1*1和2*2的产品填满
// a[1] 2*2：9个产品可以填满1个箱子，假如有不满1个箱子的，用1*1的产品填充
// a[0] 1*1：36个产品可填满一个箱子
int roundUp(int m, int n) { return (m+n-1)/n; } // 向上取整
int filling(vector<int> a) {
	int ret = a[5] + a[4] + a[3] + roundUp(a[2], 4);  // 前三大的只要一个就可以新开一个箱子，接下来由于3*3对4向上取整，所以余数需要用2*2和1*1填充
	int b[4] = {0, 5, 3, 1};  // 3*3的箱子剩1个，还需要5个2*2；剩2个，需要3个2*2；剩3个，剩1个2*2
	int left2 = 5*a[3] + b[a[2]%4]; // 和4*4装满的2*2数量，和3*3后空出来的可以装2*2的数量
	if (a[1] > left2)  // 4*4和3*3剩下的可以用2*2填满
		ret += roundUp(a[1]-left2, 9);  // 剩余的2*2用来填框
	int left1 = ret*36 - a[5]*36 - a[4]*25 - a[3]*16 -a[2]*9 - a[1]*4;
	if (a[0] > left1)
		ret += roundUp(a[0]-left1, 36);  // 作为补充的1*1用完后填空框
	return ret;
}



// 21/4/25字节笔试 easy
// 一组数，他们的平均数满足整数的条件。问最少多少次相邻两个数加减一次，使得所有数一样
// 比如 9 11 9 11，2次；1 10 10 19，3次
// 因为最后必然都是平均数，直接从头遍历，大了放到下一个，小了从下一个拿，遍历到n-1
// 即使是 0 100 3 1 这种极端情况，也成立
int adjas(vector<int>& nums) {
    int n = nums.size(), ret = 0;
    int avg = accumulate(nums.begin(), nums.end(), 0)/4;
    for (int i = 0; i < nums.size()-1; ++i) {
        if (nums[i] < avg) {
            nums[i+1] -= avg-nums[i];
            ++ret;
        } else if (nums[i] > avg) {
            nums[i+1] += nums[i]-avg;
            ++ret;
        }
    }
    return ret;
}

// 现在要让所有人在夜晚从A跑到B，花费时间cost，路上必须有蜡烛亮着，只有一根蜡烛。
// 规则是，每次让两个人过去，然后让对岸的一个人拿蜡烛回来。过去的时间取决于最慢的人，问最短让所有人到B点的时间
// 需要让每次回来的时间短，所以让最快的搭配最慢的两个，1 2 ... 10 12
// 但如果是 1 2 ... 100 102，可不可以让最慢的一起走，这样可以节省一半的时间。所以引入次快的
// 1 最快搭配最慢，最快回来，然后最快搭配次慢，最快回来：cost[0]*2+cost[n-1]+cost[n-2]
// 2 最快搭配次快，最快回来，然后最慢搭配次慢，次快回来：cost[1]*2+cost[0]+cost[n-1]
int carriage(vector<int> cost) {
	sort(cost.begin(), cost.end());
	int n = cost.size();
	int sum = 0;
	while(n) {
		if (n == 1) {  // 只剩一匹马，过去就行
			sum += cost[0];
			break;
		} else if (n == 2) {  // 两匹马
			sum += cost[1];
			break;
		} else if (n == 3) {  // 三匹马，无论什么顺序，花费时间都一样
			sum += cost[0]+cost[1]+cost[2];
			break;
		} else {
			if (2*cost[1] < cost[0]+cost[n-2])  // 方案2
				sum += cost[1]*2+cost[0]+cost[n-1];
			else sum += cost[0]*2+cost[n-1]+cost[n-2];  // 方案1
			n -= 2;
		}
	}
	return sum;
}


// 1247 medium 字符串长度相等，只能互相交换，不能自己交换，问最少交换次数
// 首先判断能不能最后相同，如果x->y y->x之和为奇数，则最终依然会剩1个，所以先要判断，
// 然后就是一个找规则的事情了，先把x->y的全变成一样的，再考虑最后剩下的1个。
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



// 376 medium 如果连续非负数之间的差严格地在正数和负数之间交替，则数字序列称为摆动序列。
// 第一个差（如果存在的话）可能是正数或负数。仅有一个元素或者含两个不等元素的序列也视作摆动序列。
// 返回nums中作为摆动序列的最长子序列的长度。
// 因为是子序列，想象一下，递增的串上只有头和尾就行了，而头和尾就是摆动点，即求摆动点的数量
// 引入初始哨兵，这样就有一个问题，2 5 2，5 2 5，哨兵的长度如何给定？所以直接设置为0
int wiggleMaxLength(vector<int>& nums) {
    int curDiff = 0, preDiff = 0; // 一个点是摆动点的前提是这个点和前点的差，前点和前前点的差，这两者异号
    int ret = 1;  // 设置nums[-1]是负数，那么nums[0]是峰，长度初始化为1，这也解释为什么preDiff是正数，1是随意取的
    for (int i = 1; i < nums.size(); ++i) { // 从nums[1]开始判断是否为摆动点
        curDiff = nums[i] - nums[i-1];
        // 0就是涉及到初始哨兵的情况，这样即使一共只有2个不同的点，也能保证全都是摆动点
        if ((curDiff > 0 && preDiff <= 0) || (curDiff < 0 && preDiff >= 0)) {
            ++ret;
            preDiff = curDiff;
        }
    }
    return ret;
}