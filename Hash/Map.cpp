#include "myheader.h"



// 560 medium nums不是有序且有负数，求所有的字串之和为k的连续子串的个数
// 找字串和，一般都和前缀和有关，假设[i,j]为符合题意的子串，则 preSum[i]-preSum[j-1]=k
// 所以第一种方法就是前缀和+暴力搜索i和j O(n^2)
// 可以发现，preSum[i]-k=preSum[j-1]，可以利用空间换时间的思路
// 如果用一个键值对 {preSum[i],times} 去记录每个前缀和出现的次数，
// 对于每一个preSum[i]，如果前面有多个符合题意的preSum[j-1]，cnt就可以加上这个数量
// 所以只需要一次遍历
// 前缀和+哈希表
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> mp;
    mp[0] = 1;  // 和为0的情况初始化为1，即前0个数之和
    int preSum = 0, res = 0;
    for (int& num : nums) {
        preSum += num;
        if (mp.find(preSum-k) != mp.end()) {
            res += mp[preSum-k];
        }
        mp[preSum] += 1;
    }
    return res;
}


// 49 medium strs中包含若干个字符串，其中某些的字母种类和对应的个数相同，一组称为anagram，将其组起来
// Input: strs = ["eat","tea","tan","ate","nat","bat"]
// Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> m; // 键值就是排好序的string
    vector<vector<string>> res;
    for (string& str : strs) {
        string key = str;
        sort(key.begin(), key.end());
        m[key].emplace_back(str);
    }
    for (auto it = m.begin(); it != m.end(); ++it) res.emplace_back(it->second);
    return res;
}
// 2 用26个字母对应的个数组成的数组，但这样键值就是一个数组，C++对于这种情况需要写一个对于数组的哈希函数，过于复杂
// 3 使用质数去设计键值，26个字母对应的质数，每个单次的字母的质数的一定不会重复，但有可能会溢出，取模有概率会碰撞

// 438 medium s中包括p的anagram，返回起始下标
// s = "cbaebabacd", p = "abc"  ----> [0,6]
// 我一开始的思路就是遍历匹配，显然每次都要排序，不够简单
// 然后发现可以利用滑动窗口
vector<int> findAnagrams(string s, string p) {
    int l = 0, r = 0, len = p.size();
    unordered_map<char,int> pm;
    for (char ch = 'a'; ch <= 'z'; ++ch) pm[ch] = 0;
    for (char ch : p) pm[ch] += 1;
    unordered_map<char,int> wd;
    vector<int> res;
    while (r < s.size()) {
        wd[s[r]] += 1;
        // 只要wd多了，就可以l前移
        while (wd[s[r]] > pm[s[r]]) {
            wd[s[l]] -= 1;
            ++l;
        }
        if (r-l+1 == len) {
            res.emplace_back(l);
        }
        ++r;
    }
    return res;
}