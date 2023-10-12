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
    vector<string> ret;
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        dfs(0, digits);
        return ret;
    }
    void dfs(int pos, string& digits) {
        if (snap.size() == digits.size()) {
            ret.emplace_back(snap);
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
    vector<vector<string>> ret;
    vector<string> snap;
    vector<vector<string>> partition(string s) {
        dfs(0, s);
        return ret;
    }
    void dfs(int pos, string &s) {
        if (pos == s.size()) {
            ret.emplace_back(snap);
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


// 93 medium 字符串s只包含数字，用以表示一个 IP 地址，返回所有可能的有效 IP 地址，中间用'.'分割
// 尝试不在s中去改。设置一个确定长度的数组seg，并且更新时带有idx信息，所以不需要推缩。
class Solution {
public:
    static constexpr int SEG_COUNT = 4;
    vector<string> ret;
    vector<int> seg; // 数字的4段
    int dotNum = 0;
    vector<string> restoreIpAddresses(string s) {
        if (s.size() < SEG_COUNT || s.size() > SEG_COUNT * 3) return ret;
        seg.resize(SEG_COUNT);
        dfs(0, 0, s);
        return ret;
    }
    void dfs(int segID, int segStart, const string &s) {
        if (segID == SEG_COUNT) { // 最后一段
            if (segStart == s.size()) { // 最后一段也遍历完了
                string snap;
                for (int i = 0; i < SEG_COUNT; ++i) { // 把seg数组的四个数字写到res
                    snap += to_string(seg[i]);
                    if (i < SEG_COUNT - 1) snap += '.';
                }
                ret.emplace_back(std::move(snap));
            }
            return;
        }
        if (segStart == s.size()) return;
        if (s[segStart] == '0') {
            seg[segID] = 0;
            dfs(segID + 1, segStart + 1, s);
            return;
        }
        int segSum = 0;
        for (int cur = segStart; cur < s.size() && cur < segStart + 3; ++cur) {
            segSum = segSum * 10 + (s[cur] - '0');
            if (segSum > 255) break;
            else {
                seg[segID] = segSum; // 带有idx信息，不需要推缩。
                dfs(segID + 1, cur + 1, s);
            }
        }
    }
};


// medium 1236 网络爬虫
// 给出一个str，这个str通过htmlParser会返回一组str，子str通过htmlParser还可以返回……
// str其实是一个url，现在求所有和start url具有相同domain的url
// 自顶向下，用一个hashset去重
class HtmlParser {
public:
    vector<string> getUrls(string url);
};
class Solution {
public:
    vector<string> crawl(string startUrl, HtmlParser htmlParser) {
        string domain_name = getDomainName(startUrl);
        unordered_set<string> vis;
        vector<string> ret;
        vis.insert(startUrl);
        ret.emplace_back(startUrl);
        dfs(startUrl, domain_name, htmlParser, vis, ret);
        return ret;
    }
    void dfs(string startUrl, string src_domain, HtmlParser htmlParser, unordered_set<string> &vis, vector<string> &ret) {
        vector<string> urls = htmlParser.getUrls(startUrl);
        for (string &url : urls) {
            if (getDomainName(url) != src_domain) continue; // 链接到的url不是相同域名，舍弃
            if (vis.count(url)) continue;
            vis.insert(url);
            ret.emplace_back(url);
            dfs(url, src_domain, htmlParser, vis, ret);
        }
    }
    string getDomainName(string url) {
        int pos = url.find("/", 7); // 从http://的下一个开始找
        if (pos == string::npos) { // http://www.abc.com 没有子目录
            pos = url.size(); // 就取最后一个有效数的下一个
        }
        return url.substr(7, pos - 7);
    }
};