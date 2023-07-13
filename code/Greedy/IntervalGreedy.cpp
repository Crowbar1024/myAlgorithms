#include "myheader.h"

// 495 easy 提莫Q了多次寒冰（时间非降序），Q的致盲时间为duration，每次Q都会重置致盲时间，一共致盲多久
// 维护每次致盲时间的右边界
int findPoisonedDuration(vector<int>& timeSeries, int duration) {
    int r = timeSeries[0]+duration, ret = duration; // 保存最后一次持续时间
    for (int i = 1; i < timeSeries.size(); ++i) {
        if (r > timeSeries[i]) ret += timeSeries[i]-timeSeries[i-1];
        else ret += duration;
        r = timeSeries[i]+duration; // 更新
    }
    return ret;
}


// 56 medium 合并区间 给出一个区间的集合，请合并所有重叠的区间。（左右边界重合算重叠）
// 排序左边界
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    if (intervals.size() == 1) return intervals;
    sort(intervals.begin(), intervals.end(), [](const vector<int> &p1, const vector<int> &p2) {
        if (p1[0] == p2[0]) return p1[1] < p2[1];
        return p1[0] < p2[0];
    });
    vector<vector<int>> ret;
    int l = intervals[0][0], r = intervals[0][1];
    for (int i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] > r) { // 当前遍历到了新区间
            ret.emplace_back(vector<int>{l,r}); // 存入老区间
            l = intervals[i][0];
        }
        r = max(r, intervals[i][1]); // 所有情况，1 在内部 2 右边界增大 3 新区间 都只需要判断r是否需要更新就行
    }
    ret.emplace_back(vector<int>{l,r});  // 存入新区间
    return ret;
}

// 57 medium 无重叠（端点不重合）的有序排序的区间列表，插入一个新区间，可以合并重叠的，返回结果区间列表
// 难点在于区间合并到最后一个区间时，有多种情况，所以干脆不插入，在不相交时插入重叠后的区间
vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
    if (intervals.empty()) return vector<vector<int>>{newInterval};
    vector<vector<int>> ret;
    int l = newInterval[0], r = newInterval[1];
    bool flag = false; // 重叠后，在下一个不相交插入之前插入重叠的区间
    for (const vector<int>& line : intervals) {
        if (line[1] < l) { // 当前线段在新区见的左侧不相交
            ret.emplace_back(line);
        } else if (line[0] > r) { // 当前线段在新区见的右侧不相交
            if (!flag) { // 之前没有插入合并后的区间
                ret.emplace_back(vector<int>{l, r});
                flag = true;
            }
            ret.emplace_back(line);
        } else { // 相交的区间
            l = min(l, line[0]);
            r = max(r, line[1]);
        }
    }
    if (!flag) ret.emplace_back(vector<int>{l, r}); // 避免最后一个区间也要重叠
    return ret;
}


// 452 medium 射箭，<a,b>表示x轴的线段，最少从x轴向上射几根箭，可以戳爆所有线
// 右边界排序，下面这种情况是射两根，所以如果新的是重叠，不需要更新右边界
// ------
//   -------
//         ---
int findMinArrowShots(vector<vector<int>>& points) {
    sort(points.begin(), points.end(), [](vector<int> &p1, vector<int> &p2) {
        return p1[1] < p2[1]; // 不需要考虑相等这种情况
    });
    int cnt = 1, r = points[0][1];
    for (int i = 1; i < points.size(); ++i) {
        if (points[i][0] > r) { // 新区间就射一根
            ++cnt;
            r = points[i][1]; // 不能让r随着区间的重叠而扩大
        }
        // 剩余情况：外部包裹住部分当前的线段，不管是全包还是半包，都不用理会
    }
    return cnt;
}


// 435 medium 给定一个区间的集合，找到需要移除区间的最小数量，使剩余区间互不重叠（左右边界重合不算重叠）
// 右边界排序
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.size() == 1) return 0;
    sort(intervals.begin(), intervals.end(), [](const vector<int> &p1, const vector<int> &p2) {
        return p1[1] < p2[1];
    });
    int cnt = 0, r = intervals[0][1];  // pos记录上一个节点的右边界
    for (int i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] < r) ++cnt; // 当前区间与上一个区间重叠，删除当前区间
        else r = intervals[i][1]; // 新区间（等号不算重叠）
    }
    return cnt;
}

// 763 medium 把字符串顺序划分为尽可能多的片段，同一字母最多出现在一个片段中。
// 贪心思想：每次遇到当前区间最远的右边界就结束当前区间
vector<int> partitionLabels(string s) {
    int pos[26];
    for (int i = 0; i < s.size(); ++i) pos[s[i]-'a'] = i; // 初始化最远右边界
    int l = 0, r = 0;
    vector<int> ret;
    for (int i = 0; i < s.size(); ++i) {
        r = max(r, pos[s[i]-'a']);
        if (i == r) { // 当前区间结束
            ret.emplace_back(r-l+1);
            l = i+1;
        }
    }
    return ret;
}

// 986 medium 计算两个已经排好序且不相交的区间列表A和B的交集，左右端点重合算一个点区间
// 合并区间是最傻逼的想法，因为区间列表自己是不相交，合并之后就无法判断来自哪个区间了
// 题干有个性质很重要：取当前位置右边界最小的区间，那么它和另外一个区间列表至多只有一个交集（如果有多个，另外一个区间列表不会不相交）
// 所以用两个指针分别指向两个区间列表的区间，慢慢前进
vector<vector<int>> intervalIntersection(vector<vector<int>>& firstList, vector<vector<int>>& secondList) {
    int p1 = 0, p2 = 0; // 指向区间列表的指针
    vector<vector<int>> ret;
    while (p1 < firstList.size() && p2 < secondList.size()) {
        int r = min(firstList[p1][1], secondList[p2][1]);
        int l = max(firstList[p1][0], secondList[p2][0]);
        if (l <= r) ret.emplace_back(vector<int>{l,r}); // 3种情况：边界重合，部分重合，全部重合
        if (firstList[p1][1] < secondList[p2][1]) ++p1; // r对应的区间前进
        else ++p2;
    }
    return ret;
}


// 630 hard <x,y>表示每个任务的时长和deadline，问最多完成几个任务
// 因为任务可以任意时间开始（完成任务之间不存在间隔），所以遇到完成不了的短任务，可以将其与之前完成的长任务置换
// 因此需要一个数据结构保存之前完成的任务时长
int scheduleCourse(vector<vector<int>>& courses) {
    // 画画图，判定为右边界排序
    sort(courses.begin(), courses.end(), [](const vector<int>& x, const vector<int>& y) {
        return x[1] < y[1];
    });
    priority_queue<int> pq; // 每个任务的时长的大顶堆
    int l = 0; // 每次完成一个任务，l就设置为该任务完成时间，
    for (const auto& course : courses) {
        int r = course[1], t = course[0];
        if (l+t <= r) {
            l += t;
            pq.emplace(t);
        } else if (!pq.empty() && pq.top()>t) { // 当前的任务无法完成，但时间较短
            l -= pq.top()-t; // 将当前任务与之前完成的时长久的任务置换
            pq.pop();
            pq.emplace(t);
        }
    }
    return pq.size();
}