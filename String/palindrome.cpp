#include "myheader.h"

// 回文有三种解法：
// 1 DP （分子串和子序列）
// 1.1 子串  dp[i][j]只能表示s[i...j]是否为回文子串，这样状态转移就可以从dp[i+1][j-1]更新
//          这样可以统计dp[i][j]为true的个数，这样需要最终跑到右上角dp[0][n-1]
//          这样dp[i][j] = (s[i]==s[j]) && (j-i<3 || dp[i+1][j-1])
//          j-i<3 就是 a aa aba 这三种情况，必然回文
//          由左下方得到，位于右上半部分，所以从下往上，从左到右迭代。位于矩阵右上半部分
// 1.2 子序列  dp[i][j]可以表示表示s[i...j]中回文子序列的个数
// 2 中心拓展  常用来统计回文子串个数
// 3 manacher


// 5 medium 最长回文子串的长度 manacher算法 
// 核心思想：在中心扩展算法的过程中，我们能够得出每个位置的臂长。那么当我们要得出以下一个位置 i 的臂长时，利用之前得到的信息
// 串   e b a b a b a b e
// 下标 l . j .mid. i . r
// 第二个a的回文半径为4（不算自己），当计算第三个a的臂长时，利用之前的信息。
// 首先计算第三个a的下标i相对于mid的镜像j，因为从左向右遍历，j的臂长已知为rds[j]，所以rds[i]至少为min(rds[j],r-i)
// 为什么r-i也是最短臂长的选项？证明过程如下：
// 如果不是的话，即i的臂长范围在以第二个a为中心的大回文串之外，如果i的最短臂长大于r-i，那么j的臂长同样，
// 假设一个点k是以j为回文中心的右边界，z是以i为回文中心的左边界，根据大回文串（以mid为中心）的性质，k和z中心对称
// 那么k对应的左边界k'和z对应的右边界z'也对称，所以相当于大回文串的臂长增大到了[k',z']，这不符合题意。
string longestPalindrome(string s) {
    int n = s.size();
    string ns(2*n+1,'#');
	for (int i = 0; i < n; ++i) ns[2*i+1] = s[i];
    int r = 0, mid = 0; // r为当前遍历到的节点中最远回文半径对应的下标，mid为对应的回文中心
	vector<int> rds(2*n+1, 0);  // 回文半径初始为0，表示不包括自己
	for (int i = 0; i < 2*n+1; ++i) {
        int j = 2*mid-i;  // 当前回文子串的镜像 {j ... mid ... i ... r}
        if (i < r) rds[i] = min(rds[j], r-i);  // 得到最短回文半径
		while (i-rds[i] > 0 && ns[i+rds[i]+1] == ns[i-rds[i]-1]) ++rds[i];  // 中心扩展回文半径
		if (i+rds[i] > r) {
			r = i+rds[i];
			mid = i;
		}
	}
    int cur = max_element(rds.begin(), rds.end()) - rds.begin();  // 最长臂长的回文中心位置
    int l = (cur-rds[cur])/2, len = rds[cur];
    return s.substr(l, len);
}

int manacher(string a) {
    string newA = "#";
	for (int i = 0; i < a.size(); i++) {
        newA += a[i];
        newA += "#";
	}
    int r = 0, s = 0, n = newA.size();
	vector<int> p(n, 0);
	for (int i = 0; i < n; ++i) {
        int i_mirror = 2*s - i;
        // i_mirror ... s ... i ... r
        if (r > i) {
            p[i] = min(p[i_mirror], r-i);  // i+p[i]不能超过r
        } else {
            p[i] = 0;
        }
        // 中心扩展法，注意一定要有下标约束
		while (i-p[i]-1 >= 0 && newA[i+p[i]+1] == newA[i-p[i]-1]) {
            ++p[i];
        }
		// 更新s和r
		if (i + p[i] > r) {
			r = i + p[i];
			s = i;
		}
	}
    int maxL = 0, center = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i] > maxL) {
            maxL = p[i];
            center = i;
        }
    }
    // return (maxL-center)/2;  // 返回最长回文子串的左边界下标
    return maxL;
}




// 647 medium 计算回文子串的个数 注意奇偶不同
int countSubstrings(string s) {
    int n = s.size(), cnt = 0;
    for (int i=0; i<n; i++) {
        cnt += extractPalindrome(s, i, i);
        cnt += extractPalindrome(s, i, i+1);
    }
    return cnt;
}
int extractPalindrome(string s, int left, int right) {
    int n = s.size();
    int cnt = 0;
    while (left>=0 && right<n && s[left]==s[right]) {
        left--;
        right++;
        cnt++;
    }
    return cnt;
}
// 



// 516 最长回文子序列（可以不连续）
// 看到最长这名字，思考DP的可行性
// 定义dp[i][j]为s[i...j]包含的最长回文子序列长度，返回dp[0][n-1]
// 如果是最长回文子串的话，这种DP方法就是错的
// 初始化：dp[i][i]=1
// 显然当s[i]==s[j]时，dp[i][j] = dp[i+1][j-1]+2
// 否则：dp[i][j] = max(dp[i+1][j],dp[i][j-1])
// 画出状态转移图，返回右上角，由左下角，下边，左边各一个得到，所有情况在右上半部分
// 所以迭代顺序，从下往上，从左向右
// 状态压缩，下边由上一轮大循环的dp[j]，左边由这次循环的dp[j-1]
// 左下角由一个临时变量存储
int longestPalindromeSubseq(string s) {
    int n = s.size();
    vector<int> dp(n, 1);
    for (int i=n-2; i>=0; i--) {
        int prev = 0;
        // dp[i] = 1;  // 可以省略，反正没有被更新
        for (int j = i+1; j < n; j++) {
            int tmp = dp[j];
            if (s[i] == s[j]) {
                dp[j] = prev + 2;
            } else {
                dp[j] = max(dp[j-1], dp[j]);
            }
            prev = tmp;
        }
    }
    return dp[n-1];
}
// 有道序号为5的最长回文子串，感觉应该分到array中，没啥技巧
// 基本都是遍历过去，每个节点向外检测，或者2个节点的向外检测




