#include "myheader.h"



// 20 easy 判断是否有效，并且有3种括号(){}[]
// 为了代码的简单，可以用哈希表。
bool isValid(string s) {
    if (s.size() & 1) return false;
    unordered_map<char, char> tb = {{')','('}, {']','['}, {'}','{'}}; // 右括号找左括号
    stack<char> sk;
    for (char ch : s) {
        if (!tb.count(ch)) sk.emplace(ch);  // 左括号直接压
        else { // 由于栈的性质，遇到一个)，如果栈顶是(，说明之间可能存在一大堆别的已经弹出了
            if (sk.empty() || sk.top() != tb[ch]) return false;
            sk.pop();
        }
    }
    return sk.empty();
}
// 衍生：JD2018春招03 如果最多只能交换字符串（一堆(和)）其中的一组数据，能否使字符串正确
// 思路就是用栈遍历这个字符串，观察栈里最后剩下的括号情况是否合理



// 22 medium 生成所有的由n对括号组成的合法组合。
// 应该没有大于的情况，不然多余的右括号没有匹配了
vector<string> generateParenthesis(int n) {
    vector<string> ret;
    string snap = "";
    dfs(n, n, ret, snap);
    return ret;
}
void dfs(int lnum, int rnum, vector<string>& ret, string& snap) {
    if (lnum == 0 && rnum == 0) { // 左右括号都用完了，snap结束
        ret.push_back(snap);
        return;
    }
    if (lnum == rnum) { // 左右括号数相等，只能先放左括号，所以剩余的左括号数量永远小于等于右括号
        snap += "(";
        dfs(lnum-1, rnum, ret, snap);
        snap.pop_back();
    // 若小于，有两种情况，比如((，下一个可以是左括号也可以是右括号
    } else {
        if (lnum > 0) {
            snap += "(";
            dfs(lnum-1, rnum, ret, snap);
            snap.pop_back();
        }
        snap += ")";
        dfs(lnum, rnum-1, ret, snap);
        snap.pop_back();
    }
}


// 1003 medium ""-"abc"-"aabcbc"...
// 可以从任意地方插入新的"abc"，判断s是否是这样产生的
// 和两个括号的匹配思路完全相同，右括号碰到左括号消掉。那么c碰到前面的ab也能消掉
bool isValid(string s) {
    stack<char> sk;
    for (char ch : s) {
        if (ch == 'a' || ch == 'b') {
            sk.push(ch);
        } else { // ch == c
            if (sk.size() < 2 || sk.top() != 'b') { // c前面是a或者不够ab，不存在的情况
                return false;
            }
            sk.pop(); // 弹出b
            if (sk.top() != 'a') {
                return false;
            }
            sk.pop(); // 弹出a
        }
    }
    return sk.empty();
}


// NC175 ()*，*可以视为单个左括号，也可以视为单个右括号，或者视为一个空字符
// 判断是否有效
// (*)) 和  ((*) 就是有效的
// 直接建两个栈，存下标
// 这里面有个反直觉的地方，如果碰到），优先弹出（是合理的，不管中间有没有*，反正*通用。
bool isValidString(string s) {
    stack<int> s1, s2;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') s1.push(i);
        else if (s[i] == '*') s2.push(i);
        else {
            if (!s1.empty()) s1.pop();
            else if (!s2.empty()) s2.pop();
            else return false;
        }
    }
    while (!s1.empty() && !s2.empty()) {
        int p1 = s1.top(); s1.pop();
        int p2 = s2.top(); s2.pop();
        if (p1 > p2) return false;
    }
    return s1.empty();
}

// 1614 easy 括号的最大嵌套深度 保证一定是有效
// 遇到( 就可以+1，甚至不需要考虑进出栈（因为有效）
int maxDepth(string s) {
    int ret = 0, tmp = 0;
    for (char ch : s) {
        if (ch == '(') ++tmp;
        else if (ch == ')') {
            ret = max(ret, tmp);
            --tmp;
        }
    }
    return ret;
}




// 1190 medium 将()中的字符串翻转，并在输出中删除括号。如输入为 (uoy(love)i)，输出为iloveyou
// 最简单的思路，用栈记录(的位置，即res的size，每次遇到)就在res上翻转一段()。时间复杂度较高。
// 其实已经有了一定思路，就是栈记录左括号的位置，比如(uoy(love)i)，两个括号的位置是{0,11}和{4,9}，
// 当遍历到第一个左括号时，res跳跃到对应的右括号，然后逆向遍历字符。然后每次遇到)，跳到love...
string reverseParentheses(string s) {
    vector<int> partner(s.size()); // patner[i]=j表示两个括号的相对位置
    stack<int> idx; // 记录左括号的位置
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') idx.emplace(i);
        else if (s[i] == ')') {
            int j = idx.top(); idx.pop();
            partner[i] = j;
            partner[j] = i;
        }
    }
    string ret = "";
    int cur = 0, dir = 1; // 开始是正向
    while (cur < s.size()) {
        if (s[cur] == '(' || s[cur] == ')') { // 遇到括号就反转遍历顺序
            cur = partner[cur];
            dir = -dir;
        } else ret += s[cur];
        cur += dir; // 前进
    }
    return ret;
}

// 32 hard 最长连续有效括号串的长度
// 难点即连续。比如()(()，连续有效的最长是2，一次遍历后只剩了一个(，显然是不符合题意的
int longestValidParentheses(string s) {
    stack<int> sk;
    sk.emplace(-1); // 哨兵，避免右括号开局时无法pop
    int ret = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') sk.emplace(i); // 压入左括号的下标
        else {
            sk.pop(); // 1 匹配一个左括号 2 左括号匹配完了，弹出哨兵，待会记录最新的没有匹配的右括号下标
            if (sk.empty()) sk.emplace(i); // 压入最新的没有匹配的右括号下标
            else ret = max(ret, i-sk.top()); // 只匹配了一个左括号也要更新长度（上一个没有匹配的左/右括号和匹配到的右括号之差）
        }
    }
    return ret;
}
// 还有一种双指针的做法，利用22的思路，即左括号和右括号的数量来判断有效匹配的长度
int longestValidParentheses(string s) {
    int ret = 0, l = 0, r = 0;
    for (int i = 0; i < s.size(); ++i) { // 正向遍历
        if (s[i] == '(') ++l;
        else ++r;
        if (l == r) ret = max(ret, 2*l); //  数量相等，说明此时是有效串
        else if (l < r) l = r = 0; // ..右括号第一次出现时，会自动初始
    }
    l = r = 0;
    for (int i = s.size()-1; i >= 0; --i) {
        if (s[i] == '(') ++r; // 反过来
        else ++l;
        if (l == r) ret = max(ret, 2*l);
        else if (l < r) l = r = 0;
    }
    return ret;
}


// 394 medium 字符串解码 n[str] n个str，str中只有字母，结果中也只有字母
// "3[a2[c]]" --> "accaccacc"
// 数字放数字栈，字母放字母栈
string decodeString(string s) {
    stack<string> sts;
    stack<int> stn;
    string ret = ""; // 记录的其实是当前[]中的数，遇到[就重置，遇到]就更新
    for (int pos = 0; pos < s.size(); ++pos) {
        if (isalpha(s[pos])) { // 字母存在res中，遇到下一个[时，将[ret]放进字母栈
            ret += s[pos];
        } else if (isdigit(s[pos])) {
            int num = s[pos]-'0';
            while (isdigit(s[++pos])) num = 10*num+s[pos]-'0'; // 得到num
            stn.emplace(num);
            --pos;  // 之前过了一格
        } else if (s[pos] == '[') {
            sts.emplace(ret);  // 3[ABC]1[A]，第二个[出现时，把ABCABCABC存进去
            ret = "";  // 开始记录下一段[]
        } else { // num[ret]，一对结束，将其展开赋值给res
            int num = stn.top(); stn.pop(); // []前必然是数字
            string tmps = "";
            while (num--) tmps += ret;  // num[ret]
            ret = sts.top()+tmps;  // 把之前存的和这一次倍增的加起来，注意顺序
            sts.pop(); // 之前存的取出来
        }
    }
    return ret;
}
