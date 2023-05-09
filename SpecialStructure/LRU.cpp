#include "myheader.h"

/*
    LRU (Least Recently Used) 算法一般是用「链表」作为数据结构来实现的，链表头部的数据是最近使用的，而链表末尾的数据是最久没被使用的。
    那么，当空间不够了，就淘汰最久没被使用的节点，也就是链表末尾的数据，从而腾出内存空间。
    大多数情况下，LRU算法对热点数据命中率是很高的。 但如果突然大量偶发性的数据访问，会让内存中存放大量冷数据，也就是缓存污染。

    LFU (Least Frequently Used) 为了确定最不常使用的键，可以为缓存中的每个键维护一个[使用计数器]。使用计数最小的键是最不常使用的键。
    当一个键首次插入到缓存中时，它的使用计数器被设置为 1 。对缓存中的键执行 get 或 put 操作，使用计数器的值将会递增。
    当存在平局（即两个或更多个键具有相同使用频率）时，应该去除最近最久未使用的键。即老员工工作效率和新员工一致时，开除老员工。
*/


// 146 hard LRU算法
// 当命中缓存，就直接把该页对应的 LRU 链表节点移动到链表的头部。
// 否则，要把新页放入到 LRU 链表的头部，如果容量超过，还要淘汰 LRU 链表末尾的页。
// 1. 因为要访问链表尾部，所以使用双向链表。
// 2. 因为要通过key来定位，所以要使用哈希表找到key在双向链表中的位置。
struct DeListNode {
    int key, val;
    DeListNode *next, *prev;
    DeListNode(int _key = 0, int _val = 0) : key(_key), val(_val), next(nullptr), prev(nullptr) {}
};
class LRUCache {
    int _capacity, size;
    DeListNode *head, *tail;
    unordered_map<int, DeListNode*> pos;
public:
    LRUCache(int capacity): _capacity(capacity), size(0) {
        head = new DeListNode();
        tail = new DeListNode();
        head->next = tail;
        tail->prev = head;
        pos.clear();
    }

    // 如果关键字 key 存在于缓存中，则返回关键字的值，并将其移动到链表的头部，最后返回该节点的值。否则返回 -1 。
    int get(int key) {
        if (pos.count(key)) {
            moveToHead(pos[key], 0);
            return pos[key]->val;
        }
        return -1;
    }
    
    // 如果 key 已经存在，则变更其 value ；
    // 如果不存在，则向缓存中插入该组 key-value 。
    // 不管存不存在在链表的头部添加该节点
    // 如果插入操作导致关键字数量超过 capacity ，则逐出最久未使用的关键字。
    void put(int key, int value) {
        if (pos.count(key)) {
            pos[key]->val = value;
            moveToHead(pos[key], 0);
        } else {
            DeListNode* newNode = new DeListNode(key, value);
            pos[key] = newNode;
            moveToHead(newNode, 1); // 直接插链表
            ++size;
            if (size > _capacity) {
                DeListNode* deleteNode = tail->prev;
                pos.erase(deleteNode->key);
                deleteNode->prev->next = tail;
                tail->prev = deleteNode->prev;
                delete deleteNode;
                --size;
            }
        }
    }

    // 因为get和put都需要将节点放到链表最前这一步，所以将这步拆成一个函数。
    // 同时新创建的节点不需要调整原位置的关系，用一个flag判断
    void moveToHead(DeListNode* node, int flag) {
        if (node->prev == head) return;
        if (!flag) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        node->next = head->next;
        head->next = node;
        node->next->prev = node;
        node->prev = head;
    }
};


// 460 hard LFU缓存
// 第一种实现，因为每次更新需要重新排列，用内置红黑树的set和优先队列都可以。因为需要重复的key，所以使用set
struct Node {
    int cnt; // 缓冲使用频率，get和put都要+1
    int time; // 缓存从创建到现在的时间
    int key, val;
    Node(int _cnt, int _time, int _key, int _val) : cnt(_cnt), time(_time), key(_key), val(_val) {}
    bool operator< (const Node& rhs) const { // 相同使用频率让最新的放前面
        return cnt == rhs.cnt ? time < rhs.time : cnt < rhs.cnt;
    }
};
class LFUCache {
    int _capacity;
    int time; // 创建LFUCache之后的时间
    set<Node> cache;
    unordered_map<int, Node> pos;
public:
    LFUCache(int capacity) : _capacity(capacity), time(0) {
        cache.clear();
        pos.clear();
    }
    
    int get(int key) {
        if (pos.count(key)) {
            return update(key, -1);
        }
        return -1;
    }
    
    // 如果 key 已存在，则变更其值；如果键不存在，请插入键值对。
    // 当缓存达到其容量 capacity 时，则应该在插入新项之前，移除最不经常使用的项。
    // 在此问题中，当存在平局（即两个或更多个键具有相同使用频率）时，应该去除最近最久未使用的键。
    void put(int key, int value) {
        if (!_capacity) return; // 存在特殊情况
        if (pos.count(key)) {
            update(key, value);
        } else {
            // 此时需要考虑一件事，创建新节点之后删除，还是删除后再插新节点
            if (pos.size() == _capacity) {
                pos.erase(cache.begin()->key); // 注意顺序
                cache.erase(cache.begin()); // 删除二叉树的根节点
            }
            // 由于使用频率的存在，新节点容易刚创建就被删除，不符合题意，所以选择后者
            Node newNode = Node(1, ++time, key, value);
            cache.insert(newNode);
            pos.insert(make_pair(key, newNode));
        }
    }

    // 更新相关节点的信息。0 <= value <= 1e9，所以用负数作为不更新的标志
    // 二叉树缓存的更新很不好，先删除再插入
    int update(int key, int value) {
        auto it = pos.find(key);
        Node cur = it->second;
        cache.erase(cur);
        cur.cnt += 1;
        cur.time = ++time;
        if (value >= 0) cur.val = value;
        cache.insert(cur);
        it->second = cur; // 哈希表更新节点，没有写默认构造，就只能这样更新
        return cur.val;
    }
};
// 显然操作的时间复杂度瓶颈在于平衡二叉树的插入删除均需要 O(logn) 的时间。
// 如果不用AVL tree，如何实现呢？需要新的/频率高的在数据结构前面。干脆用一个哈希表，key为频率，value为一个链表。
// 链表是具有相同key的node，每次更新就把相关节点放到(key+1)的表头。更新时间复杂度由Ologn降低到O1
struct Node {
    int key, val, cnt;
    Node(int _key, int _val, int _cnt) : key(_key), val(_val), cnt(_cnt) {}
};
class LFUCache {
    int _capacity;
    int minFreq; // 用来删除一整个链表用的
    unordered_map<int, list<Node>> cache; // 相同出现频率的跟着一串链表
    unordered_map<int, list<Node>::iterator> pos; // 通过key定位链表节点的位置，上面n*m个节点都可以定位
public:
    LFUCache(int capacity) : _capacity(capacity), minFreq(0) {
        cache.clear();
        pos.clear();
    }
    int get(int key) {
        if (pos.count(key)) {
            return update(key, -1);
        }
        return -1;
    }
    void put(int key, int value) {
        if (!_capacity) return; // 存在特殊情况
        if (pos.count(key)) {
            update(key, value);
        } else {
            if (pos.size() == _capacity) {
                auto cur = cache[minFreq].back(); // 不是迭代器
                pos.erase(cur.key);
                cache[minFreq].pop_back();
                if (cache[minFreq].empty()) cache.erase(minFreq); // 这里不用更新minfreq，因为后面插入时会变为1
            }
            cache[1].emplace_front(Node(key, value, 1));
            pos.insert(make_pair(key, cache[1].begin()));
            minFreq = 1; // 新插入的那个频率最小
        }
    }
    int update(int key, int value) {
        auto it = pos.find(key);
        auto cur = it->second;
        int freq = cur->cnt, val = cur->val;
        if (value >= 0) val = value;
        cache[freq].erase(cur); // 从原来频率的链表中删除
        if (cache[freq].empty()) {
            cache.erase(freq);
            if (freq == minFreq) ++minFreq; // 因为这里某个频率的节点全没了，要看情况更新最小频率
        }
        cache[freq+1].emplace_front(Node(key, val, freq+1));
        pos[key] = cache[freq+1].begin(); // 因为存的是迭代器，不用默认构造，operator[]更新不会报错
        return val;
    }
};