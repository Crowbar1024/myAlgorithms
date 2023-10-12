#include "myheader.h"


// NIM博弈
// 小王喜欢与同事玩一些小游戏，今天他们选择了玩取石子。
// 游戏规则如下：共有N堆石子，已知每堆中石子的数量，并且规定好每堆石子最多可以取的石子数（最少取1颗）。
// 两个人轮流取子，每次只能选择N堆石子中的一堆，取一定数量的石子(最少取一个），并且取的石子数量不能多于该堆石子规定好的最多取子数，取走最后所有石子的人为赢家
// 假设每次都是小王先取石子，并且游戏双方都绝对聪明，现在给你石子的堆数、每堆石子的数量和每堆石子规定的单次取子上限，请判断出小王能否获胜。
// 先手必败：分别将P1，P2,P3,…,Pn对最多可以取的数+1取余，得到的值再执行异或操作
bool nimGame(int n, vector<int> stoneNum, vector<int> maxPickNum) {
    vector<int> p(n, 0);
    for (int i = 0; i < n; i++) {
        p[i] = stoneNum[i] % (maxPickNum[i]+1);
    }
    int remainder = p[0];
    for (int i = 1; i < n; i++)
        remainder ^= p[i];
    if (remainder) return true;
    return false;
}



// HDU2516 Fibonacci博弈
// 游戏是这样的：有一堆石子，两个人轮流从其中取走一定的石子，取走最后所有石子的人为赢家，遵循如下规则:
// 第一次取不能取完，至少取1颗。从第二次开始，每个人取的石子数至少为1，至多为对手刚取的石子数的两倍。
// n < 10^9
// 当n为Fibonacci数的时候，必败。
bool FibonacciGame(int n) {
    vector<int> p(60, 1);
    for (int i = 2; i < p.size(); ++i) {
        p[i] = p[i-1] + p[i-2];
        if (p[i] == n) return false;
        else if (p[i] > n) break;
    }
    return true;
}