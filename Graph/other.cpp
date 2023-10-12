#include "myheader.h"


// 531 medium m x n 的图像由黑白像素组成。如果'B' 所在的同一行和同一列不存在其他'B'，就叫黑色孤独像素。
// 返回黑色孤独像素的个数。
// 核心是先全部遍历一遍，做预处理
int findLonelyPixel(vector<vector<char>>& picture) {
    int n = picture.size(), m = picture[0].size();
    vector<int> row(n, 0); // 每行的B数量
    vector<int> col(m, 0); // 每列的B数量
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (picture[i][j] == 'B') {
                ++row[i];
                ++col[j];
            }
        }
    }
    int ret = 0;
    for (int i = 0; i < n; ++i) {
        if (row[i] == 1) { // 该行只有一个B
            for (int j = 0; j < m; ++j) {
                if (picture[i][j] == 'B' && col[j] == 1) { // 该行的这个B的列也只有这一个
                    ++ret;
                    break;
                }
            }
        }
    }
    return ret;
}