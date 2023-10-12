#include "myheader.h"

// NC129 给定一个非负整数n，返回阶乘末尾为0的数量。
// 末尾是0，意味着因式分解后有5的阶乘和2的阶乘
// 阶乘意味着2的个数肯定比5多，所以只要统计前者
// 再分解，25的阶乘和4的阶乘……
long long thenumberof0(long long n) {
    long long zeroNum = 0;
    while (n) {
        zeroNum += n / 5;
        n /= 5;
    }
    return zeroNum;
}

// NC129的扩展，一堆数乘积的末尾为0的数量
// 同理，5*2，25*4，125*8
int thenumberof0(vector<int> nums) {
    int cnt2=0, cnt5=0;
	for (int i = 0; i < nums.size(); ++i) {
		while (nums[i]%2 == 0) {
			nums[i] /= 2;
			cnt2++;
		}
		while (nums[i]%5 == 0) {
			nums[i] /= 5;
			cnt5++;
		}
	}
	int ret = min(cnt2, cnt5);
	return ret;
}

// 1~n中包含1的个数
// 例如， 1~13 中包含 1 的数字有 1、10、11、12、13 因此共出现 6 次
// 设N = abcde ,其中abcde分别为十进制中各位上的数字。
// 如果要计算百位上1出现的次数，它要受到3方面的影响：百位上的数字，百位以下（低位）的数字，百位以上（高位）的数字。
// 1 如果百位上数字为0，百位上可能出现1的次数由更高位决定。
// 比如：12013，则可以知道百位出现1的情况可能是：100~199，1100~1199,2100~2199，，...，11100~11199，一共1200个。
// 可以看出是由更高位数字（12）决定，并且等于更高位数字（12）乘以 当前位数（100）。
// 2 如果百位上数字为1，百位上可能出现1的次数不仅受更高位影响还受低位影响。
// 比如：12113，则可以知道百位受高位影响出现的情况是：100~199，1100~1199,2100~2199，，....，11100~11199，一共1200个。
// 和上面情况一样，并且等于更高位数字（12）乘以 当前位数（100）。
// 但同时它还受低位影响，百位出现1的情况是：12100~12113,一共114个，等于低位数字（113）+1。
// 3 如果百位上数字大于1（2~9），则百位上出现1的情况仅由更高位决定。
// 比如12213，则百位出现1的情况是：100~199,1100~1199，2100~2199，...，11100~11199,12100~12199,一共有1300个，并且等于更高位数字+1（12+1）乘以当前位数（100）。
int cntOne(int n) {
    // now为当前位的数，left为更高位的数，right为更低位的数，a就是第几位
	int left = 0, right = 0, now = 0, a = 1;
	int ans = 0;
	while (n / a) {
		left = n / (a * 10);  // 左边位的数
		right = n % a;  // 右边位的数
		now = (n / a) % 10;  // 当前位的数
		if (now == 0) ans += left * a;
		else if (now == 1) ans += left * a + right + 1;
		else ans += (left + 1)*a;
		a *= 10;
	}
	return ans;
}


// 蓝桥杯 将所有包含2,3,5,7的数从自然数中删除，求第n个数
// 将k个数删除，相当于10-k进制
// 根据题意，6进制的每个数是0,1,4,6,8,9
// 127=3*36+3*6+1
// 127/6 = 21...1
// 21/6 = 3...3
// [3,3,1]
int delete2357(int n) {
    vector<int> digit{0,1,4,6,8,9};
    int ret = 0, x = 1;
    while (n) {
        ret += digit[n%6]*x;
        x *= 6;
        n /= 6;
    }
    return ret;
}


// 509 easy 求一个斐波那契数列求第n项。f(n)=f(n-1)+f(n-2)
// A = [1 1
//      1 0]
// A^2=[2 1
//      1 1]
// A^3=[3 2
//      2 1]
// 可以发现，A^n的矩阵的左上角元素是f(n+1)，左下角和右上角是f(n)，右下角是f(n-1)
class Solution {
public:
	struct yhMatrix {
		vector<int> mat;  // 0->(0,0) 1->(0,1) 2->(1,0) 3->(1,1)
		yhMatrix() : mat(vector<int>(4,0)) {}
	};
	yhMatrix mul(yhMatrix a, yhMatrix b) {
		yhMatrix tmp;
		tmp.mat[0] = a.mat[0]*b.mat[0] + a.mat[1]*b.mat[2];
		tmp.mat[1] = a.mat[0]*b.mat[1] + a.mat[1]*b.mat[3];
		tmp.mat[2] = tmp.mat[1];  // 都是对称矩阵
		tmp.mat[3] = a.mat[2]*b.mat[1] + a.mat[3]*b.mat[3];
		return tmp;
	}
	int fib(int n) {
		yhMatrix base, ret;
		base.mat[0] = base.mat[1] = base.mat[2] = 1;
		base.mat[3] = 0;
		ret.mat[0] = ret.mat[3] = 1;  // 单位矩阵
		ret.mat[1] = ret.mat[2] = 0;
		while(n) {
			if(n&1) {
				ret = mul(ret, base);
			}
			base = mul(base,base);
			n >>= 1;
		}
		return ret.mat[1];
	}
};


// 134 medium gas-cost有正有负的环状数组，找到一个索引，加一遍，过程中不能出现负数，找到改索引
// 无解返回-1。如果必有解的话，假想一个图，纵坐标是前缀和，从0点开始有正有负。
// 如果想要从某个点开始一直为正，这个解肯定出现在前面图为负数区域恰好结束后的店
int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
	int sum = 0, n = gas.size();
	for (int i = 0; i < n; ++i) sum += gas[i]-cost[i];
	if (sum < 0) return -1;
	int start = 0;
	sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += gas[i] - cost[i];
		if (sum < 0) {
			sum = 0;
			start = i + 1;
		}
	}
	return start == n ? 0: start;
}


// 1551 medium 长度为n的等差数列{2x+1}，可以选择任意两个数，一个+1，一个-1
// 返回使数组中所有元素相等的最小操作数，一定存在。
// 从中心开始，两边往中心靠拢就行
int minOperations(int n) {
    return n*n/4; // 奇数 (n*n-1)/4取整
}