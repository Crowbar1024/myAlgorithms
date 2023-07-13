#include "myheader.h"

// 295 hard 两种操作，一个往数组中添加元素，一个求数组的中位数。5e4次数的插入和查询
// 两个堆是最简单直接的解法，[l,mid,r]，分成大顶堆和小顶堆，动态控制大小，默认左边部分至多可以多一个
// 然后树状数组也可以
class MedianFinder {
private:
    priority_queue<int, vector<int>> pql; // 大顶堆，存储l部分
    priority_queue<int, vector<int>, greater<int>> pqr;
public:
    MedianFinder() {}
    void addNum(int num) {
        if (pql.empty() || pql.top() >= num) {
            pql.emplace(num);
            if (pql.size()-1 > pqr.size()) { // 默认左边部分至多可以多一个
                pqr.emplace(pql.top());
                pql.pop();
            }
        } else {
            pqr.emplace(num);
            if (pqr.size() > pql.size()) { // 默认左边部分至多可以多一个
                pql.emplace(pqr.top());
                pqr.pop();
            }
        }
    }
    double findMedian() {
        if (pql.size() < pqr.size()) return pql.top(); // 默认左边部分至多可以多一个
        return (pql.top()+pqr.top())*0.5;
    }
};