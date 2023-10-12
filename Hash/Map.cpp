#include "myheader.h"



// 49 medium strs中包含若干个字符串，其中某些的字母种类和对应的个数相同，一组称为anagram，将其组起来
// Input: strs = ["eat","tea","tan","ate","nat","bat"]
// Output: [["bat"],["nat","tan"],["ate","eat","tea"]]
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> m; // 键值就是排好序的string
    vector<vector<string>> ret;
    for (string& str : strs) {
        string key = str;
        sort(key.begin(), key.end());
        m[key].emplace_back(str);
    }
    for (auto it = m.begin(); it != m.end(); ++it) ret.emplace_back(it->second);
    return ret;
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
    vector<int> ret;
    while (r < s.size()) {
        wd[s[r]] += 1;
        // 只要wd多了，就可以l前移
        while (wd[s[r]] > pm[s[r]]) {
            wd[s[l]] -= 1;
            ++l;
        }
        if (r-l+1 == len) {
            ret.emplace_back(l);
        }
        ++r;
    }
    return ret;
}