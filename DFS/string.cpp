#include "myheader.h"

// 字符串
// 本质上还是排列和组合


// 17 medium 电话号码的字母组合：给定一个仅包含数字 2-9 的字符串
// 比如23就能对应3*3种结果，返回所有它能表示的字母组合。
// 需要按照digits的顺序，所以就是简单的dfs，只需要记录每次遍历到了digits的第几个字符就行
class Solution {
public:
    vector<string> digit2char = {"abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    string snap;
    vector<string> res;
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        dfs(0, digits);
        return res;
    }
    void dfs(int pos, string& digits) {
        if (snap.size() == digits.size()) {
            res.emplace_back(snap);
            return;
        }
        for (char &ch : digit2char[digits[pos]-'0'-2]) {
            snap += ch;
            dfs(pos+1, digits);
            snap.pop_back();
        }
    }
};


// 131 medium 分割字符串，使每个子串都是回文串。返回所有可能的分割方案。
// 决策树同一层的子节点为每次可以分割的子串长度，还是组合问题
class Solution {
public:
    vector<vector<string>> res;
    vector<string> snap;
    vector<vector<string>> partition(string s) {
        dfs(0, s);
        return res;
    }
    void dfs(int pos, string &s) {
        if (pos == s.size()) {
            res.emplace_back(snap);
            return;
        }
        for (int i = pos; i < s.size(); ++i) {
            string str = s.substr(pos, i-pos+1);
            if (isPalindrome(str)) {
                snap.emplace_back(str);
                dfs(i+1, s);
                snap.pop_back();
            }
        }
    }
    bool isPalindrome(string& str) {
        int len = str.size();
        for (int i = 0; i < len/2; ++i) {
            if (str[i] != str[len-i-1]) return false;
        }
        return true;
    }
};


// 93 medium 把s分割成合法的IP地址，中间用'.'分割
// 决策树的子树是当前要分割的长度，最短为1，最长为3。下层就是新的IP段
// 131使用了substr会创造出额外的空间，这里只用原来的s和下标进行操作，节省了空间
class Solution {
public:
    vector<string> res;
    int dotNum = 0;
    vector<string> restoreIpAddresses(string s) {
        if (s.size() < 4 || s.size() > 12) return res;  // 第一次剪枝
        dfs(0, s);
        return res;
    }
    void dfs(int pos, string &s) {
        if (dotNum == 3) {
            if (isValid(s, pos, s.size()-1)) res.emplace_back(s);
            return;
        }
        // 第二次剪枝
        for (int i = pos; i < s.size() && i < pos+3; ++i) {
            if (isValid(s, pos, i)) {
                s.insert(s.begin()+i+1, '.');
                dotNum += 1;
                dfs(i+2, s);  // 因为加了一个点，所以需要再+1
                s.erase(s.begin()+i+1);
                dotNum -= 1;
            } else break;  // 第三次剪枝，因为这一层的分层失败，全部不算了
        }
    }
    bool isValid(string& s, int l, int r) {
        if (l == s.size()) return false;  // 最后一个为空的情况
        if (s[l] == '0' && l != r) return false;  // 只有单独一个的0才会考虑
        int sum = 0;
        for (int i = l; i <= r; ++i) {  // [l,r]
            sum = sum*10 + s[i] - '0';
            if (sum > 255) return false;  // 函数内部直接返回更快
        }
        return true;
    }

};



