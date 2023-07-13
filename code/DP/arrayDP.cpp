#include "myheader.h"



// 53 medium 最大子串和（有负数）
// 以i结尾的连续子数组的最大和dp[i] = max(nums[i], dp[i-1]+nums[i])
int maxSubArray(vector<int>& nums) {
    int dp = 0, ret = INT_MIN;
    for (int i = 0; i < nums.size(); ++i) {
        dp = max(nums[i], dp+nums[i]);
        ret = max(dp, ret);
    }
    return ret;
}
// 衍生问题
// 1给定一个数组，值为正，返回累加和为给定值k的最长子数组长度（使用滑动窗口可解）
// 2给定一个数组，值可以为正负零，返回累加和为给定值k的最长子数组长度
// 计算位置i的前缀和为sum，如果i之前的某个位置j的前缀和为sum-k，那么必然[j+1:i]的子数组的累加和为k
// 值得注意的是，如果头节点就是k，但头节点之前没有节点前缀和的统计，所以需要加一个{-1,0}的哨兵
// 用一个hashmap去保存{sum, index}
// 如果当前的sum已经在map中存在，不会保存，因为需要返回的使最长的
int maxLength(vector<int>& a, int k) {
    if (a.empty()) return 0;
    int n = a.size();
    int sum = 0, ret = 0;
    unordered_map<int,int> m;
    m[0] = -1;
    for (int i=0; i<n; ++i) {
        sum += a[i];
        if (m.count(sum-k)) {
            ret = max(ret, i-m[sum-k]);
        }
        if (!m.count(sum)) {
            m[sum] = i;
        }
    }
    return ret;
}



// 3 medium 给定一个数组，值可以为正负零，返回累加和小于等于定值k的最长子数组长度
// 时间复杂度：O(n)
// 这里需要分为两步:
// 第一步：从后向前计算，把问题变成以某个位置结尾和的最小子串的长度。
// index     0  1  2  3  4  5  6  7
// value     4  3 -2  6  7 -3 -1 -2
// min_sum   4  1 -2  6  1 -6 -3 -2  (从后往前)
// min_index 0  2  2  3  1  7  7  7
// 第二步：从0到N逐一判断刚才最小长度是否可以合并在一起达到小于等于k的效果。
// 注意：1，从0-N开始位置index，一个一个遍历。
// 2，每当遇到一个开始位置index = i时, end = min_index[i]，
// 判断当前sum+=min_sum的大小是否<=K,如果<=K则尝试着往右继续扩展加上  end = min_index[i] + 1,获得其sum += min_sum[i+1]
// 如果最终的结果继续<=k；则继续向右扩展。如果，>=k则跳转到第3步，
// 3,如果 sum>=k；则右边不动，左边缩一位，也就是index = i+1;sum-=value[i];再次判断其sum是否<=k;
// 如果继续<=k，则到第二步。如果不是则继续第三步。
int minSumAndIndex(vector<int>& a, int k) {
    if (a.empty()) return -1;
    int n = a.size();
    vector<int> minSum(n,0);
    minSum[n-1] = a[n-1];
    unordered_map<int,int> m;  // 记录{min_sum, min_index}
    m[n-1] = n-1;
    for (int i=n-2; i>=0; --i) {
        if (minSum[i+1] < 0) {
            minSum[i] = a[i] + minSum[i+1];
            m[i] = m[i+1];
        } else {
            minSum[i] = a[i];
            m[i] = i;
        }
    }
    int end = 0;  // 几个累加和小于k的块的下一个
    int sum = 0;
    int ret = 0;
    for (int i=0; i<n; ++i) {
        while (end<n && sum+minSum[end]<=k) {
            sum += minSum[end];
            end = m[end] + 1;
        }
        //这里是为了防止刚开始的时候end==i,就不满足，这样sum没有被加过，所以一直为0，所以不需要减去任何值
        sum -= end > i ? a[i] : 0;
        ret = max(ret, end - i);
        //这里是为了防止刚开始的时候end==i,需要把end值往后移动一个位置，向右扩展。i是自增的
        end = max(end, i + 1);
    }
    return ret;
}






// 21 字节笔试 优美数组
// a是一组数，长度小于1000，若a[i]*a[j]=a[k]，i<j<k，则称为优美序列，保证a的长度大于2
// a最长的优美子串是多长，如果有多个，返回其个数
// dp[i][j]为以下标i结尾，前一个数下标是j的优美序列的长度
// 和139类似，往后好写，需要改变顺序
// 当然我觉得我的方法肯定不是最优解
vector<int> getBeautifulArray(vector<int> a) {
    int n = a.size();
    vector<vector<int>> dp(n+1, vector<int>(n+1, 0));
    for (int i = 2; i <= n; ++i) {
        for (int j = i-1; j >= 1; --j) {
            dp[i][j] = 2;
        }
    }
    for (int i = 3; i <= n; ++i) {
        for (int j = i-1; j >= 2; --j) {
            for (int k = j-1; k >= 1; --k) {
                if (a[i-1] == (a[j-1]*a[k-1])%10) {
                    dp[i][j] = max(dp[i][j], dp[j][k]+1);
                }
            }
        }
    }
    int maxLen = 2, sum = 1;
    for (int i = 3; i <= n; ++i) {
        for (int j = i-1; j >= 2; --j) {
            if (dp[i][j] > maxLen) {
                maxLen = dp[i][j];
                sum = 1;
            } else if (dp[i][j] == maxLen) {
                ++sum;
            }   
        }
    }
    return vector<int>{maxLen, sum};
}


