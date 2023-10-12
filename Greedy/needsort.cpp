#include "myheader.h"


/*
    多维度思考
    1 数据有2个维度，需要自定义排序：452，435，406，
    2 数据需要左右遍历2次：135
    3 数据左遍历，先进行统计信息，在左遍历一次做策略：763

*/



// 621 medium 大写英文字母数组代表任务列表，每个任务执行时间为单位时间1。
// CPU一个单位时间可以完成一个任务，或者处于待命状态。
// 相同种类的任务之间有冷却时间n，其间CPU可以执行其他任务或者在待命。计算完成所有任务所需要的最短时间。
// 比如n = 2, A-B-1-A-B-1-A-B，一共需要8个时间单位。
// 建立二元组<nextTime, leftNum>，表示每种任务的下一次可执行时间和该种的剩余任务数。
// 选择下一次可执行时间小于等于当前时间并且剩余执行次数最多的那个任务。
// 不用模拟每个时间片，我们可以在每一次遍历之前，将时间更新为二元组中nextTime的最小值。
int leastInterval(vector<char>& tasks, int n) {
    unordered_map<char, int> taskNum; // 统计每种任务的数量
    for (char ch : tasks) taskNum[ch] += 1;
    int m = taskNum.size(), num = tasks.size(); // 任务种数和任务总数
    vector<int> nextTime(m, 1); // 每种任务的下次执行时间，无所谓顺序，反正和下面一一对应，初始化为1，都可以执行
    vector<int> leftNum; // 每种任务剩余的次数，无所谓顺序
    for (auto it : taskNum) leftNum.emplace_back(it.second);
    int cur = 0; // 初始时间片，下次任务一定能在单位时间1执行。
    while (num--) {
        ++cur; // 更新时间片
        int minNextValid = INT_MAX; // 贪心，让下一次的任务开始的时间越早越好
        for (int i = 0; i < m; ++i) {
            if (leftNum[i]) minNextValid = min(minNextValid, nextTime[i]);
        }
        cur = max(cur, minNextValid); // 跳到下一次可以执行的时间，避免cur每次+1进度太慢
        // cur > minNextValid的情况，比如有多种任务，已经前进了好几步，但还是有种任务没有执行过
        int task = -1; // 当前cur选择执行的任务
        for (int i = 0; i < m; ++i) { // 选择下一次可执行时间小于等于当前时间并且剩余执行次数最多的那个任务。
            if (leftNum[i] && nextTime[i] <= cur) {
                if (task == -1 || leftNum[i] > leftNum[task]) task = i;
            }
        }
        nextTime[task] = cur + n + 1; // 更新该任务的下次执行时间
        leftNum[task] -= 1;
    }
    return cur;
}



// 406 medium 一堆人排队，他们只能看到前面比自己高或者和自己一样高的人数ki，同时也知道自己身高hi
// 即已知知识为 {hi, ki}。现在给出了一组 pair，但顺序不是队伍从前往后给出的，返回正确的队伍顺序。
// 如果按照身高从高到低排，ki从低到高排序。每次插进一个人，因为所有人都比自己高或者相同，所以只要插到ki就完了
vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
    sort(people.begin(), people.end(), [](const vector<int> &p1, const vector<int> &p2) {
        if (p1[0] == p2[0]) return p1[1] < p2[1];
        return p1[0] > p2[0];
    });
    list<vector<int>> que;
    for (const vector<int> &p : people) {
        int pos = p[1];
        auto it = que.begin();
        while (pos--) ++it;
        que.insert(it, p);
    }
    return vector<vector<int>>{que.begin(), que.end()};
}


// 135 hard 老师想给孩子们分发糖果，有 N 个孩子站成了一条直线，老师会根据每个孩子的ratings，预先给他们评分。
// 你需要按照以下要求，帮助老师给这些孩子分发糖果：
// 1 每个孩子至少分配到 1 个糖果；2 相邻的孩子中，评分高的孩子必须获得更多的糖果。
// 那么这样下来，老师至少需要准备多少颗糖果呢？
// 不要想太多，向前一次，向后一次
int candy(vector<int>& ratings) {
    int len = ratings.size();
    vector<int> cnt(len, 1);
    for (int i = 1; i < len; ++i) {
        if (ratings[i] > ratings[i-1]) cnt[i] = cnt[i-1] + 1;
    }
    for (int i = len-2; i >= 0; --i) {
        if (ratings[i] > ratings[i+1]) cnt[i] = max(cnt[i], cnt[i+1]+1);
    }
    return accumulate(cnt.begin(), cnt.end(), 0);
}


// 763 medium s由小写字母组成。把s划分为尽可能多的段，同一字母最多出现在一个段中。返回一个表示每个字符串片段的长度的列表。
// 首先用一个数组pos统计每个字母最远出现的位置，这意味着：i<=pos[i]
// 所以"858575878,14,...“这种情况，表示[l,r]区间的边界就是pos[8]
// 如果在找8的过程中出现比8更大的数，比如9，且还没到8时，这意味着需要更新要找的数为pos[9]
// 这是因为需要把9放到该区间，所以得找到i==pos
// 如果比8小，不能分，因为8还没找到。
vector<int> partitionLabels(string s) {
    int pos[26] = {0};
    for (int i = 0; i < s.size(); ++i) {
        pos[s[i]-'a'] = i;
    }
    vector<int> ret;
    int l = 0, r = 0;
    for (int i = 0; i < s.size(); ++i) {
        r = max(r, pos[s[i]-'a']);
        if (i == r) {
            ret.emplace_back(r-l+1);
            l = i + 1;
        }
    }
    return ret;
}

// 455 easy g是任务等级，s是机器等级。一个任务只能被等级大于等于它的机器完成。最多完成几个任务
// 排序，从容易完成的任务开始。参与遍历的每个任务都能完成。
int findContentChildren(vector<int>& g, vector<int>& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());
    int i = 0; // 任务
    for (int j = 0; j < s.size(); ++j) { // 其实只要找机器就行
        if (i < g.size() && s[j] >= g[i]) ++i;
    }
    return i;
}

// HDU 4864 有n个机器，m个任务。每个机器有运行时间和等级，每个任务也有运行时间和等级。每个机器至多能完成一个任务。
// 一个任务能完成的前提是其时间时间和等级不大于执行该任务的机器。
// 一个任务完成的奖励=500t+2lv。问最多能完成几个任务。当出现多种情况时，输出获得金钱最多的情况。
// 因为500>>2，并且奖励只与任务有关，所以对任务进行排序：时间高的在前面，然后时间相同的情况下，等级高在前面。
// 把所有能匹配当前任务时间的机器用一个数据结构记录，然后从中找出匹配等级最低的。
struct Node {
    int t; // 时间
    int lv; // 等级
    Node(int _t, int _lv) : t(_t), lv(_lv) {}
    bool operator<(const Node& x) { return t==x.t ? lv>x.lv : t>x.t; }
};
pair<int,ll> machineTask(vector<int> machineRuntime, vector<int> machineLevel, vector<int> taskRuntime, vector<int> taskLevel) {
    int n = machineRuntime.size(), m = taskRuntime.size();
    vector<Node> machine(n), task(m);
    for (int i = 0; i < n; i++) {
        machine[i].t = machineRuntime[i];
        machine[i].lv = machineLevel[i];
    }
    for (int i = 0; i < m; i++) {
        task[i].t = taskRuntime[i];
        task[i].lv = taskLevel[i];
    }
    sort(machine.begin(), machine.end());
    sort(task.begin(), task.end());
    vector<int> cnt(105, 0);  // level<=100，统计每个level能完成任务的机器数量
    int taskNum = 0;  // 完成的任务数
    ll maxProfit = 0;  // 最大回报
    for (int i = 0, j = 0; i < m; i++) { // 外部遍历任务，内部找到恰好能完成该任务的机器
        while (j < n && task[i].t <= machine[j].t) {  // 统计仅能匹配当前任务时间的所有机器
            cnt[machine[j].lv]++; // 因为时间从高到底降序，所以剩下的机器一定能匹配上当前的任务
            j++;
        }
        for (int k = task[i].lv; k <= 100; k++) { // 从上面的机器中寻找匹配等级最小的那个，所以从任务等级开始遍历
            if (cnt[k]) {
                taskNum++;
                maxProfit += 500*task[i].t + 2*task[i].lv;
                cnt[k]--;
                break;
            }
        }
    }
    return pair<int,ll>{taskNum, maxProfit};
}
