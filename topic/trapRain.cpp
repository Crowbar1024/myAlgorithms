#include "myheader.h"

// 11 medium 很细的棍子（棍子体积忽略）表示数组数的高度，找出其中的两条线，使得它们与x轴共同构成的容器可以容纳最多的水。
// 注意，只是挑选两个线，其他线都可以忽略，即找到满足最大的 ret=(r-l)*min(l,r) 
// 所以这道题和接水没有半毛钱关系
// 先固定l和r是两边的，因为这样向内收缩必然有最优解
// 假设固定l，左移r，且一直保持h[l]<=h[r]，那么res一定变小。所以此时l只能右移到比原来固定的h[l]大的地方才能有概率让res变大
// 假设固定r，右移l，且一直保持h[l]>=h[r]，那么res一定变小。所以同理r必须左移
int maxArea(vector<int>& height) {
	int l = 0, r = height.size()-1, ret = 0;
	while (l < r) {
		int minh = min(height[l], height[r]);
		ret = max(ret, minh*(r-l));
		while (height[l] <= minh && l < r) ++l;
		while (height[r] <= minh && l < r) --r;
	}
	return ret;
}



// 42 hard 真正的接雨水，但是棍子很粗，占满了格子，所以雨水落到棍子围成的空间
int trap(vector<int>& height) {
	int l = 0, r = height.size()-1;
	int lmax = 0, rmax = 0, ret = 0;
	while (l < r) {
		lmax = max(lmax, height[l]); // [0,l]最高
		rmax = max(rmax, height[r]); // [r,n)最高
		if (lmax > rmax) ret += rmax-height[r--]; // [lmax(l),l,r,rmax(r)]，此时在r位置存水
		else ret += lmax-height[l++];
	}
	return ret;
}



// 407 hard 3D接雨水
// 木桶理论，四周最短就是能存的水高度。所以每次要找到最小高度的柱子，更新它四周的水高度。所以用最小堆
// 最开始用四周，然后向内收缩
int trapRainWater(vector<vector<int>>& heightMap) {
	int m = heightMap.size(), n = heightMap[0].size();
	if (m <= 2 || n <= 2) return 0; // 2行和2列不可能存水
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
	vector<vector<bool>> vis(m, vector<bool>(n, false));
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i==0 || i==m-1 || j==0 || j==n-1) { // 边界不可能接水，所以从边界开始初始化
				pq.emplace(heightMap[i][j], i*n+j);
				vis[i][j] = true;
			}
		}
	}
	const vector<int> dir{0,-1,0,1,0};
	int ret = 0;
	while (!pq.empty()) {
		auto cur = pq.top(); pq.pop();
		for (int i = 0; i < 4; ++i) {
			int nx = cur.second/n+dir[i];
			int ny = cur.second%n+dir[i+1];
			if (nx<=0 || nx>=m-1 || ny<=0 || ny>=n-1 || vis[nx][ny]) continue;
			if (heightMap[nx][ny] < cur.first) {
				ret += cur.first-heightMap[nx][ny];
				heightMap[nx][ny] = cur.first; // 更新内部的格子
			}
			vis[nx][ny] = true;
			pq.emplace(heightMap[nx][ny], nx*n+ny);
		}
	}
	return ret;
}
