#include "myheader.h"


// 494 medium 对于非负nums中的任意元素，可以添加+/-在前面。现在要得到目标数S，返回所有添加符号的方法数。
// 感觉和1049很像，分成2堆，一堆比另一堆大S
// sum1=sum2+S && sum1+sum2=sum
// 得到 v=(sum+S)/2
// 定义dp[v]为恰好装满v背包的方法数
// dp[i][j]=dp[i-1][j]+dp[i-1][j-nums[i]]
int findTargetSumWays(vector<int>& nums, int target) {
    int sum = accumulate(nums.begin(), nums.end(), 0) + target;
    if (sum < 0 || sum & 1) return 0;  // 不存在
    int v = sum/2;
    vector<int> dp(v+1, 0);  // 恰好的种类数
    dp[0] = 1;
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = v; j >= nums[i]; --j) {
            dp[j] += dp[j-nums[i]];
        }
    }
    return dp[v];
}


// 96 medium 给定一个整数n，求以1 ... n为节点组成的二叉搜索树有多少种？
// 这种题目只能找找规律
// n=1 1
// n=2 2（1为节点，2为节点）
// n=3 2+1+2 （1为节点有2种，2为节点有1种，3为节点有2种）
// 即当有i个节点时，选择j当root，那么它的左子树的数量就是dp[j-1]，右子树的数量就是dp[i-j]
int numTrees(int n) {
    vector<int> dp(n+1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            dp[i] += dp[j-1]*dp[i-j];
        }
    }
    return dp[n];
}


// 887 hard K个蛋，N层楼，已知存在楼层F，满足[0,n]，任何从高于f的楼层落下的鸡蛋都会碎，从f楼层或比它低的楼层落下的鸡蛋都不会破。
// 每次操作，你可以取一枚没有碎的鸡蛋并把它从楼层x扔下（满足[1,n]）。
// 如果鸡蛋碎了，你就不能再次使用它。如果某枚鸡蛋扔下后没有摔碎，则可以在之后的操作中重复使用这枚鸡蛋。
// f确切的值的最小操作次数是多少？
// 定义dp[i][j]：测试i次，有j个蛋可以确定的楼层f。找到dp[i][K]=N时的i
// 假设当前摔蛋的是f层。分类讨论
// 1 鸡蛋破了，那么只能测[1,m-1]层下面的楼，所以+dp[i-1][j-1]，
// 2 没破，那么只能测[m+1,N]这段的楼，可以假设这段就是从1楼开始的，所以+dp[i-1][j]
// 综上：dp[i][j]可以确定的楼数： dp[i-1][j] + dp[i-1][j-1] + 1（f层自己）
// 由上，左上得到，从上到下，状态压缩后，内循环要这里要从右往左
// 边界条件：第0行 dp[0][j]=0 测试0次，f=0
// 第0列 dp[i][0]=0 没有蛋，自然为0
int superEggDrop(int K, int N) {
    vector<int> dp(K+1, 0);  
    int i = 0;
    for (; dp[K] < N; i++) {
        for (int j = K; j > 0; j--) {
            dp[j] += dp[j-1] + 1;
        }
    }
    return i;
}


// 877 一堆不同重量的石头，总重量是奇数，每次只能拿两侧的石头，我先挑，请问我的石头能否最重
// 如果石头堆数是偶数，我可以一直拿到奇数或者一直拿到偶数编号的石头。但奇偶拿到的石头数量不等，肯定赢
// 这道题的难点在于想到用DP。所以我觉得应该配得上hard
// 定义dp[i][j]为某人能从[i,...,j]石头堆里能拿到的超过他对手石头的最大重量，是个差值
// 如果我先拿，就是我超过我对手的最大重量；如果对手先拿，就是对手超过我的最大重量
// 返回值：dp[0][n-1]，因为我先拿，那么只要判断这个数是正就可以判断我赢
// 初始化，dp[i][i]=piles[i]
// 那么根据题意，状态转移方程如下所示：
// dp[i][j] = max(piles[i]-(dp[i+1][j], piles[j]-dp[i][j-1])
// 可以见由下边，左边各一个生成，并且上半部分，所以迭代顺序：从下往上，从左向右
// 所以进行状态压缩的话，下边的是上一轮的大循环的dp[j]，左边的是这一轮循环的dp[j-1]
// 由于压缩后，反正y=-x轴这条线都取不到，所以直接拿pile去初始化dp，这样还省去了对边界的每次更新重写（其实一般的题目也不会）
bool stoneGame(vector<int>& piles) {
    int n = piles.size();
    if (piles.size()&1 == 0) return true;
    vector<int> dp = piles;    // base case dp[i][i]=piles[i]
    for (int i=n-2; i>=0; i--) {
        for (int j=i+1; j<n; j++) {
            dp[j] = max(piles[i]-dp[j], piles[j]-dp[j-1]);
        }
    }
    return dp[n-1] > 0;
}
// 一样，现在返回优胜者的石头重量，下面是傻瓜版
int stoneGame(vector<int>& piles) {
    if (piles.empty()) return 0;
    int sum = accumulate(piles.begin(), piles.end(), 0);
    int firstTakePrice = firstTake(piles, 0, piles.size()-1);
    return max(firstTakePrice, sum-firstTakePrice);
}
int firstTake(vector<int>& piles, int i, int j) {
    if (i == j) return piles[i];
    return max(piles[i]+secondTake(piles,i+1,j), piles[j]+secondTake(piles,i,j-1));
}
int secondTake(vector<int>& piles, int i, int j) {
    if (i == j) return 0;  // 剩下的唯一一个被拿走了
    // 由于是后发，拿走谁取决于先发者，他拿完了，我就成了先发者
    // 由于双方都是理性个体，先发者肯定让后发者先发取最下的数
    return min(firstTake(piles,i+1,j), firstTake(piles,i,j-1));
}
// 将上述的方法化简，由于递归过程中会重复调用，使用两张二维表将firstTake和secondTake给存放
// 显然最后需要的是这两张表右上角的值，且对角线的值就是i==j的情况
// 由递归可见，每张表的每个节点都是由下方和左方的值递推而来
// 所以遍历的顺序就是从左往右，从下到上
int stoneGame(vector<int>& piles) {
    if (piles.empty()) return 0;
    int sum = accumulate(piles.begin(), piles.end(), 0);
    int firstTakePrice = firstTake(piles, 0, piles.size()-1);
    return max(firstTakePrice, sum-firstTakePrice);
}
// 对傻瓜版化简，只需要一个递归函数
int firstTake(vector<int>& piles, int i, int j) {
    if (i == j) return piles[i];
    else if (i+1 == j) return max(piles[i],piles[j]);
    // 一样的思路，下一次我先发的其实是上一次的后发，双方都是理性个体，所以第二次先发拿到的要取最小
    return max(piles[i] + min(firstTake(piles,i+2,j),firstTake(piles,i+1,j-1)),
            piles[j] + min(firstTake(piles,i,j-2),firstTake(piles,i+1,j-1)));
}
// 然后对其进行列表化，顺序是斜的，左上到右下（因为一个节点由左下三个点得到）