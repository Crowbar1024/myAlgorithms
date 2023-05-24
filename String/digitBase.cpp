#include "myheader.h"


// 58 easy s由若干单词组成，单词前后用一些空格字符隔开。返回s的最后一个单词的长度。
// 逆序遍历即可，不能用ss，因为只能识别一个空格
int lengthOfLastWord(string s) {
    stringstream ss(s);
    string tmp;
    while (ss >> tmp) {}
    return tmp.size();
}

// 7 medium 32位的有符号整数x ，返回将x中的数字部分反转后的结果。
// 如果反转后整数超过 32 位的有符号整数的范围，就返回0。
// 假设环境不允许存储 64 位整数（有符号或无符号）。
int reverse(int x) {
    int ret = 0;
    while (x) {
        if (ret<INT_MIN/10 || ret>INT_MAX/10) return 0;  // 下一次要变大/小，先判断
        int tail = x%10;  // 这里需要-89%10=9，所以正常的就行
        ret = ret*10+tail;
        x /= 10;
    }
    return ret;
}

// 8 medium 将字符串转换成一个 32 位有符号整数
// 读入字符串并丢弃无用的前导空格
// 检查下一个字符（假设还未到字符末尾）为正还是负号，读取该字符（如果有）。 确定最终结果是负数还是正数。 如果两者都不存在，则假定结果为正。
// 读入下一个字符，直到到达下一个非数字字符或到达输入的结尾。字符串的其余部分将被忽略。
// 将前面步骤读入的这些数字转换为整数（即，"123" -> 123， "0032" -> 32）。如果没有读入数字，则整数为 0 。必要时更改符号（从步骤 2 开始）。
// 如果整数数超过 32 位有符号整数，返回最近的边界值
// 有个特殊情况：+-12 返回0; 00000-54a 返回0； "   +0 123" 返回0
int myAtoi(string s) {
    int ret = 0;
    int sign = 1;
    bool flag = false;  // 判断只有一个正负号
    for (char ch : s) {
        if (!flag && ch == ' ') continue;
        else if (!flag && (ch == '-' || ch == '+')) {
            if (ch == '-') sign = -1;
            flag = true;
        }
        else if (isdigit(ch)) {
            flag = true;
            int num = ch-'0';
            if (ret<INT_MIN/10 || (ret==INT_MIN/10 && -num<=INT_MIN%10)) return INT_MIN;
            else if (ret>INT_MAX/10 || (ret==INT_MAX/10 && num>=INT_MAX%10)) return INT_MAX;
            ret = ret*10+sign*num;
        } else break;
    }
    return ret;
}

// 9 easy 32位有符号整数是否回文 不能用字符串
// 最简单的就是利用一个数组存，但可以每次把末尾的数倒转过来变成新的数
// 奇数 1221 12 12
// 偶数 121  1 12
// 但有特殊情况，比如0结尾的数，必须先判断
bool isPalindrome(int x) {
    if (x<0 || (x%10==0 && x!=0)) return false;
    int revn = 0;
    while (x > revn) {
        revn = revn*10+x%10;  // 翻转成另一个数
        x /= 10;
    }
    return x==revn || x==revn/10;
}


// 12 medium 整数转罗马数字
// 也可以硬编码，即把每一位上的数字用数组全写一遍，然后直接拼接起来
// thousands[num / 1000] + hundreds[num % 1000 / 100] + tens[num % 100 / 10] + ones[num % 10];
// 从大的开始减
string intToRoman(int num) {
    const vector<pair<int, string>> tb{
        {1000, "M"},
        {900,  "CM"},
        {500,  "D"},
        {400,  "CD"},
        {100,  "C"},
        {90,   "XC"},
        {50,   "L"},
        {40,   "XL"},
        {10,   "X"},
        {9,    "IX"},
        {5,    "V"},
        {4,    "IV"},
        {1,    "I"},
    };
    string roman;
    for (const auto &[value, symbol] : tb) {
        while (num >= value) { // 1 <= num <= 3999
            num -= value;
            roman += symbol;
        }
        if (num == 0) break;
    }
    return roman;
}
// 13 easy 反过来
int romanToInt(string s) {
    unordered_map<char, int> tb{
        {'M', 1000},
        {'D', 500},
        {'C', 100},
        {'L', 50},
        {'X', 10},
        {'V', 5},
        {'I', 1},
    };
    int ret = 0, cur = 1; // cur为当前的最大的位
    // "LVIII" -- 50 + 5 + 3
    for (int i = s.size()-1; i >= 0; --i) {
        int val = tb[s[i]];
        if (val >= cur) {
            ret += val;
            cur = val;
        } else {
            ret -= val;
        }
    }
    return ret;
}
// 273 hard 整数转英文
const vector<string> singles = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
const vector<string> teens = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
const vector<string> tens = {"", "Ten", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
const vector<string> thousands = {"", "Thousand", "Million", "Billion"};
string numberToWords(int num) {
    if (!num) return "Zero";
    string ret = "";
    for (int base = 1e9, i = 3; i >= 0; base /= 1e3, --i) { // 分成4段
        int snap = num/base;
        if (snap) {
            ret += numberToWordsHelp(snap) + thousands[i] + " ";
            num -= snap*base;
        }
    }
    while (ret.back() == ' ') ret.pop_back();
    return ret;
}
string numberToWordsHelp(int num) { // 3位数转换
    string ret = "";
    if (num >= 100) {
        ret += singles[num/100] + " Hundred ";
        num %= 100;
    }
    if (num >= 20) {
        ret += tens[num/10] + " ";
        num %= 10;
    }
    if (num >= 10) {
        ret += teens[num-10] + " ";
    } else if (num > 0) { // 50 只要输出 fifty
        ret += singles[num] + " ";
    }
    return ret;
}