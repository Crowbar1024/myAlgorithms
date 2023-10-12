#include "../myheader.h"

/*
    记录UFS计算字符串相似度的题
*/

// 737 medium 两个句子是相似的条件：1 具有相同的长度。2 sentence1[i] 和 sentence2[i] 是相似的（是similarPairs的一对）
// 判断两个句子是否相似。注意相似是可传递的
// 如果用一个词去找相似的词，这样复杂度是On^2。所以用一个并查集去统计所有相似的为一棵树。
// 为了不在UFS里面涉及string，在外部建立一个关于string和int的哈希表
bool areSentencesSimilarTwo(vector<string>& sentence1, vector<string>& sentence2, vector<vector<string>>& similarPairs) {
    int l1 = sentence1.size(), l2 = sentence2.size();
    if (l1 != l2) return false;
    unordered_map<string, int> strMap;
    int cnt = 0;
    UFS ufs(2005); // similarPairs最多有2000个string
    for (vector<string>& similarPair : similarPairs) {
        string &s1 = similarPair[0], &s2 = similarPair[1];
        if (strMap.count(s1) == 0) strMap[s1] = cnt++; // 每次新的词会获得一个唯一自增的id
        if (strMap.count(s2) == 0) strMap[s2] = cnt++;
        ufs.unit(strMap[s1], strMap[s2]);
    }
    for (int i = 0; i < l1; ++i) {
        if (sentence1[i] == sentence2[i]) continue; // 直接相等
        if (!strMap.count(sentence1[i]) || !strMap.count(sentence2[i])) return false; // 如果有一个不在哈希表中，说明肯定不相似
        if (ufs.isRelative(strMap[sentence1[i]], strMap[sentence2[i]]) == false) {
            return false;
        }
    }
    return true;
}

// 1061 medium s1==s2，说明每个s1[i]==s[2]，注意相等也有传递性
// 那么把basestr的每个字母变成字典序最小的相似字母
// 即把相等的字母放到一个集合中，然后按照字典序排序，然后baseStr每个字母转换成所在集合的最小字典序字母
string smallestEquivalentString(string s1, string s2, string baseStr) {
    UFS ufs(26);
    for (int i = 0; i < s1.size(); ++i) {
        ufs.unitSeq(s1[i] - 'a', s2[i] - 'a'); // 字典序小的成为集合的头头
    }
    string ret(baseStr);
    for (char& ch : ret) {
        ch = ufs.find(ch - 'a') + 'a';
    }
    return ret;
}