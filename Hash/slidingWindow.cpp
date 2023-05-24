#include "myheader.h"


// 904 medium 正数串 [3,3,3,1,2,1,1,2,3,3,4] ，求“只”包含两种数字的最长连续子串的长度，这里是len[1,2,1,1,2] = 5
// [l,r]是符合题意的子串，肯定每次都是r向右扩张，当[l,r]中数字种类恰好超过2时，让l前进到恰好又满足2的位置
// 所以需要借助哈希表统计每种数的个数
int totalFruit(vector<int>& fruits) {
    unordered_map<int,int> cnt;
    int l = 0, r = 0, len = fruits.size();
    int ret = 0;
    while (r < len) {
        // 可以直接放，因为map可以统计size
        cnt[fruits[r]] += 1;
        while (cnt.size() > 2) {
            cnt[fruits[l]] -= 1;
            if (cnt[fruits[l]] == 0) {
                cnt.erase(fruits[l]);
            }
            ++l;
        }
        ret = max(ret, r-l+1);
        ++r;
    }
    return ret;
}

// 76 hard 字符串s、字符串t。返回s中涵盖t所有字符的最小子串。如果s中不存在涵盖t所有字符的子串，则返回空字符串 
// s = "ADOBECODEBANC", t = "ABC" 输出 "BANC"
// 和904类似，但最大滑窗是在右移右边界的过程中更新结果（904是左边界向右走完后才更新），
// 而最小滑窗是在迭代右移左边界的过程中更新结果
string minWindow(string s, string t) {
    unordered_map<char,int> cnt;
    for (char ch : t) {
        cnt[ch] += 1;
    }
    int tsum = t.size();
    int l = 0, r = 0, len = s.size();
    int ret = len+1;  // len显然是错的
    string strRes = "";
    while (r < len) {
        if (cnt.count(s[r]) > 0) {
            // 只有该元素少才能变tsum
            if (cnt[s[r]] > 0) {
                tsum -= 1;
            }
            cnt[s[r]] -= 1;  // 该值是可能为负的
        }
        while (tsum == 0) {
            if (ret >= r-l+1) {
                ret = r-l+1;
                strRes = s.substr(l, ret);
            }
            if (cnt.count(s[l])) {
                if (cnt[s[l]] == 0) {
                    tsum += 1;
                }
                cnt[s[l]] += 1;
            }
            ++l;
        }
        ++r;
    }
    return strRes;
}