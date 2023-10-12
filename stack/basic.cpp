#include "myheader.h"


// 150 medium 逆波兰表达式求值 ["2","1","+","-3","*"] -> ((2 + 1) * -3) = -9
// 支持加减乘除
int evalRPN(vector<string>& tokens) {
    stack<int> stn;
    for (const string& s : tokens) {
        if (s!= "+" && s!= "-" && s!= "*" && s!= "/") { // 不能写 isdigit，因为有负数干扰
            stn.emplace(stoi(s));
        } else {
            int tmp2 = stn.top(); stn.pop(); // 注意顺序
            int tmp1 = stn.top(); stn.pop();
            switch (s[0]) {
            case '+':
                stn.emplace(tmp1+tmp2);
                break;
            case '-':
                stn.emplace(tmp1-tmp2);
                break;
            case '*':
                stn.emplace(tmp1*tmp2);
                break;
            case '/':
                stn.emplace(tmp1/tmp2);
                break;
            default:
                break;
            }
        }
    }
    return stn.top();
}

// easy 155 最小栈，故名意思，可以返回栈中最小的元素
// 核心思路，两个栈，一个正常，一个只存放最小值。比如，存3 4 2 3
// [3               [3
// [3 4             [3 3
// [3 4 2           [3 3 2
// [3 4 2 3         [3 3 2 2
class MinStack {
private:
    stack<int> st, minSt;
public:
    MinStack() {}
    void push(int val) {
        st.push(val);
        if (minSt.empty() || val < minSt.top()) {
            minSt.push(val);
        } else {
            minSt.push(minSt.top());
        }
    }
    void pop() {
        if (st.empty()) return;
        st.pop();
        minSt.pop();
    }
    int top() { return st.top(); }
    int getMin() { return minSt.top(); }
};


// PAT 1051 [1:n]按次序压入栈，m为栈的大小，v为出栈的顺序，判断v是否正确
// 模拟每个出栈元素的出栈顺序
bool PopSequence(int n, int m, vector<int>& v) {
    stack<int> s;
    int cur = 0;
    for (int i = 1; i <= n; ++i) {
        s.push(i);
        if (s.size() > m) break; 
        while (!s.empty() && s.top() == v[cur]) { // 栈顶为cur
            s.pop();
            ++cur;
        }
    }
    if (cur == n) return true;
    return false;
}
// 946 medium 一个是入栈顺序，一个是出栈顺序，问是否匹配 一摸一样
bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
    stack<int> s;
    int cur = 0; // 出栈
    for (int n : pushed) {
        s.emplace(n);
        while (!s.empty() && s.top() == popped[cur]) {
            s.pop();
            ++cur;
        }
    }
    if (s.empty()) return true;
    return false;
}


