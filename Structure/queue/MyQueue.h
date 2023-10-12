#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <time.h>


class MyQueue
{
public:
	MyQueue(int initSize);
	int peek();
	void push(int num);
	int poll();
	int nextIndex(int size, int index);

private:
	int start;
	int end;
	int size;
	std::vector<int> arr = std::vector<int>(10, 0);
	
};
#endif