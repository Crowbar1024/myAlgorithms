### 边搜索边插入
比如经典的两数之和，先把nums全初始化到map中，再一个一个搜索，这样效率太低。
可以先搜索，再插入。这样找到的pair一定是之前插入的。不用全部插入就能得到结果。

### 能用map代替set，效率基本都能提高
比如LC3

### 哈希是字母时，用静态数组就行

### 自己查自己和查0问题
map中用值找pair中的同伴，很容易搜索到自己，注意。
同时一个map往往能取到两个哈希set。

### 滑动窗口
一般如果没有负数，有时候是不需要用到哈希表的。
但是出现了负数和无序这些前提后，就往往需要借助哈希表和前缀和这种数据结构。



### string或者数组做键值
使用质数去设计键值，26个字母对应的质数，每个单次的字母的质数的一定不会重复，但有可能会溢出，取模有概率会碰撞
或者统计出现次数。总之需要自定义哈希函数（49的另类解法）
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        // 自定义对 array<int, 26> 类型的哈希函数
        auto arrayHash = [fn = hash<int>{}] (const array<int, 26>& arr) -> size_t {
            // accumulate()中的第四个参数项要传入一个二元操作(BinaryOperation)规则，告诉它如何将当前元素与累积量做操作。
            // 它隐式地调用(size_t)acc和(int)num这两个量，默认情况下做简单的相加运算。
            return accumulate(arr.begin(), arr.end(), 0u, [&](size_t acc, int num) {
                return (acc << 1) ^ fn(num);  // hash运算结果移位相加
            });
        };

        // 由于key的类型是array<int, 26>，属于用户自定义的一个数据类型，编译器无法针对用户自定义的数据类型做两个元素是否相等的判断
        // 所以，要告诉unordered_map你是如何确定key与key之间是否冲突(或者是否相等)的。
        // 正因如此，unordered_map的API中的第三个参数项就会让用户传入一个运算规则的函数的类型
        // 而前面写的arrayHash这个类(class)实际上是一个lambda expression(你也可以把它看作是一个仿函数(functor)的类)
        // 要获取一个显式的类的类型是容易的，但这里是隐式的，就要借助于decltype获得arrayHash的type了
        // 补充一点，lambda表达式很多都是通过decltype()的技巧获取其类型的，这也是C++11的重大进步之一
        unordered_map<array<int, 26>, vector<string>, decltype(arrayHash)> mp(0, arrayHash);
        for (string& str: strs) {
            array<int, 26> counts{};
            int length = str.length();
            for (int i = 0; i < length; ++i) {
                counts[str[i] - 'a'] ++;
            }
            // mp[counts]过程就会调用arrayHash计算counts的hash值
            // 在哈希表中寻找对应哈希值的篮子(busket)，并将该counts对应的str挂在对应篮子的链表尾部
            // 对于不存在的哈希值，对应的篮子是一个空链表，将str挂篮子的链表尾部这一操作仍然不变，
            // 如此一来就统一了emplace_back()的操作：只负责在篮子的链表的尾部添加string
            mp[counts].emplace_back(str);
        }
        vector<vector<string>> ans;
        for (auto it = mp.begin(); it != mp.end(); ++it) {
            ans.emplace_back(it->second);
        }
        return ans;
    }
};

```


### set与优先队列
优先级队列只允许按照排序顺序访问一个元素 - 即，可以获得最高优先级的项目，而当删除该项目时，可以获得下一个最高优先级，依此类推。

优先级队列还允许重复元素，因此它更像是一个multiset而不是set .

一个set允许以排序顺序进行完全访问，例如，可以在集合的中间某处找到两个元素，然后按照从一个到另一个的顺序遍历。