#include "myheader.h"

// 232 easy 两个栈实现队
// 一个负责输入，一个负责输出
// s1 1 2 3 4  s2 4 3 2 1
class MyQueue {
public:
    stack<int> input, output;
    MyQueue() {}
    void push(int x) { input.push(x); }
    int pop() {
        int ret = peek();
        output.pop();
        return ret;
    }
    int peek() {
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
        return output.top();
    }
    bool empty() {
        return input.empty() && output.empty();
    }
};

// 225 easy 队列实现栈
// 队列先进先出，所以两个队列没啥意义，倒来倒去都一样
// 所以用一个队列试试
// 1 push 1,2,3
// 2 pop
// 3 pop
// 4 push 4,5
// 5 pop
// 1 {1,2,3}
// 2 {2,3,1}-{3,1,2}-{1,2}
// 3 {2,1}-{1}
// 4 {1,4,5}
// 5 {4,5,1}-{5,1,4}-{1,4}
// 可以发现，每次push不变，每次pop需要头到尾size-1次
// 还有一哥难以解决的问题，top和pop之间的矛盾
// 由于默认push没有操作，所以对于先调用top再pop，会多roll一次，导致BUG
// 如何解决呢？直接每次push完做roll就完了。这样top和pop就不会调用roll
// 这样有一个确定，就是每次push都要On
class MyStack {
public:
    queue<int> q;
    MyStack() { }
    void push(int x) { 
        q.push(x);
        int k = q.size()-1;
        while (k--) {
            q.push(q.front());
            q.pop();
        }
    }
    int pop() {
        int tmp = q.front();
        q.pop();
        return tmp;
    }
    int top() {
        return q.front();
    }
    bool empty() { return q.empty(); }
};

