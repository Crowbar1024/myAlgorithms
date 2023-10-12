#include "myheader.h"

// 373 medium nums1,2是升序的数组，返回其中两两组合前k小的组合
// 思路和378的PQ解法一致
struct node {
    int i,j;
    int sum;
    bool operator<(const node& a) const{
        return sum > a.sum;
    }
};
vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
    int m = nums1.size(), n = nums2.size();
    priority_queue<node> pq;
    vector<vector<int>> ret;
    for (int i = 0; i < m; ++i) {
        pq.push(node{i,0,nums1[i]+nums2[0]});
    }
    while (k-- && !pq.empty()) {
        node tmp = pq.top();
        pq.pop();
        ret.emplace_back(vector<int>{nums1[tmp.i], nums2[tmp.j]});
        if (tmp.j < n-1) {
            pq.push(node{tmp.i,tmp.j+1,nums1[tmp.i]+nums2[tmp.j+1]});
        }
    }
    return ret;
}

// 字节笔试21/4/25 3 easy
// 一组pair，耗时（单位为天），延期后每天赔偿的奶茶数
// 求一组赔偿奶茶最少的顺序
// a的耗时乘b的赔偿<b的耗时乘a的赔偿
// 相等的话，id小的排在前面
// 我当时理解了很长时间，后来发现特别简单，有两个工程a和b
// 如果先做了a，那么一共的赔偿就是a的耗时乘b的赔偿


// 1167 medium
// 为了装修新房，你需要加工一些长度为正整数的棒材 sticks。
// 如果要将长度分别为 X 和 Y 的两根棒材连接在一起，你需要支付 X + Y 的费用。 
// 由于施工需要，你必须将所有棒材连接成一根。返回你把所有棒材 sticks 连成一根所需要的最低费用。
// 注意你可以任意选择棒材连接的顺序。
// 贪心+哈夫曼编码，实现方法小顶堆
// 哈夫曼编码：简单来说就是一堆数，合并的代价是两个节点之和，为了使总代价最小，每次取最小的两个节点合并
int connectSticks(vector<int>& sticks) {
	int ret = 0;
	priority_queue<int, vector<int>, greater<int>> q(sticks.begin(), sticks.end());
	while (q.size() > 1) {
		int f1 = q.top(); q.pop();
		int f2 = q.top(); q.pop();
		q.push(f1+f2);
		ret += f1+f2;
	}
	return ret;
}
// 延伸问题：
// 一块60重的金条，分为10 20 30，每次分割金条的代价就是被分割金条的长度，问分成这样的最小代价
// 60-50+10 50-20+30
// 60-30+30 30-10+20 这样最小
// 和原题相反


// 左神课
// 一个数据结构，满足插入一个数时间复杂度Ologn，取其中位数O1
// 用两个优先队列，一个大顶堆存放小的一半，一个小顶堆存放大的一半，然后取其堆头作比较
// 先放大顶堆，新的数如果比大顶堆堆头小，放入大顶堆，若大，放到小顶堆
// 为了满足条件，当两个堆size差大于1时，把大顶堆的堆头放入小顶堆

// 左神课
// 一堆工程，{c,p}分别为工程的成本和去掉成本的纯利润，和自己的初始资金
// 比如有工程{2,3}，初始资金2，接了这个工程后我的资源为2+3=5
// 现在有n个工程，我最多可以串行完成k个，问最后最多有多少资金
// 一个小顶堆，key为c；一个大顶堆，key为p
// 每次从小顶堆中把前m个c小于当前资金的元素放入大顶堆，然后从大顶堆拿出堆头做，资金增加
// 这样做k次
// 贪心中machineTask也可以用大小顶堆的思想去做
struct project {
    int c, p;
};
int doProject(vector<int> a, vector<int> b, int initialC, int k) {
    auto cmp1 = [](const project& x, const project& y){ return x.c>y.c; };  // 小顶堆
    auto cmp2 = [](const project& x, const project& y){ return x.c<y.c; };  // 大顶堆
    priority_queue<project, vector<project>, decltype(cmp1)> pq1(cmp1);
    priority_queue<project, vector<project>, decltype(cmp2)> pq2(cmp2);
    int n = a.size();
    int currentC = initialC;
    for (int i=0; i<n; ++i) {
        project tmp{a[i], b[i]};
        pq1.push(tmp);
    }
    while (k--) {
        while (!pq1.empty() && pq1.top().c <= currentC) {
            pq2.push(pq1.top());
            pq1.pop();
        }
        currentC += pq2.top().p;
        pq2.pop();
    }
    return currentC;
}


// hard 239 有一个大小为k的滑动窗口从数组nums的最左侧移动到数组的最右侧。
// 你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。返回每次滑动窗口中的最大值。
// 我一开始想用一个pair去储存滑动窗口中的最大值，但显然如果这个数没了，又需要得到次大值。
// 所以需要一个有序的数据结构去存储滑动窗口的pair，第一个想到的是优先队列。
// 因为要查看对头是否已经不在了，所以需要统计pair
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    priority_queue<pair<int,int>> pq;
    for (int i = 0; i < k; ++i) {
        pq.emplace(nums[i], i);
    }
    vector<int> ret{pq.top().first};
    for (int i = k; i < nums.size(); ++i) {
        pq.emplace(nums[i], i);
        while (pq.top().second <= i-k) {
            pq.pop();
        }
        ret.emplace_back(pq.top().first);
    }
    return ret;
}

// 347 medium 返回nums其中出现频率前 k 高的元素。
// 统计频率，使用unordered_map；排序，打包成pair放入优先队列，根据题意是小顶堆。
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> cnt;
    for (const int& num : nums) cnt[num] += 1;
    auto cmp = [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;  // 小顶堆
    };
    priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
    for (const auto& cur : cnt) {
        pq.emplace(cur);
        if (pq.size() > k) pq.pop();
    }
    vector<int> ret(k);
    for (int i = k-1; i >= 0; --i) {  // 把最小的放末尾
        ret[i] = pq.top().first;
        pq.pop();
    }
    return ret;
}
// 692 medium int变成string 返回的答案应该按单词出现频率由高到低排序。如果不同的单词有相同出现频率，按字典顺序排序。
vector<string> topKFrequent(vector<string>& words, int k) {
    unordered_map<string,int> cnt;
    for (const string& s : words) cnt[s] += 1;
    auto cmp = [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second == b.second ? a.first < b.first : a.second > b.second;  // 注意优先队列是反过来
    };
    priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(cmp)> pq(cmp);
    for (const auto& cur : cnt) {
        pq.emplace(cur);
        if (pq.size() > k) pq.pop();
    }
    vector<string> ret(k);
    for (int i = k-1; i >= 0; --i) {  // 把最小的放末尾
        ret[i] = pq.top().first;
        pq.pop();
    }
    return ret;
}