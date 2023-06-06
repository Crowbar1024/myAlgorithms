### char和int数组初始化

```c
int row[MAXSIZE] = {0};
char type[MAXSIZE] = {'\0'};  // strlen 统计长度
```

### 空间换时间
当需要排序时，看看能不能创建一个大数组


### 使用 strcpy 进行出栈两个进栈一个
比如ABC))这种计算矩阵乘法复杂度
算了BC，加入新的矩阵D，算AD
```c
for(i=0; i<strlen(s); i++){
    if(s[i] == ')'){
        res += row[s[i-2]-'A'] * col[s[i-2]-'A'] * col[s[i-1]-'A'];
        col[s[i-2]-'A'] = col[s[i-1]-'A'];  // B当成新元素D
        strcpy(&s[i-1], &s[i+1]);  // 删除C)
        i -= 2;
    }
}
```
实则为奇淫巧计，当数组元素很大时，拷贝没必要。


### memcpy
与strcpy相比，memcpy并不是遇到'\0'就结束，而是一定会拷贝完n个字节。常用来做内存拷贝，你可以拿它拷贝任何数据类型的对象，可以指定拷贝的数据长度；
```cpp
void *memcpy(void*dest, const void *src, size_t n);
```