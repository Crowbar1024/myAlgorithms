#ifndef MYSTACK_H
#define MYSTACK_H

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


class MyStack
{
public:
	MyStack(int size);
	int peek();
	void push(int num);
	int pop();
	int getIndex();

private:
	int index;
	std::vector<int> arr = std::vector<int>(10, 0);
	
};
#endif