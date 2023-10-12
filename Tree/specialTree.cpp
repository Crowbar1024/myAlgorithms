#include "myheader.h"
 


// 一个纸条，从下往上折（默认超前的方向），持续n次，输出从上到下每条折痕的凹凸性
// 默认凹凸01，1-0，2-001，3-0010011
// 可以发现是一个完全二叉树的中序遍历，树的顶点为0，左右儿子恒定为01
// 递归版
void paperFolding(int n) {
    paperFoldingProcess(1,n,true);
}
void paperFoldingProcess(int i, int n, bool tag) {
    if (i > n) return;
    paperFoldingProcess(i+1, n, true);
    if (tag) cout << 0;
    else cout << 1;
    paperFoldingProcess(i+1, n, false);
}

// PAT 
// 一棵树，每个节点可以有多个子节点，计算最大深度和最后一层的叶子节点数量，假设头节点深度为0
// nums数组是每个节点的父亲节点编号，父亲为-1的说明是root
struct node {
    int pre;
    int depth;
    bool operator<(const node& b) {
        return depth < b.depth;
    }
};
vector<int> leafCount(vector<int>& nums, int& maxDepth) {
    int len = nums.size();
	vector<node> v(len);
    for (int i = 0; i != len; ++i) {  // 初始化
        v[i].pre = nums[i];
        v[i].depth = 0;
    }
    for (int i = 0; i != len; ++i) {  // 用来遍历这棵树，以获得每个节点的深度等信息
        int tmpDepth = 0, tmpPre = i;
        while ((tmpPre = v[tmpPre].pre) != -1) {  // 自下而上溯源到-1的节点，统计次数便是节点i的深度
            tmpDepth++;
            if (v[tmpPre].depth > 0) {  // 父亲节点已经被遍历过，利用备忘录
                tmpDepth += v[tmpPre].depth;
                break;
            }
        }
        for (int j = i; v[j].depth == 0 && j != -1; j = v[j].pre)  // 更新节点i往上的所有父亲节点
            v[j].depth = tmpDepth--;
    }
    int cnt = 0;
    int maxDepth = (*max_element(v.begin(), v.end())).depth;
    for (int i = 0; i != len; ++i)
        if (maxDepth == v[i].depth) cnt++;
    return vector<int>{maxDepth, cnt};
}


// PAT
// n是节点总数，k是非叶子节点个数，tree是节点向量
// 输出所有满足路径之和为sum的路径，输出序列的顺序是降序，这就对dfs的顺序有了要求
struct node {
	int w;
	vector<int> child;
};
void printAllPaths(int n, int sum, vector<node>& tree) {
	auto cmp = [tree] (int x, int y) { return tree[x].w > tree[y].w; };  // tree本身就是引用
	for (int i = 0; i < n; i++) {
        if (tree[i].child.size() > 0) {
            sort(tree[i].child.begin(), tree[i].child.end(), cmp);
        }
	}
	vector<int> path;
	path.push_back(0);
	dfs(tree, 0, path, tree[0].w, sum);
}
// cur是当前遍历到的节点，path是头节点到cur的节点路径
// curPathSum是当前path的sum
void dfs(vector<node>& tree, int cur, vector<int>& path, int curPathSum, int sum) {
	if (curPathSum > sum) return;
	int childNum = tree[cur].child.size();  // 当前节点的子节点数量
	int pathLen = path.size();  // 头节点到当前节点的路径长度
	if (curPathSum == sum && childNum == 0) {  // 满足输出条件
		for (int i = 0; i < pathLen; i++) {
			printf("%d%c", tree[path[i]].w, (i<pathLen-1)?' ':'\n');
		}
	} else if (curPathSum < sum && childNum > 0) {  // 选择当前节点的子树
		for (int i = 0; i < childNum; i++) {
			int childNo = tree[cur].child[i];
			path.push_back(childNo);
			dfs(tree, childNo, path, curPathSum+tree[childNo].w, sum);
			path.pop_back();
		}
	}
}

// 21/4/25 字节笔试2 easy
// [0,n-1]个节点，给了[1,n]的父节点，0是根节点
// q次查询，每次查询删去一个节点，同时它的所有子节点都被删去
// 求每次查询后剩余的节点数
// 这道题不需要建自定义数组，直接二维数组搞定
// isVisited是备忘录
void dfs(vector<int>& isVisited, vector<vector<int>>& tree, int cur, int& ret) {
    if (isVisited[cur] == 1) return;
    isVisited[cur] = 1;
    --ret;
    for (int i = 0; i < tree[cur].size(); ++i) {
        dfs(isVisited, tree, tree[cur][i], ret);
    }
}

// PAT 1004 Counting Leaves
// n个节点，m个非叶子节点，然后给出m个非叶子节点的孩子节点序号
// 问每一层有几个叶子节点。
// 这道题如果用dfs来做的话，需要注意并不知道树的高度，所以需要统计叶子的数量，每层输出后计算是否超过了这个值
void dfs(vector<vector<int>>& tree, int cur, int level, vector<int>& leavesNumEachRow) {
    if (tree[cur].empty()) {
        ++leavesNumEachRow[level];
        return;
    }
    for (int i = 0; i < tree[cur].size(); ++i) {
        dfs(tree, tree[cur][i], level+1, leavesNumEachRow);
    }
}

// 690 easy 返回id的子树的节点值之和
// 使用hashmap
class Employee {
public:
    int id;
    int importance;
    vector<int> subordinates;
};
int getImportance(vector<Employee*> employees, int id) {
    int ret = 0;
    unordered_map<int, Employee*> m;
    for (int i = 0; i < employees.size(); ++i) {
        m[employees[i]->id] = employees[i];
    }
    dfs(m, id, ret);
    return ret;
}
void dfs(unordered_map<int, Employee*>& m, int id, int& ret) {
    ret += m[id]->importance;
    if (m[id]->subordinates.empty()) return;
    for (int i = 0; i < m[id]->subordinates.size(); ++i) {
        dfs(m, m[m[id]->subordinates[i]]->id, ret);
    }
}