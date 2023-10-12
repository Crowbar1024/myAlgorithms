#include "myheader.h"

// 差分数组就是原数组a中相邻元素的差，d[i]=a[i]-a[i-1]
// 根据a[0]和差分数组d，即可推出a中的所有元素
// 其最直观的应用就是当你将原始数组中[l,r]区间的元素同时加上或者减掉某个数，
// 差分数组d对应的区间左端点d[l]的值会同步变化，而他的右端点的后一个值d[r+1]则会相反地变化，除此之外不会变化。
// 可见关键之处在于是相同的树，如果是不同的，线段树就派上了用场。

/*
    难点：边界条件
    一般都是x加入，y时离开：df[x]+=和df[y]-=
    如果是[l,r]都加，就是df[y+1]-=
    df一般下标从0开始，因为0时加入符合题意。也要注意[l,r]的取值范围，是否为0开始。
*/

/*
    优化点：
    一般可以用hashmap去优化
    一个个点显然可以优化为
*/


// 1109 medium booksing[i] = {l,r,add}，[l,r]都加上一个数add。返回更新完后的数组
// 1 <= l <= r <= n
vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> df(n+1, 0); // 看+-的逻辑
    for (int i = 0; i < bookings.size(); ++i) {
        int l = bookings[i][0] - 1, r = bookings[i][1] - 1, addNum = bookings[i][2];
        df[l] += addNum; // 相比l-1多了
        df[r+1] -= addNum; // 相比r少了
    }
    vector<int> ret(n, 0);
    ret[0] = df[0];
    for (int i = 1; i < n; ++i) {
        ret[i] += ret[i-1] + df[i];
    }
    return ret;
}


// 1094 meidum trips[i]={n,l,r} n个客人在l时上车，r时下车。车负载capacity，问是否会超载
// 下车时人走了，所以不是r+1做改变
bool carPooling(vector<vector<int>>& trips, int capacity) {
    int n = trips.size();
    int len = 0;  // 最晚下车时间。作为差分数组的长度
    for (int i=0; i<n; ++i) {
        len = max(len, trips[i][2]);
    }
    vector<int> df(len+1, 0); // 0 <= l < r <= 1000 l可以取到0
    for (int i=0; i<n; ++i) {
        int l = trips[i][1], r = trips[i][2], addN = trips[i][0];
        df[l] += addN; // 相比l-1时多了人
        df[r] -= addN; // 相比r-1时少了人
    }
    int num = 0;
    for (int x : df) {
        num += x;
        if (num > capacity) return false;
    }
    return true;
}


// 253 medium 每个会议时间都会包括开始和结束的时间 intervals[i] = [starti, endi] ，返回所需会议室的最小数量
// 区间重合的不能用一个会议室，所以这是求最大重复区间的区间数
// -----
//   ------
//       ----
// 这种就需要两个，和射箭很像452，但求的是一箭最多能射爆多少区间
// 用差分数组，统计每个时间点的最大数量。
int minMeetingRooms(vector<vector<int>>& intervals) {
    int len = 0; // 最晚会议使用时间
    for (vector<int> &interval : intervals) {
        len = max(len, interval[1]); // 空间复杂度较高
    }
    vector<int> df(len + 1, 0); // 查看+-逻辑
    for (vector<int> &interval : intervals) {
        df[interval[0]] += 1;
        df[interval[1]] -= 1; // 结束会议时间r，此时会议结束了，所以相当于r-1下车
    }
    vector<int> curCnt(len, 0); // 每个时间点的人数
    curCnt[0] = df[0];
    for (int i = 1; i < len; ++i) {
        curCnt[i] = curCnt[i-1] + df[i];
    }
    int ret = *max_element(curCnt.begin(), curCnt.end());
    return ret;
}
// 可以用map优化下
int minMeetingRooms(vector<vector<int>>& intervals) {
    vector<pair<int,int>> diff;
    for (auto interval : intervals) {
        diff.emplace_back(interval[0], 1);
        diff.emplace_back(interval[1], -1); // 结束的时间点，人已经走了
    }
    sort(diff.begin(), diff.end()); // 默认是排序pair的第一个，按时间排序
    int cnt = 0, ret = 0;
    for (auto ele : diff) {
        cnt += ele.second;
        ret = max(ret, cnt);
    }
    return ret;
}


// 21年春招实习某大厂笔试
// 一串01序列，给了一组[l,r]，这个范围内的01翻转，返回最后的01序列
// 可以发现，若是 0 0 1 1 0 1
// 记录发生变化  0 0 1 0 1 1   （注意默认01位置为0）
// 若是[1,3]翻转 0 1 0 0 0 1  （下标从0开始）
// 变化数组      0 1 1 0 0 1
// 变化的数组[l]^=1 [r+1]^=1
vector<int> reverseRange(vector<int> a, vector<vector<int>> ranges) {
    int n = a.size();
    vector<int> df(n+1, 0);  // 更新l从0开始，只需要+1
    df[0] = a[0];  // 这里默认哨兵节点a[-1]=0
    for (int i=1; i<n; ++i) {
        df[i] = a[i-1]^a[i];
    }
    for (int i=0; i<ranges.size(); ++i) {
        int l = ranges[i][0], r = ranges[i][1];
        df[l] ^= 1;
        df[r+1] ^= 1;
    }
    vector<int> ret(n, 0);
    ret[0] = df[0];
    for (int i=1; i<n; ++i) {
        ret[i] = df[i]^ret[i-1];
    }
    return ret;
}



// 1854 easy logs[i] = [birthi, deathi] 表示人的出生和死亡年份。返回人口最多且最早的年份（人不应当计入他们死亡当年的人口中）
// 1 <= logs.length <= 100
// 1950 <= birthi < deathi <= 2050
// 一眼差分，用贪心是做不出来的
int maximumPopulation(vector<vector<int>>& logs) {
    vector<int> df(101, 0);  // df[0]:1950-1949(0), df[100]:2050-2049
    for (int i = 0; i < logs.size(); ++i) {
        int l = logs[i][0]-1950, r = logs[i][1]-1950;
        df[l] += 1;
        df[r] -= 1;
    }
    int ret = 0, snap = 0, pos = 0;
    for (int i = 0; i < df.size(); ++i) {
        snap += df[i];
        if (snap > ret) {
            ret = snap;
            pos = i;
        }
    }
    return pos+1950;
}