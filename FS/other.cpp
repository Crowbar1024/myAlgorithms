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


// 1376 medium n个员工，每个员工i有一个上司managet[i]，通知自己的下属要informtime[i]
// headID是最高一级的上司，即manager[headID]=-1。没有下属的员工i的informtime为0
// 问headid通知一件事情，所有员工都知道的时间是多久
class Solution {
public:
	// 自顶向下，dfs返回cur节点通知完子树的时间
    int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
		unordered_map<int, vector<int>> get_children;
		for (int i = 0; i < manager.size(); ++i) {
			if (manager[i] == -1) continue;
			get_children[manager[i]].emplace_back(i);
		}
		function<int(int)> dfs = [&](int cur) -> int {
			int ret = 0;
			for (int child : get_children[cur]) {
				ret = max(ret, dfs(child)); // 多个儿子选择最慢的
			}
			return ret + informTime[cur]; // 加上自己通知父亲的时间
		};
		return dfs(headID);
    }
	// 自底向上，dfs返回通知到自己的时间
	int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
		unordered_map<int, int> time;
		function<int(int)> dfs = [&](int cur) -> int {
			if (cur == headID) return 0;
			if (time.count(cur)) return time[cur];
			return time[cur] = dfs(manager[cur]) + informTime[manager[cur]];
		};
		int ret = 0;
		for (int i = 0; i < n; ++i) {
			ret = max(ret, dfs(i));
		}
		return ret;
    }
	// 不要hashmap去统计，复用informtime
	// 这样的话会带来一个问题，如果复用成到自己节点的路径长度，那么原来的通知自己儿子的信息就会被丢失
	// 所以把dfs的返回值改成，返回到自己的时间再加上自己通知儿子的时间
	int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
		function<int(int)> dfs = [&](int cur) -> int {
			if (manager[cur] < 0) return informTime[cur];
			informTime[cur] += dfs(manager[cur]);
			manager[cur] = -1; // 自己的父节点往上的路径，都遍历过了，将自己的父亲可以记忆下来，以后直接返回。
			return informTime[cur];
		};
		int ret = 0;
		for (int i = 0; i < n; ++i) {
			ret = max(ret, dfs(i));
		}
		return ret;
    }
	// 上面的递归写成迭代
	int numOfMinutes(int n, int headID, vector<int>& manager, vector<int>& informTime) {
		int ret = 0;
		for (int i = 0; i < n; ++i) {
			int time = 0, fa = i;
			for (; manager[fa] >= 0; fa = manager[fa]) {
				time += informTime[fa];
			}
			time += informTime[fa]; // fa此时要么是headID，要么是之前记忆过的
			ret = max(ret, time);
			// 当前节点向上进行更新和记忆
			for (fa = i; manager[fa] >= 0;) {
				int tmp_informTime = time;
				time -= informTime[fa];
				informTime[fa] = tmp_informTime; // 记忆当前的时间
				int tmp_manager = manager[fa];
				manager[fa] = -1;
				fa = tmp_manager; // 跳到上一层，所以for循环第三处不能写，否则就越界了
			}
		}
		return ret;
    }
};
