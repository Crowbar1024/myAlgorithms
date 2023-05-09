//-----------------------------------【程序说明】----------------------------------------------
//  题目来源：HDUOJ 2089
//	2018年4月18日	Create by 姚昊
/*  描述：
	[n,m]中没有4和62的数的个数
	数分DP入门题
*/
//-------------------------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include <vector>
#include <stack>
#include <queue> 
#include <string>
#include <functional>

using namespace std;
const int INF = 0x3f3f3f3f;
typedef long long ll;

int a[20];
int dp[20][2];

//pos：枚举到第pos位，pre:前一位是多少，state:1表示前一位是6，limit:数位上限
int dfs(int pos, int pre, int state, bool limit) {
	if (pos < 0) return 1;
	if (!limit && dp[pos][state] != -1) return dp[pos][state];
	int up = limit ? a[pos] : 9;
	int res = 0;
	for (int i=0; i<=up; i++) {
		if (i == 4) continue;
		if (pre == 6 && i == 2) continue;
		res += dfs(pos-1, i, i==6, limit && i==a[pos]);
	}
	if (!limit) dp[pos][state] = res;
	return res;
}

int solve(int x) {
	int pos=0;
	while (x>0) {
		a[pos++] = x%10;
		x /= 10;
	}
	return dfs(pos-1, -1, 0, true);
}

int main()  {
//	freopen("input.txt","r",stdin); 
	int n,m;
	while (~scanf("%d %d",&n,&m) && n && m) {
		memset(dp, -1, sizeof(dp));
		printf("%d\n", solve(m)-solve(n-1));
	}
	return 0;
}
