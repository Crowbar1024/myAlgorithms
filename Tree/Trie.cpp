#include "myheader.h"


// 前缀树有多种应用，最多是在查找某个字符串数组中是否存在某个字符串或者字符串的前缀
// 实现方法有多种


// medium 208 这里把node和tree分开，也是CMU15-445 lab0的做法
class TrieNode {
public:
    TrieNode* children[26];
    bool isEnd;
    TrieNode() : isEnd(false) {
        for (int i = 0; i < 26; ++i) children[i] = nullptr;
    }
};
class Trie {
public:
    Trie() { root = new TrieNode(); } // 空，然后每个儿子有26个节点
    ~Trie() { clear(root); } // 递归析构
    void insert(string word) {
        TrieNode* cur = root;
        for (char ch : word) { // 试着每层找到对应的位置插入
            int id = ch-'a';
            if (!cur->children[id]) cur->children[id] = new TrieNode();
            cur = cur->children[id];
        }
        cur->isEnd = true; // 当前word插入后，再最后一个字母处设置终结符
    }
    bool search(string word) {
        TrieNode* cur = searchPrefix(word);
        return cur && cur->isEnd;
    }
    bool startsWith(string prefix) { // 是否有单词符合该前缀
        return this->searchPrefix(prefix);
    }
private:
    TrieNode* root;
    TrieNode* searchPrefix(string word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int id = ch-'a';
            if (!cur->children[id]) return nullptr;
            cur = cur->children[id];
        }
        return cur;
    }
    void clear(TrieNode *cur) { // 递归析构
        for (int i = 0; i < 26; i++){
            if (cur->children[i]) clear(cur->children[i]);
        }
        delete cur; // 前序
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */


/* 
    如果node不分开，全部写在一起，会有以下变化
    私有变量的改变，当然可以写进创构
    Trie* next[26] = {};
    bool isword = false;
    新建的改变
    Trie* node = this;
    node->next[ch] = new Trie();
*/

/*
    如果不用数组用map，并且写在一起
    map<char, Trie*> next = {};
    bool isword = false;
    if (!node->next.count(ch)) { node->next[ch] = new Trie(); }
    node = node->next[ch];
    其实也没多少改变
*/