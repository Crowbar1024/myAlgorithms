#include "myheader.h"

// easy 796 A是否可以通过把前面部分拼到后面形成B
// 暴力解法n^2，就是让B每次向后移一位，然后用%len这种来比较
// 当然这种思想还可以直接用创建新的string来代替， 即A=A.substr(1) + A[0];
// 以上都可行，下面是比较优雅的解法
bool rotateString(string A, string B) {
    return A.size()==B.size() && (A+A).find(B) != string::npos;
}

// NC1 大数加法 比如 8934 + 87
// 想法1：使用一个空的数组存放结果，从下标0开始计算，最后翻转。
// 想发2：填充短的那个 0087，让两者长度相同，在第一个上进行操作，不需要额外的空间消耗
string bigAdd(string s, string t) {
	int ls = s.size(), lt = t.size();
	if (!ls) return t;
	if (!lt) return s;
	if (ls < lt) {
		for (int i = lt - ls; i > 0; --i) s = '0' + s;
	} else if (ls > lt) {
		for (int i = ls - lt; i > 0; --i) t = '0' + t;
	}
	int tmp = 0, carry = 0;
	for (int i = s.size() - 1; i >= 0; --i) {
		tmp = s[i] - '0' + t[i] - '0' + carry;
		s[i] = tmp % 10 + '0';
		carry = tmp / 10;
	}
	if (carry) s = '1' + s;
	return s;
}

// 43 medium 大数乘法
string multiply(string num1, string num2) {
    if (num1[0] == '0' || num2[0] == '0') return "0";
    int l1 = num1.size(), l2 = num2.size();
    int len = l1+l2;
    vector<int> ret(len, 0);
    for (int i = l1 - 1; i >= 0; --i) {
        for (int j = l2 - 1; j >= 0; --j) {
			// "345" * "16" 数组前面存小的
            ret[len-i-j-2] += (num1[i]-'0')*(num2[j]-'0');
        }
    }
    for (int i = 0; i < len; ++i) {
        if (ret[i] > 9) {
            ret[i+1] += ret[i]/10;
            ret[i] %= 10;
        }
    }
    if (ret[len-1] == 0) {
        ret.pop_back();
        --len;
    }
    string strRet(len, '0');
    for (int i = 0; i < len; ++i) {
        strRet[i] += ret[len-i-1];
    }
    return strRet;
}

// 大数阶乘
// 模拟一个数组，每次这个数组乘一个数
string bigFactorial(int n) {
    if (n == 0) return "1";
	vector<int> num(n, 0);
	num[0] = 1;
	int len = 1;
	// 每次乘 i
	for (int i = 2; i <= n; ++i) {
		int carry = 0;
		for (int j = 0; j < len; ++j) {
			carry += num[j] * i;
			num[j] = carry % 10;
			carry /= 10;
		}
		while (carry) {
			//处理进位 
			num[len++] = carry % 10;
			carry /= 10;
		}
	}
	string r(len, '0');
	for (int i = 0; i < len; ++i) {
		r[i] += num[len-i-1];
	}
	return r;
}
