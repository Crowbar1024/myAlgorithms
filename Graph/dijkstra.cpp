#include "myheader.h"

// dijkstra其实是一种BFS加贪心的策略
// 1 每次找到没有访问过的节点中离起始点最近的那个点u，并且u有可能是之前用上某次的u更新距离得到的，也就是说不与src相连
// 2 然后通过它去更新没有访问过的节点与起始点的距离
// 关键是在步骤1结束后让搜索出来的节点设置为访问过
// 无法解决负权回路问题（一个点回到自己的路径之和是负数，这样只要循环，永远都会有最小）
// 外循环次数是N-1，N是总点数，-1是因为src点已知

// PAT 1003 src起点，dst目的，weightPoint点的权重，dist是{p1,p2,d}点与点之间的间距
// 保证src和dst之间存在至少一条路径，返回输出src和dst之间最短路径的个数，以及在这些最短路径中能累计点的权重之和最大的的权重之和
vector<int> emergency(int src, int dst, vector<int>& weightPoint, vector<vector<int>>& ptpDist) {
	int N = weightPoint.size(), M = ptpDist.size();  // N为点数（从0开始），M为边数
	int INF = 0x3f3f3f3f;
    vector<int> sumW = weightPoint;  // src到其他点的最短路径上的点权重之和
    vector<vector<int>> path(N, vector<int>(N, INF));  // 邻接矩阵，储存距离
    vector<int> dist(N, INF);  // src到其他点的最短距离
    vector<int> amount(N, 1);  // src到其他点的最短路径数量，需要初始化为1
    vector<int> isVisited(N, 0);
	for (int i = 0; i < M; ++i) {
		int c1 = ptpDist[i][0], c2 = ptpDist[i][1];
		path[c1][c2] = path[c2][c1] = ptpDist[i][2];
	}
	// 初始化最短距离和点权重之和
    for (int i = 0; i < N; ++i) {
		dist[i] = path[src][i];
		if (dist[i] != INF && i != src) {
			sumW[i] += weightPoint[src];
		}
	}
	isVisited[src] = 1;
	dist[src] = 0;
	// 更新每个点到src的最短距离
	int loopN = N-1;
	while (loopN--) {
		int tmp = INF, u = -1;
		// 得到下一个离src最近的点u，并得到最近距离MIN
		for (int j = 0; j < N; ++j) {
			if (!isVisited[j] && dist[j] < tmp) {
				tmp = dist[j];
				u = j;
			}
		}
		// 所有点都遍历过了，或者src到其他点都没有联通路径
		if (u == -1) break;
		isVisited[u] = 1;
		// 利用u来更新其他点到src的最短距离
		for (int j = 0; j < N; ++j) {
			if (isVisited[j] || path[u][j] == INF) continue;
			int newDist = dist[u]+path[u][j];
			int newSumW = sumW[u]+weightPoint[j];
			if (dist[j] > newDist) {
				dist[j] = newDist;
				amount[j] = amount[u];  // 更新后，到j的最短路径就是到u再到j，所以两者的最短路径数量一致
				sumW[j] = newSumW;
			} else if (dist[j] == newDist) {
				amount[j] += amount[u];  // 之前都不是通过u到j的，现在发现了又一条最短路径，加上它
				if (sumW[j] < newSumW) {
					sumW[j] = newSumW;
				}
			}
		}
	}
    return vector<int>{amount[dst], sumW[dst]};
}

// PAT 1087 src起点（默认0），dst目的，weightPoint点的权重（src权重为0，所以没有记录src），dist是{p1,p2,d}点与点之间的间距
// 输出src到dst的最短路径的数量和对应的距离
// 如果超过一条，输出路径上的点权重之和最大的那条路径
// 如果以上还超过一条，那么就输出平均点权重最大的那个
// 保证src和dst之间只存在一条上述的路径
// 最后输出上述满足条件的路径
// PAT1003进阶，+路径
vector<int> RomeRoads(int dst, vector<int>& weightPoint, vector<vector<int>>& ptpDist) {
	int N = weightPoint.size()+1, M = ptpDist.size();  // N为点数（从0开始），M为边数
	int INF = 0x3f3f3f3f;
	int src = 0;
    vector<int> sumW(N, 0);
	for (int i = 1; i < N; ++i) {
		sumW[i] = weightPoint[i-1];
	}
    vector<vector<int>> path(N, vector<int>(N, INF));
    vector<int> dist(N, INF);
    vector<int> amount(N, 1);
	vector<int> targetNum(N, 1);  // 最短路径上的点数，初始化为1
	vector<int> pre(N, 0);  // 点i的前一个点，让src的临点的pre都是src
    vector<int> isVisited(N, 0);
	for (int i = 0; i < M; ++i) {
		int c1 = ptpDist[i][0], c2 = ptpDist[i][1];
		path[c1][c2] = path[c2][c1] = ptpDist[i][2];
	}
    for (int i = 0; i < N; ++i) {
		dist[i] = path[src][i];
		// 不需要更新suumW，因为src权重为0
	}
	isVisited[src] = 1;
	dist[src] = 0;
	pre[src] = -1;
	int loopN = N-1;
	while (loopN--) {
		int tmp = INF, u = -1;
		for (int j = 1; j < N; ++j) {
			if (!isVisited[j] && dist[j] < tmp) {
				tmp = dist[j];
				u = j;
			}
		}
		if (u == -1) break;
		isVisited[u] = 1;
		// 不能pre[u]=0，因为这里的u不一定和src是相连的
		for (int j = 1; j < N; ++j) {
			if (isVisited[j] || path[u][j] == INF) continue;
			int newDist = dist[u]+path[u][j];
			int newSumW = sumW[u]+weightPoint[j-1];
			int newNum = targetNum[u] + 1;
			if (dist[j] > newDist) {
				dist[j] = newDist;
				amount[j] = amount[u];
				sumW[j] = newSumW;
				targetNum[j] = newNum;
				pre[j] = u;
			} else if (dist[j] == newDist) {
				amount[j] += amount[u];
				if (sumW[j] < newSumW || (sumW[j] == newSumW && targetNum[j] > newNum)) {
					sumW[j] = newSumW;
					targetNum[j] = newNum;
					pre[j] = u;
				}
			}
		}
	}
	vector<int> minPath;
	for (int i = dst; i != -1; i = pre[i]) {
		minPath.push_back(i);
	}
	reverse(minPath.begin(), minPath.end());
    return minPath;
}


// PAT 1018	bikeNum为节点数量[1,N]，src默认0不算在前者里，节点的最大承载bike数为K（保证偶数）
// 现在要从src出发到dst，首先要求是最短路径，让这条路径上（不包括src）所有节点的bike数为K/2
// 从原点拿若干bike，到sp的过程中修正所有点，记录修正后剩下的bike数量
// 如果最短路径不唯一，取一开始拿的bike最少的那条
// 如果还不唯一，取最后剩下的bike最少的那条
// 比较清晰，先用dijkstra得到最短路径，然后用dfs回溯
class bikeManage {
private:
	const int INF = 0x3f3f3f3f;
	int N, M;
	int src, dst;
	vector<vector<int>> path;
	vector<int> dist;
	vector<int> isVisited;
	int target;
	int minSendNum, minCollectNum;
	vector<int> targetNum;  // 期望路径达到平稳状态后的每个节点的bike数量
	vector<vector<int>> pre;  // 记录到每个节点的最短路径的前置节点
	vector<int> curPath;  // 当前遍历到的路径
	vector<int> minPath;  // 结果
	void Init(vector<int>& bikenum, int K, vector<vector<int>>& ptpDist) {
		N = bikenum.size()+1;
		M = ptpDist.size();
		target = K/2;
		minSendNum = minCollectNum = INF;
		path.resize(N, vector<int>(N, INF));
		dist.resize(N, INF);
		isVisited.resize(N, 0);
		targetNum.resize(N, 0);
		for (int i = 0; i < M; ++i) {
			int c1 = ptpDist[i][0], c2 = ptpDist[i][1];
			path[c1][c2] = path[c2][c1] = ptpDist[i][2];
		}
		for (int i = 1; i < N; ++i) {
			dist[i] = path[src][i];
			targetNum[i] = bikenum[i-1] - target;  // 全都置为0
		}
		isVisited[src] = 1;
		dist[src] = 0;
	}

	void dijkstra() {
		int loopN = N;
		while (loopN--) {
			int tmp = INF, u = -1;
			for (int j = 1; j < N; ++j) {
				if (!isVisited[j] && dist[j] < tmp) {
					tmp = dist[j];
					u = j;
				}
			}
			if (u == -1) break;
			isVisited[u] = 1;
			for (int j = 1; j < N; ++j) {
				if (isVisited[j] || path[u][j] == INF) continue;
				int newDist = dist[u]+path[u][j];
				if (dist[j] > newDist) {
					dist[j] = newDist;
					pre[j].clear();
					pre[j].push_back(u);
				} else if (dist[j] == newDist) {
					pre[j].push_back(u);
				}
			}
		}
	}
	// 下面的结构得背一下
	void dfs(int cur) {
		curPath.push_back(cur);
		// 此时到了起点0，得到一条最短路径
		if (!cur) {
			int sendNum = 0;  // 从src处要拿的bike数量
			int collectNum = 0;  // 最后会取回来的bike数量
			for (int i = curPath.size() - 1; i >= 0; i--) {
				int bikeNum = targetNum[curPath[i]];  // 最短路径中遍历到的节点上的bike数量
				// 多了可以先记到取回来的总数里
				if (bikeNum > 0) {
					collectNum += bikeNum;
				} else {
					// 把之前多的bike用于此处
					if (collectNum > -bikeNum) {
						collectNum += bikeNum;
					// src处一开始得多拿点
					} else {
						sendNum += -bikeNum - collectNum;
						collectNum = 0;
					}
				}
			}
			if (sendNum < minSendNum || (sendNum == minSendNum && collectNum < minCollectNum)) {
				minSendNum = sendNum;
				minCollectNum = collectNum;
				minPath = curPath;
			}
		} else {
			for (int i = 0; i < pre[cur].size(); i++) {
				dfs(pre[cur][i]);
			}
		}
		curPath.pop_back();
	}
public:
	vector<int> getMinPath(vector<int>& bikenum, int K, vector<vector<int>>& ptpDist) {
		Init(bikenum, K, ptpDist);
		dijkstra();
		dfs(dst);
		reverse(minPath.begin(), minPath.end());
		return minPath;
	}
};



// 已知src和dst，每个点之间的距离和花费；
// 找出一条路径，是最短距离，如果有多条，选花费最小的。（结果保证唯一）
// 一次dijkstra难以完成，因为开始无法初始化每个点的成本
class minDistWith {
private:
	const int INF = 0x3f3f3f3f;
	int N, M;
	int src, dst;
	vector<vector<int>> path;
	vector<vector<int>> cost;
	vector<int> dist;
	vector<int> isVisited;
	vector<vector<int>> pre;  // 记录到每个节点的最短路径的前置节点
	int minCostSum;
	vector<int> curPath;  // 当前遍历到的路径
	vector<int> minPath;  // 结果
	void Init(int src, int dst, int pointNum, vector<vector<int>>& ptpDist) {
		N = pointNum;
		M = ptpDist.size();
		minCostSum = 0;
		path.resize(N, vector<int>(N, INF));
		cost.resize(N, vector<int>(N, INF));
		dist.resize(N, INF);
		isVisited.resize(N, 0);
		for (int i = 0; i < M; ++i) {
			int c1 = ptpDist[i][0], c2 = ptpDist[i][1];
			path[c1][c2] = path[c2][c1] = ptpDist[i][2];
			cost[c1][c2] = cost[c2][c1] = ptpDist[i][3];
		}
		for (int i = 0; i < N; ++i) {
			dist[i] = path[src][i];
		}
		isVisited[src] = 1;
		dist[src] = 0;
	}

	void dijkstra() {
		int loopN = N-1;
		while (loopN--) {
			int tmp = INF, u = -1;
			for (int j = 0; j < N; ++j) {
				if (!isVisited[j] && dist[j] < tmp) {
					tmp = dist[j];
					u = j;
				}
			}
			if (u == -1) break;
			isVisited[u] = 1;
			for (int j = 0; j < N; ++j) {
				if (isVisited[j] || path[u][j] == INF) continue;
				int newDist = dist[u]+path[u][j];
				if (dist[j] > newDist) {
					dist[j] = newDist;
					pre[j].clear();
					pre[j].push_back(u);
				} else if (dist[j] == newDist) {
					pre[j].push_back(u);
				}
			}
		}
	}

	void dfs(int cur) {
		curPath.push_back(cur);
		// 此时到了起点0，得到一条最短路径
		if (cur != src) {
			int costSum = 0;
			// 最后一条边，需要i>=1
			for (int i = curPath.size() - 1; i > 0; i--) {
				// 从dst开始的路径
				int edgeP1 = curPath[i], edgeP2 = curPath[i-1];
				costSum += cost[edgeP1][edgeP2];
					
			}
			if (costSum < minCostSum) {
				minCostSum = costSum;
				minPath = curPath;
			}
		} else {
			for (int i = 0; i < pre[cur].size(); i++) {
				dfs(pre[cur][i]);
			}
		}
		curPath.pop_back();
	}
public:
	vector<int> getMinPath(int src, int dst, int pointNum, vector<vector<int>>& ptpDist) {
		Init(src, dst, pointNum, ptpDist);
		dijkstra();
		dfs(dst);
		reverse(minPath.begin(), minPath.end());
		return minPath;
	}
};
