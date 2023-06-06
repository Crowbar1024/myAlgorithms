### 初始化
```cpp
map<int, string> m;  
unordered_map<int, string> m;
unordered_map<int, int> m(5); // 桶数是5
```

### 插入
1. operator[]

如果key不存在，operator[]会进行树的插入，所以**operator[]是非const**的成员函数。所以如果用const修饰并访问kv，只能用find。
```cpp
// 等价于
auto it = m.insert({1, Node()}); // operator[]等同于先默认构造一对象，然后再对其进行赋值。
*(it->second) = Node(1.50); // 因此如果没有默认构造函数，编译会报错。

m[i]++;  // 如果不存在，这条也会生成并+1
```

2. insert

返回一组pair<iterator,bool>
```cpp
m.insert(pair<int, string>(1, "student_one")); // pair初始化有多种方式
m.insert(make_pair(2, "student_two"));  
```

* 当添加新键值对时，insert() 执行效率更高；
* 而更新键值对时，operator[] 的效率更高。

### 遍历
```cpp
for(map<int, string>::iterator iter = m.begin(); iter != m.end(); iter++) {
    cout<<iter->first<<' '<<iter->second<<endl; // 键值对
}

for (auto it : m) { // 是值，不是迭代器
    out<<it.first<<' '<<it.second<<endl; // 键值对
}

for (auto [x, cx]: record) {
// for (auto [_, cx]: record) { // 可以省略
    ans += cx * (cx - 1) / 2;
} 
```

### 查找后修改
```cpp
auto cur = mapStudent.find(key); // return <key,value>
cur->second = newVal; // 哈希表更新节点，没有写默认构造，就只能这样更新

int cnt = mapStudent.count(1);  // 无法定位数据出现位置.

// 在一定能找到的情况下
Node cur = m[1]; // 不推荐这样写，因为operator[]需要有默认构造函数，而且效率较低
```

### 删除
```cpp
mapStudent.erase(iter);  
int n = mapStudent.erase(1);////如果要删除1，用关键字删除  如果删除了会返回1，否则返回0  
mapStudent.erase( mapStudent.begin(), mapStudent.end() );  
```




unordered\_map如何避免哈希冲突

[https://leetcode-cn.com/circle/discuss/H9EIRG/](https://leetcode-cn.com/circle/discuss/H9EIRG/ "https://leetcode-cn.com/circle/discuss/H9EIRG/")

