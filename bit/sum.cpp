#include "myheader.h"



// 136 easy nums除了某个元素只出现一次以外，其余每个元素均出现两次。找出那个只出现了一次的元素。
int singleNumber(vector<int>& nums) {
    int ret = 0;
    for (const int& n : nums) ret ^= n;
    return ret;
}

// 268 easy 给定一个包含 [0, n]（一共n+1个） 中 n 个不同数的数组 nums ，找出 [0, n] 这个范围内没有出现在数组中的那个数。
int missingNumber(vector<int>& nums) {
    int ret = 0;
    for (const int& num : nums) ret ^= num;
    for (int i = 0; i <= nums.size(); ++i) ret ^= i;
    return ret;
}

// 389 easy 两个字符串只包含小写字母。t由s 随机重排，然后在随机位置添加一个字母。请找出在 t 中被添加的字母。
// char也能做位运算，隐式转换
char findTheDifference(string s, string t) {
    int ret = 0;
    for (const char& ch: s) ret ^= ch;
    for (const char& ch: t) ret ^= ch;
    return ret;
}


// 287 medium 数组长度为n+1，是由[1,n]中的数组成（可以不用完），其中一个数字重复了多次，找到这个重复的数字
// 显然有位运算解法，具有最少的空间复杂度O1
// 因为这个数组的性质，1 2 3 3 4 5 (1 2 3 4 5)
// x^x = 0   0^x = x
// 最后得到的就是x
int findDuplicate(vector<int>& nums) {
    int ret = 0, n = nums.size()-1;
    for (int i = 1; i <= n; ++i) ret ^= i;
    for (int x : nums) ret ^= x;
    return ret;
}


// 191 easy 数1
int hammingWeight(uint32_t n) {
    // int cnt = 0;
    // while (n) { 
    //     n &= n-1; 
    //     ++cnt; 
    // }
    // return cnt;
    return __builtin_popcount(n);
}

// 231 easy 2的幂
bool isPowerOfTwo(int n) {
    if (n <= 0) return false;
    return (n&(n-1)) == 0;
}


// 2595 easy 统计二进制的奇数位和偶数位为1的个数
vector<int> evenOddBit(int n) {
    vector<int> ret(2); // {even odd} 下标从0开始
    // ^=1 表示0/1/0/1的变换
    for (int i = 0; n; n >>= 1, i ^= 1) ret[i] += n&1;
    return ret;
}
// 利用__builtin_popcount
vector<int> evenOddBit(int n) {
    return vector<int>{__builtin_popcount(n&0x5555), __builtin_popcount(n&0xaaaa)};
}