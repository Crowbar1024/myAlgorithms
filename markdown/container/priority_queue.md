优先队列一般用来找第k小的数，允许重复。
不重复的话一般用set（不是hash版的unordered_set）去做合适。



定义如下

```cpp
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class priority_queue;
priority_queue<Type, Container, Functional>  // 模板
// Type 为数据类型， Container 为保存数据的容器，Functional 为元素比较方式。
// STL里面默认用的是 vector。比较方式默认用 less。
// 如果是pair<itn,int>，默认先比第一个，在比第二个，也是大顶堆、
```
根据定义，less比较方式是大顶堆（与常识相悖），即队头为最大的数。

所以找第k小的数的原理：大小设置为k，每次都是弹出最大的数，弹出若干次，最后剩下的都是前k小的数，队头是其中最大的数，即全部数组中第k小的数。
可以发现这样做需要遍历完所有的数组。所以这种方法一般不是最优的，容易超时。
如果可以轻易知道最小的数的位置，那么也可以用小顶堆做，遍历k-1次，最后队头为要找的数。

### 初始化
```cpp
// int数组
priority_queue<int> pq;  // 默认大顶堆
priority_queue<int, vector<int>, greater<int>> pq;  // 小顶堆
priority_queue<int> q2(data.begin(), data.end());  // 用一个vector来做初始化
priority_queue<pair<int,int>> pq;  // 先first，再
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
```

### 常用函数
```cpp
.size()
.top()  // 队头，注意pq不是queue，而是堆。queue的队头是front
.empty()
.push()
.emplace()  // 推荐取代push，同理queue和stack
.pop()  // 队头弹出
```

### 自定义排序
* struct+内部实现
```cpp
struct Game {
    int t;
    int w;
    Game(int _t, int _w) : t(_t), w(_w) {}
    bool operator<(const Game& b) const {  // 一定要加const
        return w < b.w;  // 每次队头都是最大
    }
};
priority_queue<Game> pq;  // 会使用内置
```

* lambda+decltype
```cpp
auto cmp = [] (const Game& a, const Game& b) { return a.w > b.w; };
priority_queue<Game, vector<Game>, decltype(cmp2)> pq(cmp);
```

* struct+外部实现+decltype
```cpp
static bool cmp(const Game& a, const Game& b) {
    return a.w > b.w; 
}
priority_queue<Game, vector<Game>, decltype(&cmp)> pq(cmp);
```

> 不能在内部写lambda表达式
根据优先队列的定义，Compare有typename关键字，这意味着需要指定类型。但 lambda 对象的类型是匿名的，无法单独使用，所以需要用decltype确定类型。
而sort（STL算法函数，PQ初始化是构造函数）可以接受一个函数对象，所以直接给传递lambda对象是可以的，因为lambda对象属于函数对象。
在C++里凡是能够使用函数调用运算符的，都是函数对象。