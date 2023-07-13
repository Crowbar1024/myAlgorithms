#include "myheader.h"


// 28 medium 字符串匹配，haystack中needle部分的第一个下标，没有包括就返回-1
// https://www.zhihu.com/question/21923021/answer/281346746
void getNext(string& pattern, vector<int>& next) {
    next[0] = -1;
    int i = 0, j = -1; // i是后缀第一个的下标，j是前缀第一个的下标
    while (i < pattern.size()-1) { // 知乎的边界是是错的，因为是先+1再赋值
        if (j == -1 || pattern[j]==pattern[i]) {
            ++j;
            ++i;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}
int strStr(string haystack, string needle) {
    int i = 0, j = 0, slen = haystack.size(), plen = needle.size();
    vector<int> next(plen);
    getNext(needle, next);
    while (i < slen && j < plen) {
        if (j == -1 || haystack[i] == needle[j]) { // 相等
            ++i;
            ++j;
        } else {
            j = next[j]; // p跳转到后缀的头
        }
    }
    if (j == plen) return i-j; // 匹配完整
    return -1; // 失败
}