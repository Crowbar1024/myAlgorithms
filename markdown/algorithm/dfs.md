### 终止的snap和res
我想把snap当成是最终结果，这是不行的，因为dfs()往往是void的。
（但连通图问题有时候需要有返回值，其实就是把snap定义在dfs开始，每次+=dfs，最后返回snap）
会不断推缩。即一定会走完。
所以需要一个额外的res数组。


### dfs递归改写成栈循环
大部分场景没必要。这样做的主要是想到终点直接终结。
但这样used如何变成0是一个问题。一般要一个路径去记录，得不偿失。
这样不如干脆去考虑BFS的可行性。

部分场景可行，比如连通图，因为used可以用消元法去处理掉，并且maze不会变来变去，只变一次。

### pop的意义
易知for循环是同层的兄弟节点的顺序，弹出的意义就是刷新哥哥的顺序。
如果只有同层一个节点，还是需要加入这个函数，为了避免后续的数量问题。

### 一次研究
LC22 medium，关于dfs回溯的必要性的探讨
1. 这种写法无疑是很标准的写法
```cpp
vector<string> generateParenthesis(int n) {
    vector<string> res;
    string snap = "";
    dfs(n, n, res, snap);
    return res;
}
void dfs(int lnum, int rnum, vector<string>& res, string& snap) {
    if (lnum == 0 && rnum == 0) { // 左右括号都用完了，snap结束
        res.push_back(snap);
        return;
    }
    if (lnum == rnum) { // 左右括号数相等，只能先放左括号，所以剩余的左括号数量永远小于等于右括号
        snap += "(";
        dfs(lnum-1, rnum, res, snap);
        snap.pop_back();
    // 若小于，有两种情况，比如((，下一个可以是左括号也可以是右括号
    } else {
        if (lnum > 0) {
            snap += "(";
            dfs(lnum-1, rnum, res, snap);
            snap.pop_back();
        }
        snap += ")";
        dfs(lnum, rnum-1, res, snap);
        snap.pop_back();
    }
}
```

2. 也是比较标准的写法
需要注意的是，需要声明为const string
```cpp
vector<string> generateParenthesis(int n) {
    vector<string> res;
    dfs(n, n, res, "");
    return res;
}
void dfs(int lnum, int rnum, vector<string>& res, const string& snap) {
    if (lnum == 0 && rnum == 0) { // 左右括号都用完了，snap结束
        res.push_back(snap);
        return;
    }
    if (lnum == rnum) { // 左右括号数相等，只能先放左括号，所以剩余的左括号数量永远小于等于右括号
        dfs(lnum-1, rnum, res, snap+'(');
    // 若小于，有两种情况，比如((，下一个可以是左括号也可以是右括号
    } else {
        if (lnum > 0) {
            dfs(lnum-1, rnum, res, snap+'(');
        }
        dfs(lnum, rnum-1, res, snap+')');
    }
}
```

时间效率上，第一种无疑更优秀。
可以发现，其实都是有了回溯的意思。每次dfs之后，都会回到原来的情况