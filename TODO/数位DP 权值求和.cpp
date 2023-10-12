//-----------------------------------������˵����----------------------------------------------
//  ��Ŀ��Դ��HDUOJ 2089
//	2018��4��18��	Create by Ҧ�
/*  ������
	[a,b],f(x) = x��ÿһλ*2^i�����
	������f(x)<=f(a)��[0,b]�ĸ���
	����DP������
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
const int maxn=1e4+5;

int a[20];
int dp[20][maxn];
int thd;

int f(int a) {
	if (a == 0) return 0;
	int ans = f(a/10);
	return ans*2+(a%10);
}

//pos��ö�ٵ���posλ��sum:ö�ٵ���һλʱ�ļ�Ȩ�ͣ�limit:��λ����
int dfs(int pos, int sum, bool limit) {
	if (pos < 0) return sum <= thd;
	if (sum > thd) return 0;
	if (!limit && dp[pos][thd-sum] != -1) return dp[pos][thd-sum];
	int up = limit ? a[pos] : 9;
	int ret = 0;
	for (int i=0; i<=up; i++) {
		ret += dfs(pos-1, sum+i*(1<<pos), limit&&i==a[pos]);
	}
	if (!limit) dp[pos][thd-sum] = ret;
	return ret;
}

int solve(int x) {
	int pos=0;
	while (x>0) {
		a[pos++] = x%10;
		x /= 10;
	}
	return dfs(pos-1, 0, true);
}

int main()  {
//	freopen("input.txt","r",stdin); 
	int t, k=0, a, b;
	scanf("%d",&t);
	while (t--) {
		memset(dp, -1, sizeof(dp));
		printf("Case #%d: ",++k);
		scanf("%d %d", &a, &b);
		thd = f(a);
		printf("%d\n", solve(b));
	}
	return 0;
}

