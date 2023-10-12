//-----------------------------------[Program Explantion]------------------------------------
//  Problem Source: PAT 
//	2018/8/23	Create by Howie Yao
/*  Description:
	给出插入的顺序
	求最后AVL树的root
*/
//-------------------------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <functional>

using namespace std;
const int INF = 0x3f3f3f3f;
typedef long long ll;
const int maxn = 1e3+5;
const int MOD = 1e9 + 7;

struct node {
	int val;
	node* left;
	node* right;
};

// 右右
node* turnleft(node * root) {
	node* t = root->right;
	root->right = t->left;
	t->left = root;
	return t;
}
// 左左
node* turnright(node * root) {
	node* t = root->left;
	root->left = t->right;
	t->right = root;
	return t;
}
// 右左
node* turnLeftThenRight(node* root) {
	root->left = turnleft(root->left);
	return turnright(root);
}
// 左右
node* turnRightThenLeft(node* root) {
	root->right = turnright(root->right);
	return turnleft(root);
}
int getNodeHeight(node *root) {
	if (root == NULL) return 0;
	else return max(getNodeHeight(root->left), getNodeHeight(root->right)) + 1;
}
node* insert(node *root, int v) {
	if (root == NULL) {
		root = new node();
		root->val = v;
		root->left = root->right = NULL;
	} else if (v < root->val) {
		root->left = insert(root->left, v);
		if (getNodeHeight(root->left) - getNodeHeight(root->right) == 2) {
			root = (v < root->left->val) ? turnright(root) : turnLeftThenRight(root);
		}
	} else {
		root->right = insert(root->right, v);
		if (getNodeHeight(root->left) - getNodeHeight(root->right) == -2) {
			root = (v > root->right->val) ? turnleft(root) : turnRightThenLeft(root);
		}
	}
	return root;
}

int main() {
	freopen("input.txt", "r", stdin);
	int n;
	cin >> n;
	node* root = NULL;
	for (int i = 0; i < n; i++) {
		int tmp;
		cin >> tmp;
		root = insert(root, tmp);
	}
	printf("%d", root -> val);
	return 0;
}