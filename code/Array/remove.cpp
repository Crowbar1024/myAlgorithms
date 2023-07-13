#include "myheader.h"

// 可以发现一个共同点，都是将所需要的元素提到原数组的前头来



// 27 删除数组里面所有为val的数，并返回删除后的数组长度，要求空间复杂度O1，无所谓新数组里的顺序
// 这是一种边统计边更新的操作，比较少见，但很有效
int removeElement(vector<int>& nums, int val) {
    int len = nums.size(), cnt = 0;
    for (int i = 0; i < len; i++) {
        if (nums[i] == val) cnt++;
        else nums[i - cnt] = nums[i];
    }
    nums.resize(len - cnt);
    return len - cnt;
}

// 26 27变体 unique非降序序列，并返回最后的长度。一摸一样
int removeDuplicates(vector<int>& nums) {
    int len = nums.size(), cnt = 0;
    for (int i = 1; i < len; i++) {
        if (nums[i] == nums[i-1]) cnt++;
        else nums[i - cnt] = nums[i];
    }
    nums.resize(len - cnt);
    return len - cnt;
}

// 283 27变体 把数组中所有的0搬到后面，剩下元素的顺序不变
void moveZeroes(vector<int>& nums) {
    int len = nums.size(), cnt = 0;
    for (int i = 1; i < len; i++) {
        if (nums[i] == 0) cnt++;
        else nums[i - cnt] = nums[i];
    }
}

// 80 medium 非降序，重复的删掉只剩两个
int removeDuplicates(vector<int>& nums) {
    int len = nums.size(), cnt = 0, ret = 0;
    for (int i = 1; i < len; i++) {
        if (nums[i]==nums[i-1] && cnt>0) {
            cnt += 1;
            continue;
        } else if (nums[i]==nums[i-1] && cnt==0) {
            cnt += 1;
            nums[i-ret] = nums[i];
        } else {
            if (cnt > 1) ret += cnt-1;
            nums[i-ret] = nums[i];
            cnt = 0;
        }
    }
    if (cnt > 1) ret += cnt-1;  // 最后是连着的，消不了
    return len-ret;
}


// 443 medium 字符串压缩 ["a","b","b","b","b","b","b","b","b","b","b","b","b"] -> ["a","b","1","2"]
// 压缩后得到的字符串 s 不应该直接返回 ，需要转储到字符数组 chars 中。需要注意的是，如果组长度为 10 或 10 以上，则在 chars 数组中会被拆分为多个字符。
// 返回该数组的新长度。你必须设计并实现一个只使用常量额外空间的算法来解决此问题。
// 增加一个哨兵，避免{aab} {abb}这种情况的纷乱思考
int compress(vector<char>& chars) {
    int cnt = 1, cur = 1;
    chars.emplace_back(' ');  // 有特殊符号，但空格没用过
    int n = chars.size();
    for (int i = 1; i < n; ++i) {
        if (chars[i] == chars[i-1]) ++cnt;
        else {
            if (cnt == 1) chars[cur++] = chars[i];  // 直接写下一个字母
            else {
                string digit = to_string(cnt);
                for (int j = 0; j < digit.size(); ++j) chars[cur++] = digit[j];
                chars[cur++] = chars[i];  // 最后一个是追加的
                cnt = 1;
            }
        }
    }
    return cur-1;
}