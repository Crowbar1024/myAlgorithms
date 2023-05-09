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


// 1109 medium booksing的内容为{l,r,add}，[l,r]都加上一个数add。返回更新完后的数组
// 1 <= l <= r <= n
vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> df(n+2, 0);  // l从1开始，最后一位更新是n+1（左右都闭），所以一共n+2个
    for (int i=0; i<bookings.size(); ++i) {
        int l = bookings[i][0], r = bookings[i][1], addNum = bookings[i][2];
        df[l] += addNum;
        df[r+1] -= addNum;
    }
    vector<int> res(n, 0);
    res[0] = df[1];
    for (int i=1; i<n; ++i) {
        res[i] = res[i-1] + df[i+1];
    }
    return res;
}


// 1094 meidum 乘客上下车问题，trips{n,l,r}，n个客人l时上车，r时下车，车负载capacity，问是否会超载
// 关键点：需要找到最远的距离，作为差分数组的长度
// 下车时人走了，所以不是r+1做改变
// 0 <= l < r <= 1000
bool carPooling(vector<vector<int>>& trips, int capacity) {
    int n = trips.size();
    int len = 0;  // 最远距离
    for (int i=0; i<n; ++i) {
        len = max(len, trips[i][2]);
    }
    vector<int> df(len+1, 0);  // 0也算
    for (int i=0; i<n; ++i) {
        int l = trips[i][1], r = trips[i][2], addN = trips[i][0];
        df[l] += addN;
        df[r] -= addN;
    }
    int num = 0;
    for (const int& x : df) {
        num += x;
        if (num > capacity) return false;
    }
    return true;
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
    vector<int> res(n, 0);
    res[0] = df[0];
    for (int i=1; i<n; ++i) {
        res[i] = df[i]^res[i-1];
    }
    return res;
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
    int res = 0, snap = 0, pos = 0;
    for (int i = 0; i < df.size(); ++i) {
        snap += df[i];
        if (snap > res) {
            res = snap;
            pos = i;
        }
    }
    return pos+1950;
}