#include "myheader.h"

/*
    一般解决正整数连续子串和的问题有多种方法
    1. 二分法：找sum[j]==sum[i]-target的最大j
    2. 滑动窗口
    
    我认为滑动窗口一般分为多类
    1. 左右向中间靠拢，这种一般是有序数组，这种一般都可以归为双指针，所以不算滑动窗口
    2. 左右同时从头开始，一般非有序

    技术细节
    1. 一般r在一次循环前进一次
    2. l会在一次循环可能前进多次
    3. 有时候会引入哈希表
    
    当数组中存在负数时，滑动窗口就失灵了，因为窗口边框前进和后退不知道和究竟是变大还是变小
    此时可以选择前缀和
*/



// 209 medium 正数数组，找到最短的连续子串的和满足恰好大于等于target，没有返回0
int minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size(), ret = n + 1, curSum = 0;
    for (int l = 0, r = 0; r < n; ++r) {
        curSum += nums[r];
        while (curSum >= target && l <= r) {
            ret = min(ret, r-l+1); // 更新必须得放这，因为这是最小，出了循环保证不了
            curSum -= nums[l++];
        }
    }
    return ret == n + 1 ? 0 : ret;
}
// 325 medium 209改 找到和等于 k 的最长连续子数组长度，没有返回0
// 这里还以为是正整数，所以写了下面的代码，发现存在负数就会失效
// 所以得用前缀和去做
int maxSubArrayLen(vector<int>& nums, int k) {
    int n = nums.size(), ret = 0, curSum = 0;
    for (int l = 0, r = 0; r < n; ++r) {
        curSum += nums[r];
        if (curSum < k) continue; // 让r一直往右滑直到恰好>=k
        while (curSum > k && l <= r) {
            curSum -= nums[l++];
        } // 退出后是<=k
        if (curSum == k) {
            ret = max(ret, r-l+1);
        }
    }
    return ret;
}


// 1208 medium 两个长度相同的字符串，将s[i]变到t[i]需要 |s[i] - t[i]| 的开销
// 用于变更字符串的最大预算是 maxCost。在转化字符串时，总开销应当小于等于该预算，这也意味着字符串的转化可能是不完全的。
// 如果你可以将 s 的子字符串转化为它在 t 中对应的子字符串，则返回可以转化的最大长度。
// 如果 s 中没有子字符串可以转化成 t 中对应的子字符串，则返回 0。
// 问题可以转化为，连续子串的和不超过给定值，其最大长度
// 板子题
int equalSubstring(string s, string t, int maxCost) {
    int n = s.size();
    vector<int> diff(n, 0);
    for (int i = 0; i < n; ++i) {
        diff[i] = abs(s[i] - t[i]);
    }
    int ret = 0;
    for (int l = 0, r = 0, sum = 0; r < n; ++r) { // 每次r前进
        sum += diff[r];
        while (sum > maxCost && l <= r) {
            sum -= diff[l++];
        }
        // 如果有一个位置特别大，那么该位置不满足maxCost，所以此时 r-l+1 == 0
        ret = max(ret, r - l + 1);
    }
    return ret;
}


// 3 medium 返回s没有重复字符的最长连续字串 "pwwkew"->"wke" 3
// 正常思路是set+滑动窗口
int lengthOfLongestSubstring(string s) {
    unordered_set<char> digitSet;
    int ret = 0;
    for (int l = 0, r = 0; r < s.size(); ++r) {
        while (digitSet.count(s[r]) > 0 && l < r) { // 一直删除l，直到不重复
            digitSet.erase(s[l++]);
        }
        digitSet.insert(s[r]);
        ret = max(ret, r-l+1);
    }
    return ret;
}
// 但是有个低效的地方，右边界重复的情况下，得一直删除左边界并前进
// 可以用map记下每个字符的索引，直接进行跳转
int lengthOfLongestSubstring(string s) {
    unordered_map<char,int> pos; // 右边界字符第一次出现的位置
    int ret = 0;
    for (int l = 0, r = 0; r < s.size(); ++r) {
        // 右边界重复，左边界就直接跳到右边界字符第一次出现的下一个
        if (pos.count(s[r]) > 0) l = max(l, pos[s[r]]+1);
        pos[s[r]] = r; // 更新
        ret = max(ret, r-l+1);
    }
    return ret;
}


// NC28 hard 给出两个字符串 s 和 t，要求在 s 中找出最短的包含 t 中所有字符的连续子串。（注意包含的顺序没要求）
// 数据范围：两者长度<10000，保证s和t字符串中仅包含大小写英文字母
// 要求：空间复杂度 O(n) 时间复杂度 O(n)
// 如果 s 中没有包含 t 中所有字符的子串，返回空字符串
// 满足条件的子串可能有很多，但是题目保证满足条件的最短的子串唯一。
// 根据复杂度的要求就可以排除set和map的可行性，自然就想到滑动窗口
// 由于不要求包含T的顺序，所以需要一个cnt数组去记录T所有元素的个数
string minWindow(string S, string T) {
    // A-65 a-97
    vector<int> cnt(128, 0);
    // cnt记录T中每个字符的数量
    for (auto ch : T) cnt[ch-'A'] += 1;
    // restNumT表示 T 中还未被 [l,r) 包含的元素个数
    int l = 0, r = 0, restNumT = T.size();
    int len = INT_MAX, head = 0;
    while (r < S.size()) {
        // 当前r属于T，而且份额还没用完，直接用（左闭右开）
        if (cnt[S[r]-'A'] > 0) {
            restNumT -= 1;
        }
        cnt[S[r]-'A'] -= 1;  // 当窗口中包含所有T元素时，其他元素的cnt值可能小于0
        r += 1;
        // 表示全部恰好包含全部了，开始移动l
        while (restNumT == 0) {
            if (r-l < len) {
                len = r-l;
                head = l;
            }
            // 前面的r已经走了很长的距离，导致T的所有元素都被[l,r)包括了
            // 所以自然cnt数组内所有属于T的元素的数量都小于等于0（r的移动可能接受超过T数量的元素被包含进来）
            // 所以没有出现在T中的元素cnt肯定是负数
            // 如果是0，说明此时对应的字符已经到了恰好可以包含T的窗口的左边界，所以终止while循环，去更新右边界
            if (cnt[S[l]-'A'] == 0) {
                restNumT += 1;  // 接下来要向右移动l，所以会增加
            }
            cnt[S[l]-'A'] += 1;  // 接下来要向右移动l，所以l对应的会增加
            l += 1;
        }
    }
    return len == INT_MAX ? "" : S.substr(head, len);
}


// 1004 medium 一串01序列，k为最大翻转数，问通过最多k次翻转，最长的1子串长度为多少
// 即求一串01子串中，0有k个，这个子串的最长长度
// 即需要一个值cnt用来统计窗口中的0个数
int longestOnes(vector<int>& nums, int k) {
    int ret = 0, n = nums.size();
    for (int l = 0, r = 0, cnt = 0; r < n; ++r) {
        if (nums[r] == 0) ++cnt;
        while (cnt > k) {
            if (nums[l++] == 0) --cnt;
        }
        ret = max(ret, r-l+1);
    }
    return ret;
}

// 340 medium 至多包含 k 个不同字符的最长子串的长度
// 哈希表+滑动窗口
int lengthOfLongestSubstringKDistinct(string s, int k) {
    int ret = 0, n = s.size();
    unordered_map<char,int> digitCnt;
    for (int l = 0, r = 0; r < n; ++r) {
        digitCnt[s[r]] += 1;
        while (digitCnt.size() > k) {
            digitCnt[s[l]] -= 1;
            if (digitCnt[s[l]] == 0) digitCnt.erase(s[l]);
            ++l;
        }
        ret = max(ret, r - l + 1);
    }
    return ret;
}
// 由于字符串是char，[0,255]的，所以可以用数组+cnt变量代替
int lengthOfLongestSubstringKDistinct(string s, int k) {
    int ret = 0, n = s.size(), cnt = 0;
    vector<int> digitCnt(256, 0);
    for (int l = 0, r = 0; r < n; ++r) {
        if (digitCnt[s[r]] == 0) cnt += 1;
        digitCnt[s[r]] += 1;
        while (cnt > k) {
            digitCnt[s[l]] -= 1;
            if (digitCnt[s[l]] == 0) cnt -= 1;
            ++l;
        }
        ret = max(ret, r - l + 1);
    }
    return ret;
}




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
        // 只要wd多了，或者pm[s[r]]==0，就可以l前移
        // 第二种情况会让l==r
        // 并且为了应对pm[s[r]]==0，需要做map的初始化
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


// easy 674 最长严格上升子串长度，DP也能做
int findLengthOfLCIS(vector<int>& nums) {
    int l = 0, r = 0, ret = 1;
    for (int i = 1; i < nums.size(); ++i) {
        if (nums[i] > nums[r]) ++r;
        else l = r = i;
        ret = max(r-l+1, ret);  // 每次都要更新，避免一直都是严格递增，导致没有机会更新
    }
    return ret;
}
// 1839 meidum  所有元音按顺序排布的最长子字符串
// 只包括 a e i o u，所以边判断是否大于等于边记录种类数就行
int longestBeautifulSubstring(string word) {
    int l = 0, r = 0, ret = 0, cnt = 1;
    for (int i = 1; i < word.size(); ++i) {
        if (word[i] > word[r]) {
            ++r;
            ++cnt;
        } else if (word[i] == word[r]) {
            ++r;
        } else {
            l = r = i;
            cnt = 1;
        }
        if (cnt == 5) ret = max(r-l+1, ret);
    }
    return ret;
}


// 1151 medium 01数组，可以任意交换位置，问最少的交换次数，使得1聚合在一起
// 假设1有k个，聚合在一起，说明是一个大小为k的窗口，那么就是统计窗口中有多少0
// 即一个固定长度窗口中最少的0是多少，或者说1最多是多少。
int minSwaps(vector<int>& data) {
    int n = data.size();
    int k = accumulate(data.begin(), data.end(), 0);
    int curSum = accumulate(data.begin(), data.begin() + k, 0);
    int cnt1 = curSum;
    for (int l = 1; l <= n - k; ++l) {
        curSum += data[l + k - 1] - data[l - 1];
        cnt1 = max(cnt1, curSum);
    }
    return k - cnt1;
}


// 159 medium 至多包含两个不同字符的最长子串 英文字母
// 即滑动窗口中，字符数量至多为2，滑动窗口的最大长度
// 肯定不能l一次一次试，那就用一个哈希表记录s[l]在滑动窗口中的最远位置
int lengthOfLongestSubstringTwoDistinct(string s) {
    unordered_map<char,int> pos;
    int ret = 0, n = s.size();
    for (int l = 0, r = 0; r < n; ++r) {
        pos[s[r]] = r;
        if (pos.size() > 2) {
            l = pos[s[l]]  + 1;
            if (l == r) { // abaac 还是得保留b之后的a 去掉b
                for (auto [ch, idx] : pos) { // 找到b
                    if (ch != s[l - 1] && ch != s[r]) {
                        pos.erase(ch);
                        l = idx + 1;
                        break;
                    }
                }
            } else { // aabbc
                pos.erase(s[l - 1]);
            }
        }
        ret = max(ret, r - l + 1);
    }
    return ret;
}

// 1100 medium 所有长度为 k 且不含重复字符的子串，求子串的数目
// 使用一个数组记录每个元素的出现次数，每次移动时，减去上一次的头，加上这次的尾
int numKLenSubstrNoRepeats(string s, int k) {
    int n = s.size(), sum = 0;
    if (k > n) return 0;
    vector<int> cnt(26, 0);
    for (int i = 0; i < k; ++i) {
        if (cnt[s[i] - 'a'] == 0) ++sum;
        cnt[s[i] - 'a'] += 1;
    }
    int ret = (sum == k) ? 1 : 0;
    for (int l = 1; l <= n - k; ++l) {
        int r = l + k - 1;
        if (cnt[s[l - 1] - 'a'] == 1) --sum; // 注意先减后加
        cnt[s[l - 1] - 'a'] -= 1;
        if (cnt[s[r] - 'a'] == 0) ++sum;
        cnt[s[r] - 'a'] += 1;
        if (sum == k) ++ret;
    }
    return ret;
}