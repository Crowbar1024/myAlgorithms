1.  禁止range for statement（就是新的那种）去向vector添加元素，因为其等价于传统的for语句（是包含了.end()），一旦在块里面添加或者删除，end函数的值可能就无效了。这种方法其实类似迭代器。

2.  值初始化（有些vector中元素不支持的话就得提供初始化）。int是0，string是空的string对象。

3.  vector初始化最好不要定义好长度，因为C++标准要求能高效添加元素（与Java不同）。只有一种例外，就是所有元素都相同。

4.  返回值类型：size函数返回的值的类型是vectorsize::type，同理下标类型也是相应的size::type。如果用下标去访问不存在的元素，就会产生buffer overflow缓冲区溢出。为了避免这个问题，尽可能使用范围for语句。简单可以用auto去定义下标，麻烦一点用decltype

### 初始化
```cpp
// 一维数组
vector<int> v(3);  // 长度为3
vector<int> v(3,0);  // 长度为3，全为0
vector<int> v; a.resize(3,0);  // 这种写法常用于构造函数，因为上一行似乎不能写在初始化列表中。
vector<int> v{10};  // 第一个元素为10
// 类外类内
vector<vector<int>> used;
used = vector<vector<int>>(row, vector<int>(col, 0));
vector<string> mat(n, string(m, 0));  // 没错，这样也能初始化。并且每行用char遍历也可以

// 赋值
a = {b.begin()+2, b.end()};  // 截取b的一段给a
a.assign(b.begin()+2, b.end());  // 另一种写法
a = b;  // 深拷贝

// 二维数组
vector<vector<int>> v(r, vector<int>(c, 0));  // r*c的二维数组
v[0].resize(4,0);  // 只能一行一行resize
```

### 初始化列表
C11之前
```cpp
class Foo {
private:
    vector<string> name;
    vector<int> val;
public:
    Foo() : name(5), val(5,0) {}
};
```
C11之后 initializer_list
```cpp
class Foo(){
private:
    vector<string> name = vector<string>(5);
    vector<int> val{vector<int>(5,0)};
}
```

### 返回
```cpp
return {};  // 返回空，懒得定义。多维也可以这样
```

### 遍历
```cpp
// 严格类型
for (decltype(v.size()) i = 0; i != 10; i++)
// 拷贝构造（避免使用）
for (int num : v)
// 值引用（要修改）
for (int& num : v)
// 值引用（不要修改）
for (const int& num : v)
```
### 函数
```cpp
.size()
.back()  // 末尾元素，模拟栈常用到
.pop_back()
.push_back()
.emplace_back()  // 建议代替push_back
.empty()
.erase(begin,end)  // 没写end就是后面全部
.insert(it, v.begin(), v.end())  // 在迭代器it前插入b的范围，
.insert(it, cnt, val)  // 当然也可以插入一个值，或者cnt个val
.insert(a.end(), cnt, val)  // 另类追加
```

### 排序
* lambda+pair
```cpp
sort(nums.begin(), nums.end(), [](const auto& a,const auto& b) {
        return a.first*b.second < b.first*a.second;
});
```

* lambda+vector模拟pair
```cpp
sort(nums.begin(), nums.end(), [](const vector<int> &p1, const vector<int> &p2) {
    if (p1[0] == p2[0]) return p1[1] < p2[1];
    return p1[0] < p2[0];
});
```

* struct+内部实现
```cpp
struct point {
    int x;
    int y;
    bool operator<(const point& b) {
        return y < b.y;
    }
    point(int _x, int_depth) ; x(_x), y(_y) {}
};
auto it = *max_element(r.begin(), r.end());
```
* struct+外部实现（如果还有类内，加入cmp的算法会覆盖类内的排序）
```cpp
struct point {
    int x;
    int y;
    point(int _x, int_depth) ; x(_x), y(_y) {}
};
bool cmp(const point& a, const point& b) {
    return a.y < b.y;
}
auto it = *max_element(r.begin(), r.end(), cmp);
```

* struct+lambda（覆盖类内）
```cpp
struct point {
    int x;
    int y;
    point(int _x, int_depth) ; x(_x), y(_y) {}
};
// 将cmp拎出来
auto cmp = [] (const point& a, const point& b) { return a.y < b.y; };
auto it = *max_element(r.begin(), r.end(), cmp);
// 写进去
auto it = *max_element(r.begin(), r.end(), [](const point& a, const point& b) { 
            return a.y < b.y; 
});
```
建议lambda全部写进函数体，或者直接在外部写静态方法static bool cmp（推荐使用static）


### vector扩容
push_back() 向容器尾部添加元素时，首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果是拷贝的话，事后会自行销毁先前创建的这个元素）。

emplace_back() 在实现时，则是直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程。


### insert的一些问题
1. 向没有初始化长度的数组中动态添加元素
```cpp
vector<int> res;
for (int &num : nums) {
    res.insert(res.begin() + num, num*num);
}
return res;
```
2. 向初始化长度的数组中动态添加元素
```cpp
vector<int> res(n, 0);
for (int &num : nums) {
    for (int i = n-2; i >= num; --i) {
        res[i+1] = res[i];
    }
    res[num] = num*num;
}
return res;
```
两种从效率上没多大区别：
1. 由于 vector 中 capicity 的存在，若是发生多次动态扩容会导致多次的全量拷贝。
2. 初始化长度，手动搬运数据，每一次都是On。而1的 capicity 较低时，移动数据长度会短一些。

所以推荐用链表
```cpp
list<vector<int>> que;
for (int &num : nums) {
    auto it = que.begin();
    for(int i = 0; i < num; ++i) ++it;
    que.insert(it, num*num);
}
return vector<int>{que.begin(), que.end()};
```
虽然这样需要花费时间在定位pos上。