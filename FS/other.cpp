#include "myheader.h"



// 字节校招，给定两个正整数a=b=1和指定的正整数n，a和b可以进行一下两种操作
// 1 b=a, a<<=1; 2 b+=a;
// 问最少多少次可以使a=n?
// 类似迷宫，两种操作就是方向，最短距离就是最少次数，只不过次数只能用map统计
int operationOnTwo1(int n) {
    queue<pair<int,int>> q;
	q.push({1, 1});
	map<pair<int,int>, int> cnt;
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		if (cur.first == n) {
			return cnt[cur];
			break;
		}
		pair<int,int> tmp;
		tmp.first = cur.first * 2;
		tmp.second = cur.first;
		if (!cnt.count(tmp)) {
			q.push(tmp);
			cnt[tmp] = cnt[cur] + 1;
		}
		tmp.first = cur.first + cur.second;
		tmp.second = cur.second;
		if (!cnt.count(tmp)) {
			q.push(tmp);
			cnt[tmp] = cnt[cur] + 1;
		}
	}
    return -1;
}
