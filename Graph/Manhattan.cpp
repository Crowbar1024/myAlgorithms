#include "myheader.h"

// nyistOJ 7 街区最短路径问题  
// 曼哈顿距离
// 一个街区有很多住户，街区的街道只能为东西、南北两种方向。住户只可以沿着街道行走。
// 用(x,y)来表示住户坐在的街区。例如（4,20），表示用户在东西方向第4个街道，南北方向第20个街道。
// 现在要建一个邮局，使得各个住户到邮局的距离之和最少。即求最短的曼哈顿距离
// 由题意得，这个点的横坐标就是所有点的横坐标的中位数，同理纵坐标。
int minStreetDistance(vector<int> x, vector<int> y) {
    int n = x.size();
    sort(x.begin(), x.end());
    sort(y.begin(), y.end());
    int ret = 0;
    for (int i = 0; i < n/2; i++) {
        ret += x[n-i-1]- x[i] + y[n-i-1] - y[i];
    }
    return ret;
}

// 1131 最远曼哈顿距离
// 给定两个数组，求|xi-xj|+|yi-yj|+|i-j|
// 其实不管是如何的形式，绝对值的和都可以拆成普通的求和
// 并且由于i和j可以呼唤，最终的形式数量要变成一半
// A = arr1[i] + arr2[i] + i
// B = arr1[i] + arr2[i] - i
// C = arr1[i] - arr2[i] + i
// D = arr1[i] - arr2[i] - i
// 最终可以化为的形式
// max{ret} = max{max(A)-min(A),max(B)-min(B),max(C)-min(C),max(D)-min(D)}