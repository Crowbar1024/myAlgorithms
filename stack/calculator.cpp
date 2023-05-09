#include "myheader.h"


// 224 hard 计算器模拟
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