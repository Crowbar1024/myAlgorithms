list是双向链表。

按照道理用的是 forward_list，即单向链表

list的第一个元素的前向指针总为 null，因为它前面没有元素；同样，尾部元素的后向指针也总为 null。


### 初始化
```cpp
std::list<int> values(n); // 默认0
std::list<int> values(n, val);
std::list<int> values{1,2,3,4}; // 拷贝构造
std::list<int> values(arr.begin(), arr.end()); // 拷贝构造

```


### 常量
```cpp
front(); //	返回第一个元素的引用。
back();
size();
empty();
begin(); // 返回指向容器中第一个元素的双向迭代器。
end(); // 返回指向容器中最后一个元素所在位置的下一个位置的双向迭代器。
```

### 常用函数
```cpp
.push_front(); emplace_front()  // 头插
.push_back(); emplace_back() // 尾插
.pop_front(); // 头删
.pop_back(); // 尾删

.insert(it, val); // 在迭代器位置之前插入新元素
.emplace(it, val) // 同，但只能插一个
.insert(it, n, val);
.insert(it, v.begin(), v.end());
.insert(std::next(it,2), val); // 实现迭代器之后的插入，就用next
.insert(std::prev(it,2), val); // 同理，在更之前
std::advance(it,2) // void，next为正，prev为负



.swap(l1, l2); // 两个链表交换，不是节点交换


.erase(it);

// 寻找On
auto it = find(l.begin(), l.end(), val);
if (it != l.end()) {}


```


### 遍历
```cpp
for(auto it=linklist.begin(); it!=linklist.end(); it++) {
    cout<<*it<<" ";    //从头到尾输出链表的值
}


```cpp
for(int n : l) {
    cout << n << " ";
}
```


### vector多次动态扩容时可以用list替换
```cpp
list<vector<int>> que;
for (int &num : nums) {
    auto it = que.begin();
    for(int i = 0; i < num; ++i) ++it;
    que.insert(it, num*num);
}
return vector<int>{que.begin(), que.end()};
```
