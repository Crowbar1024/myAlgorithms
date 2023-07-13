#include "myheader.h"


// 474 medium 找出二进制字符串数组strs的最大子集的大小，该子集中最多有m个0和n个1
// ["10","0001","111001","1","0"], m = 5, n = 3 最大子集是 {"10","0001","1","0"} ，因此答案是 4 
// dp[i][m][n] = max(dp[i-1][m][n], dp[i-1][m-strs[i].m][n-strs[i].n]+1)
// 两个维度，比较特殊
int findMaxForm(vector<string>& strs, int m, int n) {
    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
    for (const string& str : strs) {
        int zeroNum = 0, oneNum = 0;
        for (const char& ch : str) {
            if (ch == '0') ++zeroNum;
            else ++oneNum;
        }
        for (int i = m; i >= zeroNum; --i) {
            for (int j = n; j >= oneNum; --j) {
                dp[i][j] = max(dp[i][j], dp[i-zeroNum][j-oneNum]+1);
            }
        }
    }
    return dp[m][n];
}

// 1143 medium 两个字符串最长公共子序列
// 经典的DP，定义dp[i][j]为text1前i个和text2前j个的最长公共子序列长度
// 注意不要定义成以i，j结尾的最长公共子序列长度，这样难以确定返回值，除非说了是连续的公共子串
// 初始化：dp[0][j]=0，dp[i][0]=0，最后返回dp[m][n]
// 显然当text1[i]==text2[j]时：dp[i][j] = dp[i-1][j-1] + 1
// 否则：dp[i][j] = max(dp[i-1][j], dp[i][j-1])
// 根据状态转移方程，上边，左边，左上各一个，要求返回右下角
// 所以迭代顺序为：从上往下，从左往右
// 进行状态压缩，上边用上一个大循环dp[j]，左边用这次循环dp[j-1]
// 左上用一个临时变量prev存储
int longestCommonSubsequence(string text1, string text2) {
    int n1 = text1.size(), n2 = text2.size();
    vector<int> dp(n2+1, 0);  // dp[0][j]第0行初始化
    for (int i = 1; i <= n1; i++) {  // 遍历每一行，第0行已经初始化，不用遍历
        int prev = 0;  // dp[i-1][0] 第1列的左上
        dp[0] = 0;  // dp[i][0] 每一行的第0列，即全表的第0列。
        for (int j = 1; j <= n2; j++) {
            int tmp = dp[j];  // 在下一次中会变成相对应的左上
            if (text1[i-1] == text2[j-1]) dp[j] = prev + 1;
            else dp[j] = max(dp[j], dp[j-1]);
            prev = tmp;  // 成了下面遍历的前一次遍历的左上
        }
    }
    return dp[n2];
}
// 上面一问的结果的路径
// 思路完全一样，按照上面的代码定义dp[i][j]为最长的相同子序列的string
string longestCommonSubsequencePath(string text1, string text2) {
    int n1 = text1.size(), n2 = text2.size();
    vector<string> dp(n2+1, "");
    for (int i = 1; i <= n1; i++) {
        string prev = "";
        dp[0] = "";
        for (int j = 1; j <= n2; j++) {
            string tmp = dp[j];
            if (text1[i-1] == text2[j-1]) dp[j] = prev + text1[i-1];
            else dp[j] = dp[j].size() > dp[j-1].size() ? dp[j] : dp[j-1];
            prev = tmp;
        }
    }
    return dp[n2];
}


// 718 medium 最长公共子串
// 和1143就不同了，需要定义成以i，j结尾的最长公共子串长度，为了避免0歧义，增大一个
// 显然当text1[i-1]==text2[j-1]时：dp[i][j] = dp[i-1][j-1] + 1
// 不等于时显然有，dp[i][j] = 0
// 初始化：dp[0][j]=0，dp[i][0]=0，因为是i和j结尾，dp[0][0]不知道具体值，所以需要从0开始遍历
// 根据状态转移方程，左上一个
int findLength(vector<int>& nums1, vector<int>& nums2) {
    int l1 = nums1.size(), l2 = nums2.size();
    vector<int> dp(l2+1, 0);  // 不能是l2+1
    int ret = 0;
    for (int i = 1; i <= l1; ++i) {
        int prev = 0;  // 上一行的第0列不存在，设置0
        for (int j = 1; j <= l2; ++j) {
            int tmp = dp[j];
            if (nums1[i-1] == nums2[j-1]) dp[j] = prev+1;
            else dp[j] = 0;
            prev = tmp;
            ret = max(ret, dp[j]);
        }
    }
    return ret;
}
// 翻转遍历顺序以节省变量
int findLength(vector<int>& nums1, vector<int>& nums2) {
    int l1 = nums1.size(), l2 = nums2.size();
    vector<int> dp(l2+1, 0);  // 不能是l2+1
    int ret = 0;
    for (int i = 1; i <= l1; ++i) {
        for (int j = l2; j >= 1; --j) {
            if (nums1[i-1] == nums2[j-1]) dp[j] = dp[j-1]+1;
            else dp[j] = 0;
            ret = max(ret, dp[j]);
        }
    }
    return ret;
}
// 滑动窗口解法，即使两把尺子一开始接触到离开，A尺子屁股接触B的头
// 相遇的长度先增大再减小，每次在相遇部分遍历（长度相同，所以不同就从0开始）
// 假设两把尺子长度相同，都是m
// 第一次对比1个，A[m-1]和B[0]
// 第二次对比2个，A[m-2]和B[0]，A[m-1]和B[1]
int findLength(vector<int>& A, vector<int>& B) {
    int m = A.size(), n = B.size(), ret = 0;
    for(int diff = -(m - 1); diff <= n - 1; ++diff) { // 枚举对应关系
        for(int i = max(0, -diff), l = 0; i < min(m, n - diff); ++i) { // 遍历公共部分
            l = (A[i] == B[i + diff])? (l + 1) : 0;
            ret = max(ret, l);
        }
    }
    return ret;
}
// HJ65 查找两个字符串a,b中的最长公共子串。若有多个，输出在较短串中最先出现的那个。
// 因为状态压缩了，只有j的记录，所以最短串是谁呢？
// 根据遍历顺序，一定是从上到下，且要保证最先出现，那么必然nums1是最短串才行。
// 所以不能压缩，一定得是全量。
string findLength(string& nums1, string& nums2) {
    int l1 = nums1.size(), l2 = nums2.size();
    vector<vector<int>> dp(l1+1, vector<int>(l2+1,0));
    int ret = 0, pos = 0;
    for (int i = 1; i <= l1; ++i) {
        for (int j = 1; j <= l2; ++j) {
            if (nums1[i-1] == nums2[j-1]) dp[i][j] = dp[i-1][j-1]+1;
            else dp[i][j] = 0;
            if (dp[i][j] > ret) {
                pos = i;  // 是取不到的，所以减长度是起始位置。
                ret = dp[i][j];
            }
        }
    }
    return nums1.substr(pos-ret,ret);
}




// 字符串s由左括号和右括号组成，()(())这种就是正确的序列，求正确序列的最大长度
// 典型的用下标i结尾做DP的例子，假设dp[i]表示以i结尾的正确序列的长度
// ()(()())
// 01234567
// 02002048
// 当遍历到下标7时，如果为右括号，那么得找到那个与其匹配的左括号
// 找到之后，就是7前面的正确序列长度加上左括号左边的正确序列的长度
// 值得注意的就是下标的越界问题
int maxBracketLength(string s) {
    int n = s.size();
    vector<int> dp(n,0);
    int ret = 0;
    for (int i=1; i<s.size(); ++i) {
        if (s[i] == ')') {
            int pre = i-1-dp[i-1];  // 找到那个与s[i]相匹配的左括号
            if (pre && s[pre] == '(') {
                dp[i] = dp[i-1] + 2 + (pre>0 ? dp[pre-1] : 0);
            }
        }
        ret = max(ret, dp[i]);
    }
    return ret;
}





// 72 hard 两个字符串之间通过最少多少次增删改可以相同
// 这道题的难点是想到用DP去做
// 想到之后，理所当然，dp[i][j]表示以i和j结尾的字符串通过的最少次增删改的次数
// 那么初始情况：dp[0][j]=j, dp[i][0]=i 最后返回dp[l1][l2]
// 如果word1[i]=word[j] 显然dp[i][j]=dp[i-1][j-1]
// 如果是不相等的情况，因为要最少，那么迭代的状态肯定是上一个再+1
// 上个状态有dp[i-1][j-1]和dp[i-1][j],dp[i][j-1]
// 前者很好考虑，让word1[i]变成word2[j]就可以了
// 但后者可以是把word1[i]删掉，再通过dp[i-1][j]。综上，只需要选择其中最小的一种就行了
int minDistance(string word1, string word2) {
    int l1 = word1.size(), l2 = word2.size();
    vector<vector<int>> dp(l1+1, vector<int>(l2+1, 0));
    for (int i=0; i<=l1; i++) {
        dp[i][0] = i;
    }
    for (int j=0; j<=l2; j++) {
        dp[0][j] = j;
    }
    for (int i=1; i<=l1; i++) {
        for (int j=1; j<=l2; j++) {
            if (word1[i-1] == word2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = min(dp[i-1][j], min(dp[i-1][j-1], dp[i][j-1])) + 1;
            }
        }
    }
    return dp[l1][l2];
}
// 上一道的题的压缩
// 根据转移方程式，当前的点是通过上面一个，左边一个，左上方一个得到
// 显然上面一个，用外循环的上一次得到的dp[j]，左边一个，用这次循环的上一个dp[j-1]
// 那么左上就需要一个临时变量去记录：
// 每次在j的循环里面保存一个dp[j]，这个dp[j]显然是上一行的dp[i-1][j]
// 在更新完这一行的dp[i][j]后，这个记录相对于下一次就变成了dp[i-1][j-1]
// 同时，每次需要重新更新dp[0]和重置左上的数dp[0][0]
int minDistance(string word1, string word2) {
    int l1 = word1.size(), l2 = word2.size();
    vector<int> dp(l2+1, 0);
    for (int j=1; j<=l2; j++) {
        dp[j] = j;
    }
    for (int i=1; i<=l1; i++) {
        int prev = dp[0];
        dp[0] = i;    // dp[0]是dp[i][0]
        for (int j=1; j<=l2; j++) {
            int tmp = dp[j];
            if (word1[i-1] == word2[j-1]) {
                dp[j] = prev;
            } else {
                dp[j] = min(prev, min(dp[j],dp[j-1])) + 1;
            }
            prev = tmp;
        }
    }
    return dp[l2];
}

// m*n的矩阵，从左上角走到右下角有多少种方法，只能向右向下
// 显然 dp[i][j]=dp[i-1][j]+dp[i][j-1]
// 初始化 dp[i][1]=dp[1][j]=1
// 左边+上边，得到右下角
// 遍历顺序选择：从上到下，从左到右
// 上边的压缩dp[j]，左边dp[j-1]
int uniquePaths(int m, int n) {
    vector<int> dp(n+1);
    dp[1] = 1;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <=n; ++j) {
            dp[j] = dp[j-1] + dp[j];
        }
    }
    return dp[n];
}
// 因为只能向右走或者向下走，所以从一共需要的步数中挑出n-1个向下走，剩下的m-1个就是向右走
// 其实就是从（m-1+n-1）里挑选（n-1）或者（m-1）个，是一个组合问题


// 10 hard 正则表达式 s是字符串，p是模式，查看是否匹配成功
// '.'可以匹配任意单个字符(不能为空字符)，'*'可以让前面的字符出现0或者任意次(保证每次出现* 时，前面都匹配到有效的字符)
// 比如aa和a*aa返回true，ab和.*返回true（两个..）
// 一看就要用DP来做，而且dp的对象显而易见，dp[i][j]定义为前i个和前j个是否匹配
// 结果返回dp[n1][n2]，初始化：dp[i][0]=false，dp[0][0]=true，所以i和j都是从1开始遍历
// 当p[j-1]!='*': dp[i][j] = dp[i-1][j-1] && (s[i-1]==p[j-1] || p[j-1]=='.')
// 当p[j-1]=='*': 有两种情况，第1种让前面的字符不出现，第2种至少出现一次
// 则第一种情况:dp[i][j] = dp[i][j-2]
// 第二种情况：dp[i][j] = dp[i-1][j] && (s[i-1]==p[j-2] || p[j-2]=='.')
// 第二种假设：s是abbb，p是ab*，abb和ab*匹配成功，再判断abb的下一个b要和p的*的前一个字符是否相同
// 状态压缩，全图遍历，返回左下角，迭代顺序与上边，左上角，左边的左边得到
// 综上，从上往下，从左往右迭代
bool isMatch(string s, string p) {
    int n1 = s.size(), n2 = p.size();
    vector<bool> dp(n2+1, false); // dp[0][j]
    for (int i = 0; i <= n1; i++) { // 从0开始，因为dp[0][j]无法判断，毕竟*是可以匹配0个的
        bool prev = dp[0]; // 左上角，第i行第1列的左上角是第i-1行第0列，第0行的左上角无意义，都是false
        dp[0] = (i == 0); // dp[i][0]=false, dp[0][0]=true;
        for (int j = 1; j <= n2; j++) {
            bool tmp = dp[j]; // 未更新前的dp[j]是上一行的dp[i-1][j]
            if (p[j-1] != '*') dp[j] = (i > 0) && prev && (s[i-1] == p[j-1] || p[j-1] == '.');
            else dp[j] = dp[j-2] || ((i > 0) && dp[j] && (s[i-1] == p[j-2] || p[j-2] == '.')); // '*'不会出现在第一个，所以不用判断j>=2
            prev = tmp; // 对于下一个j来说，就是dp[i-1][j-1]
        }
    }
    return dp[n2];
}
// 44 hard 和10同类型，'?' 可以匹配任何单个字符，'*' 可以匹配任意字符串（包括空字符串）。
// 也就是把10的任意字符变成了任意字符串
// p[j-1]!='*'，和p[j-1]=='*'的第一种情况都一致，但是第二种情况会发生变化
// 因为可以匹配任意字符串，所以dp[i][j] = dp[i-1][j]，不需要要推到很前面
// 简单起见，不进行状态压缩
bool isMatch(string s, string p) {
    int n1 = s.size(), n2 = p.size();
    vector<vector<int>> dp(n1+1, vector<int>(n2+1, 0)); // dp[i][0]=false(except 0)
    dp[0][0] = 1;
    for (int j = 0; j < n2; ++j) {
        if (p[j] == '*') dp[0][j+1] = 1; // dp[0][j]不好判断，因为前缀多个*也是符合题意的
        else break;
    }
    for (int i = 1; i <= n1; i++) {
        for (int j = 1; j <= n2; j++) {
            if (isalpha(p[j-1])) {
                dp[i][j] = (s[i-1] == p[j-1]) && dp[i-1][j-1];
            } else if (p[j-1] == '?') {
                dp[i][j] = dp[i-1][j-1];
            } else {
                dp[i][j] = dp[i-1][j] || dp[i][j-1]; // 匹配任意字符串或空
            }
        }
    }
    return dp[n1][n2];
}

// 1092 hard 最短的公共Supersequence（子序列的反义词）
// 比如ab,bc的结果就是abc
// 这道题的思路就是利用LCS，最长公共子序列，不过这里要得到它，即需要知道DP的路径
string shortestCommonSupersequence(string &str1, string &str2) {
    string lcs = longestCommonSubsequence(str1, str2);
    string ret = "";
    int i = 0, j = 0;
    for (auto c = lcs.begin(); c != lcs.end(); c++, i++, j++) {
        while (*c != str1[i]) ret += str1[i++];
        while (*c != str2[j]) ret += str2[j++];
        ret += *c;
    }
    return ret + str1.substr(i) + str2.substr(j);
}
string longestCommonSubsequence(string text1, string text2) {
    int n1 = text1.size(), n2 = text2.size();
    vector<string> dp(n2+1, "");
    for (int i = 1; i <= n1; i++) {
        string prev = "";
        dp[0] = "";
        for (int j = 1; j <= n2; j++) {
            string tmp = dp[j];
            if (text1[i-1] == text2[j-1]) dp[j] = prev + text1[i-1];
            else dp[j] = dp[j].size() > dp[j-1].size() ? dp[j] : dp[j-1];
            prev = tmp;
        }
    }
    return dp[n2];
}