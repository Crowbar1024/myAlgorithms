#include "MyQueue.h"

 
MyQueue::MyQueue(int initSize) {
	arr = std::vector<int>(initSize);
	start = end = size = 0;
}


int MyQueue::peek() {
	if (size == arr.size()) {
		return 0;    // TODO throw
	}
	return arr[start];
}

void MyQueue::push(int num) {
	if (size == arr.size()) {
		return;    // TODO throw
	}
	size++;
	arr[end] = num;
	end = nextIndex(arr.size(), end);
}

int MyQueue::poll() {
	if (size = 0) {
		return 0;    // TODO throw
	}
	size--;
	int tmp = start;
	start = nextIndex(arr.size(), start);
	return arr[tmp];
}

int MyQueue::nextIndex(int size, int index) {
	return index == size - 1 ? 0 : index + 1;
}