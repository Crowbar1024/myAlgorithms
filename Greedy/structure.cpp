#include "myheader.h"

/*
    顾名思义，贪心中往往需要一些数据结构，比如哈希表，比如set，multiset，优先队列，deque等等
    尤其是deque，有时候能取代multiset和优先队列
*/

// 2071 hard n个任务和m个工人。tasks为每个任务需要的能力值，worker为工人的能力值。
// 每个工人只能完成一个任务，且相应的能力值要大于等于完成任务所需的能力值。
// 有 pills 个药丸，每个工人最多只能使用一片药丸来增加 strength 点能力值
// 问最多有多少个任务可以被完成？
// 贪心思路：用能力从小到大的人匹配能力值要求从小到大的任务，如果不行就吃药。对于现在吃了药的能力，让他完成他能完成的最大的任务。
// 但问题是问最多能完成任务数。假设为x，那么能力从小到大的人的起点应该是第n-x小的人。因为一定选前k小的任务和前k大的人。
// 因此需要遍历k，所以是二分。同时满足二分的性质：如果能完成 x 个任务那么一定能完成 x-1 个任务。
// 贪心思路可以有多种方法：
// 1 双端任务队列
// 因为分吃药和不吃药的人完成任务的区别，前者要完成他能吃药完成的最大的任务，后者要完成恰好小于他能力的任务
// 可以用deque来模拟，队头是吃药的人要完成的任务，队尾是不吃药的人要完成的任务
int maxTaskAssign(vector<int>& tasks, vector<int>& workers, int pills, int strength) {
    int n = tasks.size(), m = workers.size();
    sort(tasks.begin(), tasks.end()); // 完成前x个小的
    sort(workers.begin(), workers.end());
    if (!pills && workers[m-1] < tasks[0]) return 0;
    if (workers[m-1]+strength < tasks[0]) return 0;
    int l = 1, r = min(n, m); // k 左闭右闭
    while (l < r) {
        int mid = (l+r+1)/2; // 根据下面l的演变进行调整
        if (check(tasks, workers, pills, strength, mid)) l = mid;
        else r = mid-1;
    }
    return l;
}
bool check(vector<int>& tasks, vector<int>& workers, int pills, int strength, int x) {
    int n = tasks.size(), m = workers.size();
    deque<int> dq; // 遍历过程中从大到小
    for (int i = m-x, j = 0; i < m; ++i) { // 不能加j的上限，因为可能一次吃药把全部加完了
        // 1 如果非空，说明之前吃了药，并加了一堆之前吃药才能完成的任务；
        // 2 如果空了，说明之前不用吃药，消耗了一堆任务
        if (j < x && dq.empty()) dq.emplace_back(tasks[j++]);
        if (workers[i] >= dq.back()) dq.pop_back(); // 不吃药
        else { // 要吃药
            if (!pills || (workers[i]+strength < dq.back())) return false; // 药没了；药不够嗨
            while (j < x && workers[i]+strength >= tasks[j]) { // 把当前的人吃药能完成的任务全放到队头
                dq.emplace_front(tasks[j++]);
            }
            dq.pop_front(); // 吃药后做掉当前能做掉的最大的任务
            pills -= 1;
        }
    }
    return true;
}
// 2 multiset 性能略差
bool check(vector<int>& tasks, vector<int>& workers, int pills, int strength, int x) {
    int n = tasks.size(), m = workers.size();
    // 要确定遍历顺序，双端队列都是从小到大。但这里需要改变一下
    // 因为如果从小到大遍历人，吃药时找到恰好最大的能完成的任务，这是存在问题的，因为可能找不到，这就要回退一个个遍历
    // 所以从大到小遍历就没问题，遍历的对象是任务，吃药的话，这样找到第一个大于等于减去药力的人，这比较简单
    multiset<int> wks{workers.begin()+m-x, workers.end()}; // 根据上面的讨论，存储人
    for (int i = x-1; i >= 0; --i) {
        if (auto it = prev(wks.end()); tasks[i] <= *it) wks.erase(it); // 该任务不吃药就能满足
        else {
            if (!pills) return false;
            auto pos = wks.lower_bound(tasks[i]-strength);
            if (pos == wks.end()) return false;
            --pills;
            wks.erase(pos);
        }
    }
    return true;
}