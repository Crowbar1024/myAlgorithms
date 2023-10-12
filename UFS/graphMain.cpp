#include "../myheader.h"

/*
    DFS和BFS和并查集都可以处理连通图
    
    当n个互相分立的连通分量需要变为连通图的时候，只需要添加n-1个路线，成为连通图

    如果是无向图，显然可以用UFS判断是否有环。判断方法：
    遍历到某条边时，如果该边的两点已经在一个集合中，那么说明之前两点已经存在一条路径，
    那么加入这条边后，会形成环。
*/

// 323 medium 计算无向图中连通分量的数目
int countComponents(int n, vector<vector<int>>& edges) {
    UFS ufs(n);
    for (vector<int>& edge : edges) {
        int p1 = edge[0], p2 = edge[1];
        ufs.unit(p1, p2);
    }
    return ufs.getCount();
}


// 547 medium 给了一个 n*n 的邻接矩阵，求图中连通图的数量
// 并查集找老大的个数
int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size(), ret = 0;
    UFS ufs(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (isConnected[i][j]) ufs.unit(i, j);
        }
    }
    return ufs.getCount();
}



// 261 medium 无向图判断这些边能够形成一个合法有效的树结构
// UFS检查环和联通分量个数
bool validTree(int n, vector<vector<int>>& edges) {
    UFS ufs(n);
    for (vector<int>& edge : edges) {
        int p1 = edge[0], p2 = edge[1];
        if (ufs.isRelative(p1, p2)) {
            return false;
        }
        ufs.unit(p1, p2);
    }
    return ufs.getCount() == 1 ? true : false;
}


// 990 medium equations（小写字母）形如["a==b","b==c","a==c"]为true，["a==b","b!=c","c==a"]为false
// 就是==就将元素放在一起，遇到！=判断是不是在一个集合里就行了
bool equationsPossible(vector<string>& equations) {
    UFS ufs(26);
    for (const string& str : equations) {
        int x = str[0]-'a', y = str[3]-'a';
        if (str[1] == '=') ufs.unit(x, y);
    }
    for (const string& str : equations) {
        int x = str[0]-'a', y = str[3]-'0';
        if (str[1] == '!' && ufs.find(x) == ufs.find(y)) return false;
    }
    return true;
}

// 684 medium 冗余连接。[1:n]节点组成一棵树，现在使其中两点连接，使之有环。
// 现在需要删除一条边，让它重新变成一棵树。如果有多解，删掉 edges 中靠后的那条。
// 因为只有一条边需要删除，只要从前往后遍历，遇到在同一个连通图的两个点，说明这就是最后需要删除的边
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size(); // 根据题意，n就是节点个数
    UFS ufs(n + 1); // 因为节点下标从1开始，不想做映射，所以并查集大小+1
    for (const vector<int>& edge : edges) {
        if (ufs.isRelative(edge[0], edge[1])) { // 两点在同一集合，说明这条边是圈
            return edge;
        }
        ufs.unit(edge[0], edge[1]);  // 两点加入集合
    }
    return {};
}
// 685 hard 684的有向图版本，依然是只用删除一条靠后的边，让结果是一棵树
// 1->2 2->3 3->1 这是有环无冲突，删除最后一条导致成环的边
// 1->2 3->2 1->3 这是无环有冲突，指向2的节点有两个，删除冲突点（有多个点指向的那个）的第一个父亲
// 1->2 2->3 3->1 4->2 有环有冲突，删除最后一条导致冲突的边
class Solution {
public:
    // 因为这次需要父亲节点来判定边的方向，所以需要在并查集里多一个数组
    vector<int> actr; // 祖先节点 ancestor
    int find(int i) { // 查询并更新祖先
        if (actr[i] == i) return i;
        return actr[i] = find(actr[i]);
    }
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        int n = edges.size(); // 同，边数=节点数
        vector<int> fa(n+1); // 下标从1开始
        actr.resize(n+1);
        for (int i = 1; i <= n; ++i) { // 开始自己是自己的父亲和祖先
            actr[i] = fa[i] = i;
        }
        int conflictEdge = -1, cycleEdge = -1;
        for (int i = 0; i < n; ++i) {
            int x = edges[i][0], y = edges[i][1]; // x -> y
            if (fa[y] != y) { // 说明 y 已经有了父亲，但下面需要y的父亲是x，所以指向y的有2个，出现冲突
                conflictEdge = i; // 取最后的一条冲突边
            } else {
                fa[y] = x;
                int rootx = find(x), rooty = find(y); // 注意此时y还没有更新祖先
                // if (find(x) == find(y)) { // 即 1->2 2->3 ‘3->1’ 的情况
                if (rootx == rooty) { // 即 1->2 2->3 ‘3->1’ 的情况
                    cycleEdge = i; // 3->1 是成环的最后遍历到的边
                } else {
                    actr[y] = actr[x];
                }
            }
        }
        if (conflictEdge < 0) { // 有环无冲突，观察例子1
            return vector<int>{edges[cycleEdge][0], edges[cycleEdge][1]};
        } else {
            if (cycleEdge >= 0) { // 有环有冲突，观察例子3
                return vector<int>{fa[edges[conflictEdge][1]], edges[conflictEdge][1]};
            } else { // 无环有冲突，观察例子2
                return vector<int>{edges[conflictEdge][0], edges[conflictEdge][1]};
            }
        }
    }
};


// 1319 medium 节点个数n，connections[i]=<a,b>表示a和b相连
// 可以拿走两个节点之间的连线，并用它连接一对未连接的节点。返回使所有节点都连通所需的最少操作次数。如果不可能，则返回 -1 。 
// 如果大于等于这个数，说明一定能形成连通图。而形成全连通图就是连通分量-1，所以依然只要计算联通分量个数就行
// DFS：由于connections的形式，需要建立邻接表
// 所以UFS最方便
int makeConnected_UFS(int n, vector<vector<int>>& connections) {
    if (connections.size() < n-1) return -1;
    UFS ufs(n);
    for (int i = 0; i < connections.size(); ++i) {
        ufs.unit(connections[i][0], connections[i][1]);
    }
    int ret = count_if(ufs.cnt.begin(), ufs.cnt.end(), 
            std::bind(less<int>(), std::placeholders::_1, 0)); // bind2nd(less<int>(), 0) 写法在C17被废除
    return ret-1;
}


// 岛屿数量 200 也可以用UFS做，但常数项较高，而且需要UFS需要两个数组，不划算


// 924 hard graph为邻接表，initial为污染的节点，污染会随连接扩散
// 现在可以将一个污染节点清理，但后续还可能会被污染，问清理哪里节点，可以使得最后正常的节点个数最多？
// 如果有多个，返回最小的那个。
// 显然，如果一个联通分量内有多个污染节点，无论清理哪个，最终都会被污染。
// 所以，只有当那些独占一个联通分量的污染节点，清理后才能全部恢复正常。
// 那么就转化成了怎么求数量最多的联通分量的问题
int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
    int n = graph.size(), m = initial.size();
    UFS ufs(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[i][j] == 1) ufs.unit(i, j);
        }
    }
    unordered_map<int,int> cc; // 每个连通分量的感染节点个数
    for (int i = 0; i < m; ++i) {
        int root = ufs.find(initial[i]);
        cc[root] += 1;
    }
    int retRoot = -1, ret = n;
    vector<int> excluNode; // 独占感染点的root
    unordered_set<int> multiNode; // 多个独占的
    for (auto [infectRoot, infectCnt] : cc) {
        if (infectCnt == 1) {
            excluNode.emplace_back(infectRoot);
        }
    }
    if (excluNode.empty()) { // 没有独占的，说明怎么删都没用，返回最小的
        auto it = min_element(initial.begin(), initial.end());
        return *it;
    } else if (excluNode.size() == 1) { // 只有一个独占，返回这个
        for (int node : initial) {
            if (ufs.find(node) == excluNode[0]) {
                ret = node;
                break;
            }
        }
    } else { // 多个独占，返回其中联通分量最大的
        int maxCnt = 0;
        for (int root : excluNode) {
            int rootCnt = ufs.cnt[root];
            if (rootCnt < maxCnt) { // 都是负数
                maxCnt = rootCnt;
            }
        }
        for (int root : excluNode) { // 得到多个独占的节点的头节点
            int rootCnt = ufs.cnt[root];
            if (rootCnt == maxCnt) {
                multiNode.emplace(root);
            }
        }
        for (int node : initial) {
            int root = ufs.find(node);
            if (multiNode.count(root) > 0) {
                ret = min(ret, node); // 返回其中最小的
            }
        }
    }
    return ret;
}
// 看了官方的代码，发现简洁很多，有几个可以优化的地方：
// 1 因为长度不长，哈希表用数组代替
// 2 原版是先找root，再根据root找node，没有必要，直接找node就行
int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
    int n = graph.size();
    UFS ufs(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[i][j] == 1) ufs.unit(i, j);
        }
    }
    vector<int> cc(n, 0); // 每个连通分量的感染节点个数
    for (int cur : initial) {
        int root = ufs.find(cur);
        cc[root] += 1;
    }
    int maxCnt = -1, ret = n;
    for (int cur : initial) {
        int curRoot = ufs.find(cur);
        if (cc[curRoot] > 1) continue; // 一个联通分量的污染节点不是1个，无意义
        int curCnt = ufs.cnt[curRoot];
        if (curCnt < maxCnt) {
            maxCnt = curCnt;
            ret = cur;
        } else if (curCnt == maxCnt && cur < ret) {
            ret = cur;
        }
    }
    if (ret == n) { // 这种情况就是所有的连通分量里面的污染节点都超过1
        ret = *min_element(initial.begin(), initial.end());
    }
    return ret;
}
// 当然这道题也有DFS的做法，就是给联通分量染色