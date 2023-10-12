#include "myheader.h"

/*
    置换选择排序算法
    大文件分割成小文件，如果内存较小，该算法可以生成数学期望为内存大小两倍的顺串
    扫雪机模型，要求大文件数据分布均匀
    每次输出一个，就要读入一个
*/ 

class Replace_Selection_Sort {
private:
    vector<int> tr; // 内存数组，前半部分[0:m-1]为完全二叉树，默认小顶堆，后半部分[m,n-1]为remain数组，存放更小的值
    int n; // 内存大小
    int m; // 堆的大小
    vector<vector<int>> ret; // 最终生产的顺串，每个长度可能不同
    void init(vector<int>& bigFile, int n) {
        tr.clear();
        ret.clear();
        this->n = n;
        this->m = n;
        tr.resize(n);
        for (int i = 0; i < n; ++i) tr[i] = bigFile[i]; // 先读入内存
        build();
        int id = 0; // 顺串的序号
        for (int i = n+1; i < bigFile.size(); ++i) {
            int val = bigFile[i];
            if (!m) { // 堆没了
                ++id; // 一个顺串写完了，开始下一个顺串
                m = n; // 把remain部分的建堆
                build();
            }
            ret[id].emplace_back(tr[0]);
            if (val > tr[0]) { // 大的就放入堆
                tr[0] = val;
                swap(tr[0], tr[m-1]);
                heapify(0);
            } else { // 小的就放到remain数组
                tr[0] = tr[m-1];
                tr[m-1] = val;
                --m; // 堆变小
                heapify(0);
            }
        }
    }
    inline void build() { // 建堆
        for (int i = n/2-1; i >= 0; --i) heapify(i);
    }
    void heapify(int i) {
        int l = i<<1|1;
        while (l < m) {
            int largest = (l+1<m && tr[l+1]>tr[l]) ? l+1 : l;
            largest = tr[largest]>tr[i] ? largest : i;
            if (largest == i) break;
            swap(tr[i], tr[largest]);
            i = largest;
            l = i<<1|1;
        }
    }
public:
    vector<vector<int>> getSmallFiles(vector<int>& bigFile, int n) {
        init(bigFile, n);
        return ret;
    }
};


