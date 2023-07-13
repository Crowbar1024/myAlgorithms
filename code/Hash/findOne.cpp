#include "myheader.h"


// 414 easy 找数组中第3大的（要求distinct），如果没有，返回最大的
// 根据distinct的要求用set来做，一直存储前三位最大的，set的insert操作和erase操作都是对数级的
// 因为这里要利用set存放是非严格上升的特性，不适合unordered_set
// 这样每次排除一个最小的，剩下三个最大的
// 至于优先队列，当然可以对原数组做一遍unique操作再放进放出，也行。
int thirdMax(vector<int>& nums) {
    set<int> top3;
    for (const int& n : nums)
        if (top3.insert(n).second && top3.size() > 3) top3.erase(top3.begin());
    return top3.size() == 3 ? *top3.begin() : *top3.rbegin();
}


// 720 medium 返回 words 中最长的一个单词，该单词是由 words 中其他单词逐步添加一个字母组成。
// 若其中有多个可行的答案，则返回答案中字典序最小的单词。若无答案，则返回空字符串。
// words = ["a", "banana", "app", "appl", "ap", "apply", "apple"]
// 输出："apple" 解释："apply" 和 "apple" 都能由词典中的单词组成。但是 "apple" 的字典序小于 "apply" 
// 根据单词长度和字典序顺序排序，然后用一个哈希表储存所有前缀
string longestWord(vector<string>& words) {
    sort(words.begin(), words.end(), [](const string& s1, const string& s2) {
        if (s1.size() == s2.size()) return s1 > s2;  // 让最小的位于相同长度的单词最后，这样遍历完就能得到
        return s1.size() < s2.size();
    });
    unordered_set<string> exist{""};
    string ret = "";
    for (const string& s : words) {
        string pre = s.substr(0, s.size()-1);
        if (exist.count(pre)) {  // 有前缀才放进来
            exist.insert(s);
            ret = s;
        }
    }
    return ret;
}