#include "myheader.h"

// 953 easy order为26个字母的乱序，words是按照order写的，判断是否复合语序
// 每次对一个word进行遍历太麻烦，干脆对word用order排序，最后判断是否有序
bool isAlienSorted(vector<string>& words, string order) {
    vector<char> rcd(26);  // 记录每个字母的位置，比如c在第一个，rcd[c-'a]=0+'a'
    for (int i = 0; i < 26; ++i) rcd[order[i]-'a'] = i+'a';
    for (string& word : words) {
        for (char& ch : word) {
            // 让乱序的变有序
            ch = rcd[ch-'a'];
        }
    }
    return is_sorted(words.begin(), words.end());
}


// 14 easy 字符串数组中的最长公共前缀
// ["flower","flow","flight"]  "fl"
// 横向和纵向都可以，这里选择纵向
string longestCommonPrefix(vector<string>& strs) {
    for (int j = 0; j < strs[0].size(); ++j) { // 第1个单词的列遍历
        for (int i = 1; i < strs.size(); ++i) { // 竖着的行遍历
            if (j == strs[i].size() || strs[i][j] != strs[0][j]) {
                return strs[0].substr(0,j);
            }
        }
    }
    return strs[0];
}