# string

底层是指向真正字符串的指针
[find底层原理](https://www.zhihu.com/question/392846851/answer/1211500754)
### 初始化
```cpp
string s(n,'.');  // n个'.'
string s(1, 'c');  // char转string
string s = "";  // 

string s = s1;  // 浅拷贝，和vector的=操作差不多

char chs[10] = "12345";
string b(s,s+4);  // 复制char数组前4个
```

### 追加
```cpp
s += s1 + s2; // 增加多个string
s += ch;  // 增加一个char，不能增加多个，类型转换的问题
```

### 遍历
```cpp
for (auto c:str)  // char

// .size()的类型是非负整数string:type_size
for (decltype(s.size()) index = 0; index != s.size(); index++)
    s[index] = toupper(s[index]);  // tolower

for (auto a = s.begin(); a != s.end(); a++)
    *a = toupper(*a);  
```
### 查找
```cpp
// 查找从pos开始的"xx"出现的位置，没有pos就是第一个
auto pos = s.find("xx", pos);
if (pos != string::npos)  // 找不到
```
### 截取
```
// 截取从pos开始的n个字符拷贝并返回，没有n就是从pos开始的全部字符，注意s没变
b = s.substr(pos, n);
```

### 删除
```cpp
s.erase(pos, n); // 删除从pos开始的n个字符
s.erase(s.begin()+pos); // 删除第pos个字符
s.erase(s.begin(),s.begin()+pos); // 删除一段
```

### 比较
```cpp
a == b
a.compare(b)
```

### 插入
```cpp
// 插入，返回插入的位置
it = s.insert(s.begin()+5, '.')
```

### 转换
* int转换
```cpp
string s = to_string(num); // 浮点也行
int v = stoi(s);
```
* char*转换
```cpp
char* d = new char[len];
memcpy(d, in.c_str(), len); // c_str()返回当前字符串的首字符地址
```
* const char*
```cpp
const char* str = "1234";
char* str = "1234";
char str[] = {'1','2','3','4'};
string s2 = string(str);
cout << s2.size(); // 没错，都是存储了长度的

```

### 排序
```cpp
// 多个字符串拼接在一起，按字典序排列的快速方法
bool cmp0(string a, string b) {    // 这里就不太适合常量引用
    return a + b < b + a;
}
```

### 模拟栈
```cpp
string s;
s.empty();  // 是否为空
s.back();  // 栈顶
s.push_back(ch);
s.pop_back();
```


# cstring
cstring中字符串一般都是char *


### strcasecmp
比较两个字符串是否相等，\0结尾。
```cpp
if (strcasecmp(method,"GET") == 0)
```

### strcasecmp
strcasecmp指定位数
```cpp
if (strncasecmp(m_url,"http://",7) == 0)
```

### strpbrk
在源字符串（s1）中找出最先含有搜索字符串（s2）中任一字符的位置并返回，若找不到则返回空指针。
```cpp
这里面s2里面有空格和tab，用于分析请求行
char *m_url = strpbrk(text, " \t");
```


### strspn
检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。
```cpp
// m_url此时指向的是第一个空格后的一个字符，但不知道之后是否还有，将m_url向后偏移，通过查找，继续跳过空格和\t字符，指向请求资源的第一个字符
m_url += strspn(m_url, " \t");
```


### strchr
该函数返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。
```cpp
char *strchr(const char *str, int c);

// m_url 为http://www.runoob.com/aa.jpg，找到目标/aa.jpg
m_url = strchr(m_url,'/');

```

### atol
把参数 str 所指向的字符串转换为一个长整数（类型为 long int 型）。
```cpp
long int atol(const char *str);

m_content_length = atol(text);
```


# stringstream
### 根据分隔符读取字符串
```cpp
// str "1,21,-1,1"
vector<int> res;
stringstream input(str); //读取str到字符串流中
string temp;
// 使用getline函数从字符串流中读取,遇到分隔符时停止,和从cin中读取类似
// 注意,getline默认是可以读取空格的
while(getline(input, temp, pattern)) {
    res.emplace_back(stoi(temp));
}
```