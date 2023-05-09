#include "myheader.h"

/*
    拓扑排序：
    拓扑排序可以用于有向无环图（DAG）和无向无环图
    举个例子，一个工程编译，需要编译ABCDE...等文件。每个文件可能会依赖于其他文件，所以编译顺序需要先编译没有依赖的文件。
    一般要求其输出顺序，即为对应的拓扑排序顺序。
    1. 先遍历出所有入度为0的节点，然后删除。
    2. 继续遍历图中剩余的入度为0的节点。
    3. 依次往复。
    
    数据结构：
    1 邻接表
    2 数组用来记录节点的入度

    用途：
    1. DAG经过拓扑排序后，剩余节点为0，常用于判断一个有向图是否有环。
    2. 无向无环图可以经过拓扑排序，得到1个或2个最小高度树的节点，遍历轮数就是最小高度

    本质：
    BFS

*/



// 207 medium 有许多门课程，<x,y> x的前置课程是y，问是否可以修完全部课程
// 所以是一个有向图是否有环的问题
// 可以转换为拓扑排序后剩余的点的数量是否为0
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> in(numCourses, 0);  // 每个节点的入度
    vector<vector<int>> adjL(numCourses);  // 邻接表
    for (int i = 0; i < prerequisites.size(); ++i) {
        adjL[prerequisites[i][1]].emplace_back(prerequisites[i][0]); // y->x
        in[prerequisites[i][0]] += 1;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        if (!in[i]) q.emplace(i); // 入度为0的节点入列
    }
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        numCourses -= 1; // 删除入度为0的节点
        for (int next : adjL[cur]) { // 删除节点时将其的相邻节点入列
            if (--in[next] == 0) q.emplace(next); // 相邻节点对应的入度减一。如果为0，说明其为下一轮入列的节点
        }
    }
    return numCourses == 0;
}
// 210 medium 返回你为了学完所有课程的任意学习顺序。如果不可能完成所有课程，返回一个空数组
// 只需要在207的基础上，设置一个数组，每次记录出栈元素就行。



// vivo 21秋A i的编译依赖input[i]，-1表示头节点
// 比如 5,0,4,4,5,-1
// 5->0 0->1 4->2 4->3 5->4 5(head)
// 5->0->1
//  ->4->2
//     ->3
// 需要升序输出编译顺序 5,0,4,1,2,3
// 按照之前的普通队列，其实就是BFS的蔓延，不符合升序，所以使用一个优先队列，即小顶堆
string compileSeq(string input) {
    vector<int> nums;
    stringstream ss(input); //读取str到字符串流中
    string temp;
    while(getline(ss, temp, ',')) {
        nums.emplace_back(stoi(temp));
    }
    int n = nums.size();
    vector<int> in(n, 0);
    vector<vector<int>> adjL(n);
    for (int i = 0; i < n; ++i) {
        if (nums[i] < 0) continue;
        adjL[nums[i]].emplace_back(i);
        in[i] += 1;
    }
    priority_queue<int, vector<int>, greater<int>> q; // 小顶堆
    for (int i = 0; i < n; ++i) {
        if (!in[i]) q.emplace(i);
    }
    string res;
    while (!q.empty()) {
        int cur = q.top(); q.pop();
        n -= 1;
        res += to_string(cur);
        res += ",";
        for (int next : adjL[cur]) {
            if (--in[next] == 0) q.emplace(next);
        }
    }
    res.pop_back();
    return res;
}



// 310 medium 一个全连通的无向图，n个节点[0:n-1]，n-1条边。<x,y>表示x与y相连。
// 可选择图中任何一个节点作为一颗树的根。请你找到所有的最小高度树并按任意顺序返回它们的根节点标签列表。
// 假设现在画出了最小高度数，每次把度为1的节点删除，最后要么剩一条边，要么剩一个点。
// 显然，如果为边，对应的最小高度是删除轮数+1；如果为点，最小高度是删除轮数
vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
    if (n == 1) return vector<int>{0};
    vector<int> deg(n, 0); // 每个节点的度
    vector<vector<int>> adjL(n);
    for (int i = 0; i < n-1; ++i) {
        adjL[edges[i][0]].emplace_back(edges[i][1]);
        adjL[edges[i][1]].emplace_back(edges[i][0]);
        deg[edges[i][0]] += 1;
        deg[edges[i][1]] += 1;
    }
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (deg[i] == 1) q.emplace(i);
    }
    while (n > 2) {
        int times = q.size();
        n -= times;
        while (times--) { // 因为q.size()会变，所以要用前面一次的删除节点数作为终止条件，类似层序遍历
            int cur = q.front(); q.pop();
            for (int next : adjL[cur]) {
                if (--deg[next] == 1) q.emplace(next);
            }
        }
    }
    vector<int> res;
    while (!q.empty()) {
        res.emplace_back(q.front()); q.pop();
    }
    return res;
}
// PAT 1021 做了一定修改。最大高度树，返回对应的高度和根结点。如果有多个，按照节点序号从小到大输出。
// 可证：如果s-t为无向图的最长路径，那么从任意节点深搜得到的最深的节点一定是s或者t。但另一个往往得不到，除非dfs起点为“中点”
// 所以只需要两次DFS即可，以任一节点为起点，深搜，找到最深的节点。然后以该节点为起点深搜，最深的路径长度即为所求。
// 这两个结点集合的并集就是所求。
vector<int> findMaxHeightTrees(int n, vector<vector<int>>& edges) {
    if (n == 1) return vector<int>{0};
    vector<int> deg(n, 0); // 每个节点的度
    vector<vector<int>> adjL(n);
    for (int i = 0; i < n-1; ++i) {
        adjL[edges[i][0]].emplace_back(edges[i][1]);
        adjL[edges[i][1]].emplace_back(edges[i][0]);
    }
    vector<bool> isVisit(n, false);
    
    int maxlen = 0;
    vector<int> res; // 存储符合题意的节点
    dfs(isVisit, adjL, res, 0, 1, maxlen);
    int p = res[0]; // 其实任意都行
    fill(isVisit.begin(), isVisit.end(), false); // 重新开始dfs
    // maxlen和nodes不用更新，前者会更大，后者本来就保存了一部分端点
    dfs(isVisit, adjL, res, p, 1, maxlen);
    return res;
}
void dfs(vector<bool>& isVisit, vector<vector<int>>& adjL, vector<int>& res, int i, int len, int maxlen) {
    if (len > maxlen) { // 更新最长路径的长度和端点
        maxlen = len;
        res.clear();
        res.emplace_back(i);
    } else if (len == maxlen) {
        res.emplace_back(i);
    }
    isVisit[i] = true;
    for (int next : adjL[i]) {
        if (!isVisit[next]) dfs(isVisit, adjL, res, next, len+1, maxlen);
    }
}


