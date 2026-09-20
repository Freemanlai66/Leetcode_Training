#pragma once
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 问题待定：
/*
sxxx：xxx
*/

/*
模板题：
*/

// 三指针

// 【5.1】三指针 (1)
/*
2563.统计公平数对的数目：给你一个下标从 0 开始、长度为 n 的整数数组 nums ，
和两个整数 lower 和 upper ，返回 公平数对的数目 。
如果 (i, j) 数对满足以下情况，则认为它是一个 公平数对 ：
0 <= i < j < n，且
lower <= nums[i] + nums[j] <= upper
*/
// ---------------------
// 其他做法见8.3.1专题相向双指针
namespace s2563o22
{   /* 枚举左边的 i，统计右边有多少个合法的 j
    枚举 i, 计算满足 j > i 且 nums[j] <= upper - nums[i] 的 j 的个数，记作 count(upper)。
    枚举 i, 计算满足 j > i 且 nums[j] < lower - nums[i]，也即nums[j] <= lower - 1 - nums[i]的j的个数
    记作count(lower - 1)，答案就是 count(upper) - count(lower - 1)。
    计算count(upper)的过程是O(n)的，注意，当i == j时要直接退出循环
    此时意味着对当前i，在右边找不到一个满足nums[j] <= upper - nums[i]的j
    而随着继续循环num[i]的增大，这个不等式将更加无法满足，而且如果继续循环j - i将变成负数
    */
    class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
            sort(nums.begin(), nums.end());
            auto count = [&](int upper) {
                long long res = 0;
                int j = nums.size() - 1;
                for (int i = 0; i < nums.size(); i++) {
                    while (j > i && nums[j] > upper - nums[i]) {
                        j--;
                    }
                    if (i == j) break;
                    res += j - i;
                }
                return res;// count函数的范围值是对每个i的的遍历nums[j] <= upper - nums[i]的总和
                };
            return count(upper) - count(lower - 1);// 将多个减法拆分成多个加法和一次减法
        }
    };
}
