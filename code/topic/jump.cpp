#include "myheader.h"

// 45 medium 跳跃游戏 给你一个非负整数数组 nums，你最初位于数组的第一个位置。
// 数组中的每个元素代表你在该位置可以跳跃的最大长度（也可以不跳最大长度，跳近一点）。
// 目标是使用最少的跳跃次数到达数组的最后一个位置。（假设总是可以到达数组的最后一个位置）
int jump(vector<int>& nums) {
    if (nums.size() == 1) return 0;
    int rmax = 0; // 每次能达到的最远位置（不一定会跳）
    int cur = 0; // 每次选择，都要跳到最远位置（贪心）
    int cnt = 0;
    for (int i = 0; i < nums.size() - 1; i++) { // 遍历到终点前的一个位置
        rmax = max(rmax, i + nums[i]); // [0:i]能达到的最远距离
        // 2 100 1，当遍历到100时，此前2的位置跳到1，但100位置可以直接跳到终点
        // 而之前只可能跳到当前遍历处或者更后面的位置（因为存在假设，所以之前的位置不可能在当前遍历处之前）
        // 所以假设之前跳到了当前位置，现在还需要再跳一次到终点
        if (rmax >= nums.size()-1) return cnt + 1;
        if (i == cur) {
            cur = rmax; // 再次贪心，跳到最远位置
            cnt++;
        }
        // 这样贪心，可以保证只要中途到了终点，一定能保证条约次数最少
    }
    return -1; // 不会运行到这
}
// 55 medium 同45非负，判断你是否能够到达最后一个位置。（45默认一定能）
bool canJump(vector<int>& nums) {
    if (nums.size() == 1) return true;
    int rmax = 0, cur = 0;
    for (int i = 0; i < nums.size()-1; ++i) {
        rmax = max(rmax, nums[i]+i);
        if (rmax >= nums.size()-1) return true;
        if (cur == i) {
            if (nums[cur] == 0 && cur == rmax) return false; // 达不到只有一种情况，就是困在0处且最远也只能到这儿了。两个条件必不可少
            cur = rmax;
        }
    }
    return false; // 不会运行到这
}
// 1306 medium 依旧是跳跃问题，45 55的变体
// arr非负，即有可能存在若干个0的数，arr[i]说明当前可以跳的位置{i-arr[i],i+arr[i]}
// 给一个起始点，判断是否可以到达任意是0的位置，不能跳出边界。
// 明显的DFS
bool canReach(vector<int>& arr, int start) {
    if (arr[start] == 0) return true;
    return dfs(arr, start);
}
// 选择列表有2，向前向后
// 同时，为了避免重复访问（这题特殊之处），将边界条件设置成重复就终止
bool dfs(vector<int>& arr, int pos) {
    if (arr[pos] < 0) return false;
    arr[pos] = -arr[pos];  // 这个节点访问过了
    bool ret = (arr[pos] == 0);
    for (int i = 0; i < 2; i++) {
        int newPos = pos + (2*i-1)*arr[pos];
        if (newPos >= 0 && newPos < arr.size()) {
            ret = ret || dfs(arr, newPos);  // 注意这里不需要做把选择列表取出和送回
            // 因为这道题没有路径之说，只需要遍历完所有节点就行，所以时间复杂度为On
        }
    }
    return ret;
}