#include "myheader.h"

// 字符串最小表示法
// 返回开头节点
// 双指针法，l和r分别指向可能为最小表示法的节点
// k表示相等substring的长度
// 6223724[2]23
int MinimumRepresentation(string s) {
    int i = 0, j = 1, k = 0, len = s.size();
    while (i < len && j < len && k < len) {
        int d = s[(i+k)%len] - s[(j+k)%len];
        if (d == 0) k++;
        else {
            if (d < 0) j += k+1;  // 因为此时[j+1,j+k]都不可能成为最小表示法的头
            else i += k+1;
            k = 0;
            if (i == j) ++j;
        }
    }
    return min(i, j);
}

// hard 1163 返回s的子串中字典序最大的
// 和字符串最小表示法思路完全一致，甚至更简单
string lastSubstring(string s) {
    int i = 0, j = 1, k = 0, len = s.size();
    while (i+k < len && j+k < len) {
        int d = s[i+k] - s[j+k];
        if (d == 0) k++;
        else {
            if (d < 0) i += k+1;
            else j += k+1;
            k = 0;
            if (i == j) ++j;
        }
    }
    int ret = min(i, j);
    return s.substr(ret);
}