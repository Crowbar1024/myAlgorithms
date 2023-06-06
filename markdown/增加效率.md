# 使用函数

### 交换数组
别傻乎乎的创建tmp交换，直接递归
```cpp
vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() > nums2.size()) return intersect(nums2, nums1);
}
```

### 替换
新值（右）换旧值（左），并返回旧值。\<atomic\> 确保并发的安全
注意，a.exchange(val)要求a是atomic变量

```cpp
// 第一种，线程不安全
int tmp = dp1;
dp1 = dp1 + dp2;
dp2 = tmp;

// 线程安全
dp2 = exchange(dp1, dp1+dp2);  // 斐波那契
```


### 反转
```cpp
reverse(v.begin(),v.end());
// reverse_copy是不改变原顺序，创建新的空间
```

### 降序排序
```cpp
sort(v.rbegin(), v.rend());
sort(v.begin(), v.end(), greater<int>());
```

### 是否有序
```cpp
is_sorted(v.begin(),v.end());  // vector<string> 判断的是string间的
```

### 获取极值和得到下标
不要想着二维也能用，不存在的，老老实实遍历。
```cpp
auto it = max_element(dp.begin(), dp.end());  // 返回的是迭代器位置，同理min_element
int maxW = *it;
int maxV = distance(dp.begin(), it);  // 相当于it-v.begin()
return vector<int>{maxV, maxW};
```

### 排序去重
傻瓜版
```cpp
sort(a.begin(), a.end());
// unique 会把有序重复的变成有序唯一，并返回第一个未定值的位置
a.erase(unique(a.begin(), a.end()), a.end());
```
使用set
```cpp
set<int> s(a.begin(), a.end());
```

### 累加（数字/字符串拼接）
```cpp
int sum = accumulate(nums.begin(), nums.end(), 0);


// 不能在accumulate里面写""，会报错
vector<string> s{"a","123","bc"};
string res = "";
res = accumulate(s.begin(), s.end(), res);
// 但这样就不会了
string res = accumulate(s.begin(), s.end(), string(""));
```

### 填充
```cpp
fill(nums.end()-cnt, nums.end(), 0);
```



### 获取一段连续的相等元素
```cpp
// pair<vector<int>::iterator,vector<int>::iterator>
auto bounds = equal_range(v.begin(), v.end(), 20, mygreater)
cout << bounds.first - v.begin();    // second too
```

### 大于等于
lower_bound、upper_bound，大于等于，大于
找不到返回.end()。在字符串里面是按照字典序

set_intersection




### 统计次数
```cpp
count(v.begin(), v.end(), val); // 统计容器中等于val的个数
count_if(v.begin(), v.end(), func); // 统计容器中func为true的元素
count_if(v.begin(), v.end(), val, func); // 统计容器中func为true且为val的元素
count_if(v.begin(), v.end(), bind2nd(less<int>(), 5)) // 统计<5的
// 绑定第二个参数，less是仿函数。同理greater
```


### 删除
remove()和remove_if() 函数会返回移除目标成员之后的区间的尾后迭代器。
```cpp
auto itor = remove(str.begin(), str.end(), 'e');  // ::isspace空格
str.erase(itor, str.end());
```

### 寻找第一个
```cpp
// 不一定需要有序
auto itor = find(v.begin(), v.end(), 10);
if (itor != v.end()) {...}  // 找不到
cout << *itor  // 找得到
int idx = distance(v.begin(), itor);  // 位置
```

### if 效率
尝试用 flag &= 避免很多的ifelse判断


### 减少for
```cpp
all_of() 算法会返回 true，前提是序列中的所有元素都可以使函数指针返回 true。
any_of() 算法会返回 true，前提是序列中的任意一个元素都可以使函数指针返回 true。
none_of() 算法会返回 true，前提是序列中没有元素可以使函数指针返回 true。
```
