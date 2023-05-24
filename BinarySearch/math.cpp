#include "myheader.h"



// 69 easy 牛顿法找x的根t 
// 假设f(x)=0，已知(x0,f(x0))是f(x)的点
// 那么经过(x0,f(x0))的切线就是 y = f(x0) + f'(x0)(x-x0)
// 所以x轴的交点就是(x0-f(x0)/f'(x0), 0)
// 然后迭代条件就是f(x)>0 (因为初始条件就是从>0的部分开始迭代的，所以初始如果从<0开始的话，需要相应的改变迭代条件)
// 这里就是求 f(t) = t*t - x = 0 的t，所以初始的t要大
// 所以新x轴交点 t = t - (f(t)/f(t)') = t - ((t*t-x) / (2t) = t/2 + x/(2t)
int mySqrt(int x) {
    const double eps = 1e-6;
    double t = x;
    while (t*t-x > eps) {
        t = (t+x/t)/2;  // t/2 + x/(2*t) 计算次数多了一次
    }
    return (int) t;  // 由于返回类型是整数，小数部分将被舍去
}
// 衍生：判断x是否是完全平方数 367

// 50 medium 求幂x^n
// 核心思想：快速幂
// 快速幂的本质：一个一个去乘太麻烦，把一共要乘的次数n用二进制的形式写出来，这个步骤复杂度是logn级别的
// 举个例子，求x^26，26的二进制是1 1 0 1 0。从最后一位开始创建
// 最后一位是0，写0，形式上就是res不变；
// 进位，体现为x=x*x，m/2=13；
// 13最后一位是1，那么在这个位置上写1，形式上就是res乘上x的这个位置的幂，以此类推……
// 难点：如果n是负数，那么将x赋值为x的倒数，但这样有个例外，当n是-2^31时，-n就超过了int范围，所以要让n转换成long
double myPow(double x, int n) {
    if (abs(x) < 1e-6) return x;  // x = 0
    if (abs(x) - 1 < 1e-6 && abs(x) - 1 > -1e-6) return x > 0 ? x : n & 1 ? x : -x;  // x = 1 or -1
    double ret = 1.0;
    long m = static_cast<long>(n);
    if (m < 0) {
        x = 1.0 / x;
        m = -m;
    }
    while (m) {
        if (m & 1) ret *= x;
        x *= x;
        m >>= 1;
    }
    return ret;
}

// 给你两个整数，被除数和除数(不为0)。将两数相除，要求不使用乘法、除法和取余运算。商截去其小数部分
// 假设我们的环境只能存储 32 位 有符号整数，如果商严格大于INT_MIN ，则返回INT_MIN；同理INT_MAX
// 最块的自然是使用移位操作，如果全部为负，C++对负数的移位操作做了提醒，而leetcode的C++直接禁止负数移位
// 所以为了效率，还是全部转化成为正数，但这样就带了一些负数所没有的边界问题
// 其实就是利用快速幂的性质，先找到一个最大的幂，然后不断减小，复杂度就是2logn级别。
int divide(int dividend, int divisor) {
    if (dividend == INT_MIN) {
        if (divisor == -1) return INT_MAX; // 只有这种情况才会越界
        else if (divisor > 0) return divide(dividend + divisor, divisor) - 1; // 避免边界问题的傻瓜解决方案
        else return divide(dividend - divisor, divisor) + 1;
    }
    if (divisor == INT_MIN) return 0;
    bool flag = (dividend ^ divisor) >= 0; // 同号商为正，异号为负
    if (dividend < 0) dividend = -dividend; // 全部变成正数
    if (divisor < 0) divisor = -divisor;
    int part = divisor; // 初始化为2^0*divisor
    int halfNum = dividend >> 1; // 19/3=6=4+2，最大的part只能不到原来的一半，-12>-9终止
    int power = 1; // 幂
    while (part <= halfNum) { // 可以相等，就是两者之间2的幂恰好只有一个
        part <<= 1;
        power <<= 1;
    }
    int ret = 0;
    while (dividend >= divisor) {
        while (part > dividend) { // 找到次大的幂
            part >>= 1;
            power >>= 1;
        }
        if (part <= dividend) { // 2^power*divisor
            dividend -= part;
            ret += power;
        }
    }
    return flag ? ret : -ret;
}