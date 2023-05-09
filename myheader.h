#ifndef MYHEADERH
#define MYHEADERH


#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <bitset>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <string>
#include <functional>
#include <atomic>
#include <sstream>
#include <numeric>
#include <climits>
#include <ctime>
#include <type_traits>

using namespace std;


const int INF = 0x3f3f3f3f;
typedef long long ll;
const int maxn=5e2+5;
const int MOD=1e9+7;



struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
ListNode* generateList(vector<int>& a) {
    ListNode dummy(INT_MIN), *tail = &dummy;
    for (const int& n : a) {
        tail->next = new ListNode(n);
        tail = tail->next;
    }
    return dummy.next;
}
void printList(ListNode* head) {
    ListNode *cur = head;
    while (cur) {
        if (cur->next) cout << cur->val << " ";
        else cout << cur->val;
        cur = cur->next;
    }
    cout << endl;
}

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


void printArray(vector<int>& a) {
    for (int i = 0; i < a.size(); ++i) {
        if (i != a.size()-1) {
            cout << a[i] << " ";
        } else {
            cout << a[i] << endl;
        }
    }
}
void printMatrix(vector<vector<int>>& a) {
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < a[i].size(); ++j) {
            if (j != a[i].size()-1) {
                cout << a[i][j] << " ";
            } else {
                cout << a[i][j] << endl;
            }
        }
    }
}

class UFS {
public:
    vector<int> cnt; // cnt[i]<0说明i是联通分量的头头，其绝对值为集合元素的数量
    UFS(int n = 0) {
        cnt.resize(n, -1); // 一开始所有自己的头头就是自己，-1表示自己的联通分量就自己一个
    }
    int find(int i) { // 查询i的集合的头头
        if (cnt[i] < 0) return i; // 是自己
        else return cnt[i] = find(cnt[i]);
    }
    void unit(int x, int y) {
        int root1 = find(x);
        int root2 = find(y);
        if (root1 != root2) {
            cnt[root1] += cnt[root2];  // 集合的数量增加
            cnt[root2] = root1;  // 后者挂在前者上
        }
    }
    void unitSeq(int nx, int ny) { // 让一个集合中的最小值成为头
        int root1 = find(nx);
        int root2 = find(ny);
        if (root1 != root2) {
            if (root1 > root2) swap(root1, root2);
            cnt[root1] += cnt[root2];
            cnt[root2] = root1;
        }
    }
    int getCount() {
        int ret = 0;
        for (const int elem : cnt) {
            if (elem < 0) {
                ret += 1;
            }
        }
        return ret;
    }
    bool isRelative(int x, int y) {
        return find(x) == find(y);
    }
};


const vector<int> dir{0,-1,0,1,0};  // 左上右下 逆时针

#endif