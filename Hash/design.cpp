#include "myheader.h"


// 705 easy 设计哈希集合
// 使用链地址法
class MyHashSet {
    vector<list<int>> data;
    static const int base = 769; // 为了尽可能避免冲突，应当将 base 取为一个质数。
    static int hash(int key) { return key % base; }
public:
    MyHashSet() : data(base) {}

    // 向哈希集合中插入值 key
    void add(int key) {
        int hashCode = hash(key);
        auto it = std::find(data[hashCode].begin(), data[hashCode].end(), key);
        if (it != data[hashCode].end()) return; // 存在就不用插了
        data[hashCode].emplace_back(key);
    }
    
    void remove(int key) {
        int hashCode = hash(key);
        auto it = std::find(data[hashCode].begin(), data[hashCode].end(), key);
        if (it != data[hashCode].end()) {
            data[hashCode].erase(it);
        }
    }
    
    bool contains(int key) {
        int hashCode = hash(key);
        auto it = std::find(data[hashCode].begin(), data[hashCode].end(), key);
        if (it != data[hashCode].end()) return true;
        return false;
    }
};


// 706 easy 设计哈希映射
// 依然链地址法，改一下存储结构就行了
class MyHashMap {
    vector<list<pair<int,int>>> data;
    static const int base = 769; // 为了尽可能避免冲突，应当将 base 取为一个质数。
    static int hash(int key) { return key % base; }
public:
    MyHashMap() : data(base) {}
    
    void put(int key, int value) {
        int hashCode = hash(key);
        // 不能用find了，因为只给了key
        for (auto it = data[hashCode].begin(); it != data[hashCode].end(); ++it) {
            if ((*it).first == key) {
                (*it).second = value;
                return;
            }
        }
        data[hashCode].emplace_back(make_pair(key, value));
    }
    
    int get(int key) {
        int hashCode = hash(key);
        for (auto it = data[hashCode].begin(); it != data[hashCode].end(); ++it) {
            if ((*it).first == key) {
                return (*it).second;
            }
        }
        return -1;
    }
    
    void remove(int key) {
        int hashCode = hash(key);
        for (auto it = data[hashCode].begin(); it != data[hashCode].end(); ++it) {
            if ((*it).first == key) {
                data[hashCode].erase(it);
                return;
            }
        }
    }
};