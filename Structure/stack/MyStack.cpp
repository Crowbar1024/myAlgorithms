#include "MyStack.h"

 
MyStack::MyStack(int size) {
	arr = std::vector<int>(size);
	index = 0;
}


int MyStack::peek() {
	if (index == 0) {
		return 0;
	}
	return arr[index - 1];
}

void MyStack::push(int num) {
	if (index == arr.size()) {
		return;    // TODO throw
	}
	arr[index++] = num;
}

int MyStack::pop() {
	if (index = 0) {
		return 0;    // TODO throw
	}
	return arr[--index];
}

int MyStack::getIndex() {
	return index;
}