#include "myheader.h"

// 2244 medium tasks表示每个任务的难度级别。在每一轮中，你可以完成2个或者3个相同难度级别的任务。
// 返回完成所有任务需要的最少轮数，如果无法完成所有任务，返回 -1。
// 想一想，任何数都可以用若干个2和3的组合得到，除了1。
// 有一些情况需要考虑，若 x%3=2，那么就是x/3+1
// 若 x%3=1，那么把一个3拆开，x/3-1+2
int minimumRounds(vector<int>& tasks) {
    unordered_map<int, int> cnt; // 每种任务的个数
    for (int x : tasks) cnt[x]++;
    int times = 0;
    for (auto [_, x] : cnt) {
        if (x == 1) return -1;
        else if (x == 2) times++;
        else if (x%3 == 0) times += x/3;
        else times += x/3+1;
    }
    return times;
}


// 343 medium 给定一个正整数n，将其拆分为至少两个正整数的和，并使这些整数的乘积最大化，返回这个乘积
//  4：2*2
//  5：2*3
//  6：3*3
//  7：2*2*3 
//  8：2*3*3
//  9：3*3*3
//  10：2*2*3*3
//  11：2*3*3*3
//  12：3*3*3*3
//  13：2*2*3*3*3
// 整明白了，就是2和3的幂，而且3尽可能的多，所以2最后只有一个或者2个
int integerBreak(int n) {
    if (n == 2) return 1;
    if (n == 3) return 2;
    int n2 = n % 3;
    int n3 = n / 3;
    int ret;
    if (n2 == 0) {  // 6 9 12
        ret = (int) pow(3, n3);
    } else if (n2 == 1) {  // 7 10 13
        ret = 4 * (int) pow(3, n3-1);
    } else {  // 5 8 11
        ret = 2 * (int) pow(3, n3);
    }
    return ret;
}

// 进阶版，由于答案过大，请对 998244353 取模。
// 快速幂
long long myPow(long long m) {
    long long ret = 1, x = 3;
    while (m) {
        if (m & 1) ret = (ret*x)%998244353;
        x = (x*x)%998244353;
        m >>= 1;
    }
    return ret;
}
long long cutRope(long long number) {
    if (number == 2) return 1;
    if (number == 3) return 2;
    long long n2 = number % 3;
    long long n3 = number / 3;
    long long ret;
    if (n2 == 0) {
        ret = myPow(n3);
    } else if (n2 == 1) {
        ret = 4 * myPow(n3-1);
    } else {
        ret = 2 * myPow(n3);
    }
    return ret%998244353;
}


// POJ 2140 n可以写成一串连续的数之和，问共有几串的组合
// 比如 15 = 15 = 7+8 = 4+5+6 = 1+2+3+4+5 共有4种
// 最直观的解法是利用等差数列，构造a1和n的关系，通过遍历a1的可能，复杂度On
// 下面是OLogn的解法，解释如下：
// 15是否可以用1个数组成？可以，r += 1 15/1=15
// 14是否可以用2个数组成？可以，r += 1 14/2=7,7+1
// 12是否可以用3个数组成？可以，r += 1 12/3=4 4+1 4+2
// 9是否可以用4个数组成？不可以
// 5是否可以用5个数组成？可以，r += 1 5/5=1 2 3 4 5
int consecutiveSum(int n) {
    int i = 1, r = 0;
	while (n > 0) {
		r += !(n % i);
		n -= i++;
	}
	return r;
}


// POJ 1032 N = A1+A2+...+An（n个数两两互不相等）
// 求 A1*A2*...*An的最大值时的序列
// 对任意一个整数a，a=b+c(b>1,c>1)，那么b*c>=a，即任意一个数拆为两个数(都大于1)后其乘积大于该数。
// 因此，我们的目标是求得 N=2+3+4+...+(n-1)+x
// 因为拆分后的数不能重复，即最后剩余的x要拆为x个1，从后往前分别加到已拆的数中（如果从前往后会出现重复数值）。
vector<int> maxProduct(int N, int n) {
	int i;
    vector<int> arr(n, 0);
	for (i = 2; N - i >= 0; i++) {
		N -= i;
		arr[i-2] = i;
	}
    for (int j = i - 3; j >= 0 && N > 0; j--, N--) {    
        arr[j]++;
    }
}

// POJ 1218 猩猩点灯 监狱里有n盏灯，从1到n，一开始都是关着的。
// 每次把序号倍数的灯，改变其状态。即改变1~n遍，求最后有多少个灯是亮的。
// 对于某盏灯n，含有的因子（包括1和n）个数为m，则这个灯亮暗的次数就是m。
// 显然，如果m是奇数，则门n最终是亮的，最终这个问题就是求1~n中因子个数是奇数的数的个数。
// 一个数x可以质因数分解：x=(p1^a1)*...*(pn^an)，那么x的因子个数就是(a1+1)*...*(an+1)
// 比如24=2^3*3 {1,2,4,8} {1,3} 两个集合之间相乘就是4*2=8
// 因为要求x的因子个数是奇数，则上面所有的ai+1都是奇数，因为有个偶数乘积就会是偶数，那么所有ai都是偶数
// 那么n可以表示成： n=(p1^2b1)*...*(pn^2bn)=(p1^b1*...*pn^bn)^2
// 即n是个完全平方数
// 至此得出一个结论，一个正整数n是完全平方数的充分必要条件是n有奇数个因数（包括1和n本身）。 
// 最终转化成求小于等于n的数中有几个完全平方数。
// 比如63，平方根为7，显然[1,7]就是完全平方数
int lightOnNumber(int n) {
    int num = (int) sqrt(n);
    return num;
}

// 367 easy 判断x是否是完全平方数
// 2n-1的等差数列的和就是完全平方数
bool isPerfectSquare(int num) {
    int x = 1;
    while (num > 0) {
        num -= x;
        x += 2;
    }
    return num == 0;
}