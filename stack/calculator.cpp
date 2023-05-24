#include "myheader.h"


/*
	计算器一共有4道
	227 + - * / 空格
	224 + - ( ) 空格
	772
	770

	为编码方便，有一些要求
	'+' 不能用作一元运算(例如， "+1" 和 "+(2 + 3)" 无效)
	'-' 可以用作一元运算(即 "-1" 和 "-(2 + 3)" 是有效的)
	输入中不存在两个连续的操作符
	整数除法仅保留整数部分。
	所有中间结果将在 [-2^31, 2^31 - 1] 的范围内。


	正规流程
	1 中缀转后缀

	不正规流程
	一个栈就行，操作直接在遍历过程中搞定。不过这样需要很多前提。
	比如227，没有括号，有乘除，那么需要栈的节点就是积
	224，有括号，没乘除，那么栈的节点就是括号前的符号

*/

// 227 medium 没有括号，这意味着乘除可以直接对栈底进行操作
int calculate(string s) {
	vector<int> stk;
	char preSign = '+';
	int num = 0;
	int n = s.length();
	for (int i = 0; i < n; ++i) {
		if (isdigit(s[i])) {
			num = num * 10 + (s[i] - '0');
		}
		// 注意最后一次循环也需要结算
		if (!isdigit(s[i]) && s[i] != ' ' || i == n - 1) {
			switch (preSign) { // 2 + 3 * 4，当遍历到*时，prev是+，{2, 3}，最后一次会*4
				case '+':
					stk.push_back(num);
					break;
				case '-':
					stk.push_back(-num);
					break;
				case '*':
					stk.back() *= num;
					break;
				case '/':
					stk.back() /= num;
					break;
				default:
					return 0;
			}
			preSign = s[i];
			num = 0;
		}
	}
	return accumulate(stk.begin(), stk.end(), 0);
}

// 224 hard + - 括号，没有乘除，括号前的符号压栈，这样括号内部的符号和栈顶相乘就是真值
int calculate(string s) {
	stack<int> signStk;
	int preSign = 1; // 数字前的符号
	signStk.emplace(preSign); // 根据题意，第一个数一定为正
	int n = s.length(), ret = 0, num = 0;
	for (int i = 0; i < n; ++i) {
		if (s[i] == ' ') {
			continue;
		} else if (isdigit(s[i])) {
			num = num * 10 + (s[i] - '0');
		} else {
			ret += preSign * num;
			num = 0;
			switch (s[i]) {
				case '+':
					preSign = signStk.top();
					break;
				case '-':
					preSign = -signStk.top();
					break;
				case '(':
					signStk.emplace(preSign);
					break;
				case ')':
					signStk.pop();
					break;
				default:
					return 0;
			}
		}
	}
	return ret + preSign * num;
}



// 224 hard 
struct np {  // int类型数据的计算器
    int val;
    char ope;
    bool jd;  // judge val or operator
};
// 把给的中缀表达式str转换成后缀表达式
// 比如 4^3^2*5-(3-1)/2 变成
// 4 3 2 ^ ^ 5 * 3 1 - 2 /
// 难点在于有可能会出现正号和负号，需要对其进行处理
vector<np> infixToPostfix(string& str) {
	auto itor = remove_if(str.begin(), str.end(), ::isspace);  // 清除空格
  	str.erase(itor, str.end());
	stack<char> operatorStack;
	vector<np> postFixExpression;
    int num = 0;
	for(int i = 0; i < str.size(); ++i) {
		if (str[i] == '(') operatorStack.push(str[i]);
		else if (str[i] == ')') {
			while(operatorStack.top() != '(' && !operatorStack.empty()) {
				postFixExpression.push_back(np{0,operatorStack.top(),false});
				operatorStack.pop();
			}
			operatorStack.pop();
		} else if ((str[i] == '+' && i > 0 && (isdigit(str[i-1]) || str[i-1] == ')')) ||
			 (str[i] == '-' && i > 0 && (isdigit(str[i-1]) || str[i-1] == ')'))) {  // 运算优先级最低
			while(!operatorStack.empty() && (operatorStack.top() == '+' || operatorStack.top() == '-' || operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^')) {
				postFixExpression.push_back(np{0,operatorStack.top(),false});
				operatorStack.pop();
			}
			operatorStack.push(str[i]);
		} else if (str[i] == '-') {
            // negeFlag = 1;
			// 0-
			postFixExpression.push_back(np{0,' ',true});
			operatorStack.push(str[i]);
        } else if (str[i] == '+') {  // 正号，没用，抛弃
			continue;
		} else if (str[i] == '*' || str[i] == '/') {  // 比加减高
			while (!operatorStack.empty() && (operatorStack.top() == '*' || operatorStack.top() == '/' || operatorStack.top() == '^')) {
				postFixExpression.push_back(np{0,operatorStack.top(),false});
				operatorStack.pop();
			}
			operatorStack.push(str[i]);
		} else if (str[i] == '^') {  // 运算优先级最高
			if (!operatorStack.empty() && operatorStack.top() != '^') {  // 乘方运算没有结合律，后面的优先级高
				postFixExpression.push_back(np{0,operatorStack.top(),false});
				operatorStack.pop();
			}
			operatorStack.push(str[i]);
		} else if (isdigit(str[i])) {  // 是数字
            num = 10*num + (str[i]-'0');
            if ((i < str.size()-1 && !isdigit(str[i+1])) || i == str.size()-1) {
                postFixExpression.push_back(np{num,' ',true});
                num = 0;
            }
		}
	}
	while (!operatorStack.empty()) {
		postFixExpression.push_back(np{0,operatorStack.top(),false});
		operatorStack.pop();
	}
	return postFixExpression;
}
int GetResultFromPostfixExpression(vector<np>& postFix) {
	int flag = 1;
	stack<int> digitStack;
	for(np ch : postFix) {
		if (ch.jd) {
			digitStack.push(ch.val);
		} else if (!digitStack.empty() && !ch.jd) {
			int a = digitStack.top(); digitStack.pop();
			int b = digitStack.top(); digitStack.pop();
			int tmp = 1;
			switch(ch.ope) {
				case '+':
					tmp = a + b;
					break;
				case '-':
					tmp = b - a;
					break;
				case '*':
					tmp = a * b;
					break;
				case '/':
					tmp = b / a;
					break;
				case '^':
					while (a--) tmp *= b;
					break;
			}
			digitStack.push(tmp);
		}
	}
	return digitStack.top();
}
int calculate(string s) {
    vector<np> postFix = infixToPostfix(s);
    return GetResultFromPostfixExpression(postFix);
}