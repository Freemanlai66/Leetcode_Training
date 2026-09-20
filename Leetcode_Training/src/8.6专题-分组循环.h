#pragma once
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 问题待定：
/*
*/

/*
模板题：
1.分而治之，分组循环成熟模板：2414
2.山脉类型分组循环题的统一模板：135
*/

// 分组循环
/*
题目一般为：寻找一个满足特定条件的最长、最短的子数组，每个子数组之间一定不能出现【重叠】！
同时每一组的判断逻辑都是相同的
外层循环负责遍历组之前的准备工作（记录开始位置），和遍历组之后的统计工作（更新答案最大值）。
内层循环负责遍历组，找出这一组最远在哪结束。
这个写法的好处是，各个逻辑块分工明确，也不需要特判最后一组（易错点）
*/

// 【6.1】分组循环 (11)
/*
2760.最长奇偶子数组：给你一个下标从 0 开始的整数数组 nums 和一个整数 threshold 。
请你从 nums 的子数组中找出以下标 l 开头、下标 r 结尾 (0 <= l <= r < nums.length) 且满足以下条件的 最长子数组 ：
nums[l] % 2 == 0，对于范围 [l, r - 1] 内的所有下标 i ，nums[i] % 2 != nums[i + 1] % 2
对于范围 [l, r] 内的所有下标 i ，nums[i] <= threshold，以整数形式返回满足题目要求的最长子数组的长度。


*/
// ---------------------
namespace s2760m1
{
    class Solution {
    public:
        int longestAlternatingSubarray(vector<int>& nums, int threshold) {
            // 1.最左端为偶数 2.奇偶相间 3.每个都小于等于threshold 4.找最长子数组
            int n = nums.size(), ans = 0;
            for (int left = 0; left < n; ++left) {
                if (nums[left] % 2 != 0 || nums[left] > threshold) {
                    continue;
                }
                int right = left + 1;
                while (right < n) {
                    if (nums[right] <= threshold && (nums[right] % 2 != nums[right - 1] % 2)) {
                        ++right;
                    }
                    else {
                        break;
                    }
                }
                ans = max(ans, right - left);
                left = right - 1;// 之后自动++，新for循环内left == 之前循环中的right
            }
            return ans;
        }
    };
}

namespace s1446m1
{
    class Solution {
    public:
        int maxPower(string s) {
            int n = s.size(), ans = 1;
            int i = 1;
            while (i < n) {
                int start = i - 1;
                while (s[i] == s[i - 1]) {
                    ++i;
                }
                ans = max(ans, i - start);
                ++i;
            }
            return ans;
        }
    };
}

namespace s1869m1
{
    class Solution {
    public:
        bool checkZeroOnes(string s) {
            int oneLen = 0, zeroLen = 0;
            int left = 0, n = s.size();
            while (left < n) {
                if (s[left] == '0') {
                    int right = left + 1;
                    while (right < n && s[right] == '0') {
                        ++right;
                    }
                    zeroLen = max(zeroLen, right - left);
                    left = right;
                }
                else {
                    int right = left + 1;
                    while (right < n&& s[right] == '1') {
                        ++right;
                    }
                    oneLen = max(oneLen, right - left);
                    left = right;
                }
            }
            return oneLen > zeroLen;
        }
    };
}

// 模板题1：分而治之，分组循环成熟模板
namespace s2414m1
{
    class Solution {
    public:
        int longestContinuousSubstring(string s) {
            int ans = 0, n = s.size();
            int left = 0;
            while (left < n) {
                int right = left + 1;// 定义right = left + 1，这样更新答案时区间长度也至少有1
                while (right < n && s[right] - s[right - 1] == 1) {// 注意right < n
                    ++right;
                }
                ans = max(ans, right - left);// 更新答案
                left = right;// 此时的rigth一定是不满足当前区间条件的，可以作为下个区间的开头
            }
            return ans;
        }
    };
}

// 完全套模板
namespace s674m1
{
    class Solution {
    public:
        int findLengthOfLCIS(vector<int>& nums) {
            int ans = 0, left = 0, n = nums.size();
            while (left < n) {
                int right = left + 1;
                while (right < n && nums[right] > nums[right - 1]) {
                    ++right;
                }
                ans = max(ans, right - left);
                left = right;
            }
            return ans;
        }
    };
}

// m1为终极垃圾写法，优化后为m2，模板进阶题，当需要连续限定3个元素时，使判断条件为left < n - 1和right < n - 1有奇效
// 同时需要注意在进入while循环前，先用continue清理掉一些情况
namespace s978m1
{
    class Solution {
    public:
        int maxTurbulenceSize(vector<int>& arr) {
            int ans = 0, n = arr.size(), left = 0;
            if (n == 1)
                return 1;
            while (left < n) {
                int right = left + 1;
                while (right < n) {
                    if (arr[right] > arr[right - 1]) {
                        if (right + 1 < n && arr[right + 1] < arr[right]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }
                    else if (arr[right] < arr[right - 1]) {
                        if (right + 1 < n && arr[right] < arr[right + 1]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        ++left;
                        break;
                    }
                }
                if (right == n) {
                    ans = max(ans, right - left);
                }
                else {
                    ans = max(ans, right - left + 1);
                }
                left = right;
            }
            return ans;
        }
    };
}
namespace s978m2
{
    class Solution {
    public:
        int maxTurbulenceSize(vector<int>& arr) {
            int ans = 1, n = arr.size(), left = 0;
            while (left < n - 1) {
                int right = left + 1;
                if (arr[left] == arr[right]) {
                    ++left;
                    continue;
                }
                while (right < n - 1) {
                    if (arr[right] > arr[right - 1]) {
                        if (arr[right + 1] < arr[right]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }
                    else if (arr[right] < arr[right - 1]) {
                        if (arr[right] < arr[right + 1]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }
                }// 退出while循环时，right一定是湍流数组的一部分，湍流数组为[left, right]
                ans = max(ans, right - left + 1);
                left = right;
            }
            return ans;
        }
    };
}

namespace s228m1
{
    class Solution {
    public:
        vector<string> summaryRanges(vector<int>& nums) {
            vector<string> ans;
            int n = nums.size(), left = 0;
            while (left < n) {
                int right = left + 1;
                while (right < n) {
                    if (nums[right] == nums[right - 1] + 1) {// 这里防止-2^31 - 1溢出，需要移项
                        ++right;
                    }
                    else {
                        break;
                    }
                }
                if (right - left == 1) {
                    ans.push_back(to_string(nums[left]));
                }
                else {
                    string temp = "";
                    temp += to_string(nums[left]);
                    temp += "->";
                    temp += to_string(nums[right - 1]);
                    ans.push_back(temp);
                }
                left = right;
            }
            return ans;
        }
    };
}

// 综合进阶题
namespace s845m1
{
    class Solution {
    public:
        int longestMountain(vector<int>& arr) {
            int left = 0, n = arr.size(), ans = 0;
            while (left < n - 1) {
                int right = left + 1;
                if (arr[right] <= arr[left]) {
                    ++left;
                    continue;
                }
                // 找到山峰后的下一个元素
                while (right < n && arr[right - 1] < arr[right]) {
                    ++right;
                }
                // 如果山峰是数组末端 或 山峰下一个元素和山峰平齐
                if (right == n || arr[right] == arr[right - 1]) {
                    left = right;
                    continue;
                }
                // 找到山谷最底处
                while (right < n - 1 && arr[right + 1] < arr[right]) {
                    ++right;
                }
                ans = max(ans, right - left + 1);
                left = right;
            }
            return ans;
        }
    };
}

// 相当于s845山脉题的超级进化
// m1方法要点在于分成上坡下坡两种讨论，同时确保只计算[left, right]闭区间内的元素
// 同时不要在求下坡时左闭右开，求上坡时又左闭右闭，必须统一
// 模板题2：o1方法可以看作山脉类型题的统一模板，简洁易懂，值得学习
namespace s135m1
{
    class Solution {
    public:
        int candy(vector<int>& ratings) {
            int left = 0, ans = 0;
            int n = ratings.size();
            int preHeight = 0;// 用previous Height记录前一个坡段的高度，应对山峰类型/\情况
            // 比如前一个上坡段高度为6，紧接着的下坡段为3，山顶的元素需要给6糖果，而不是按当前下坡段得到的高度3给糖果
            while (left < n - 1) {
                int right = left + 1;
                if (ratings[right] == ratings[left]) {
                    ++ans;
                    ++left;
                    continue;
                }
                if (ratings[right] > ratings[left]) {// 找上坡
                    while (right < n - 1) {
                        if (ratings[right + 1] > ratings[right]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }// 此时[left, right]范围内全是上坡元素
                    int height = right - left + 1;
                    ans += (height + 1) * height / 2;// 等差数列求和
                    preHeight = height;
                    if (right < n - 1 && ratings[right + 1] < ratings[right]) {
                        left = right;// 如果紧接着也是一个坡段，那么让当前坡段的一个元素给下一个坡段用
                    }               
                    else {
                        left = right + 1;// 如果当前坡段后面的元素是平的（相等），直接移过去
                    }
                }
                else { // 找下坡
                    while (right < n - 1) {
                        if (ratings[right + 1] < ratings[right]) {
                            ++right;
                        }
                        else {
                            break;
                        }
                    }// 此时[left, right]范围内全是下坡元素
                    int height = right - left + 1;
                    ans += (height + 1) * height / 2;
                    if (left > 0 && ratings[left - 1] < ratings[left]) {
                        ans += max(height, preHeight) - height - preHeight;
                    }// 上坡段让了一个给下坡，那么山顶元素就会被计算两遍，只要最大的
                    preHeight = height;
                    if (right < n - 1 && ratings[right + 1] > ratings[right]) {
                        --ans;
                        left = right;// 如果紧接着也是一个坡段，那么让当前坡段的一个元素给下一个坡段用
                    }// 下坡段让了一个给上坡，那么山脚元素就会被计算两遍，只需ans - 1即可
                    else {
                        left = right + 1;
                    }
                }
            }
            return left == n - 1 ? ans + 1 : ans;// 注意：当最后元素同时不属于上坡或下坡时，会遍历不到（包括n == 1的情况）
        }
    };
}
namespace s135o1
{   // m1中考虑到可能从第一个元素开始下坡，或是最后一个元素是山顶，将找“山脉”拆分成了找“上坡/下坡”
    // o1则延续了找“山脉”的思路，但用inc和dec两个变量记录上坡段和下坡段的长度（不包括山顶）
    // 如此一来，如果是从第一个元素开始下坡，那么inc=0，同理如果最后一个元素是山顶，那么dec=0
    // 另一方面，山顶元素也不会被重复加两遍，只需要比较inc和dec的大小即可，不像m1中需要用preHeight来记录
    // 另一个技巧是预先给所有孩子先发一个糖果，这样连续两个山脉的固底元素就算被重复加两次也不会影响答案（因为是0）
    // 同时用i替代left，省去每次循环后的赋值，如left = ...
    class Solution {
    public:
        int candy(vector<int>& ratings) {
            int n = ratings.size();
            int ans = n; // 先给每人分一个
            for (int i = 0; i < n; i++) {
                int start = i > 0 && ratings[i - 1] < ratings[i] ? i - 1 : i;

                // 找严格递增段
                while (i + 1 < n && ratings[i] < ratings[i + 1]) {
                    i++;
                }
                int top = i; // 峰顶

                // 找严格递减段
                while (i + 1 < n && ratings[i] > ratings[i + 1]) {
                    i++;
                }

                int inc = top - start; // start 到 top 严格递增
                int dec = i - top;     // top 到 i 严格递减
                ans += (inc * (inc - 1) + dec * (dec - 1)) / 2 + max(inc, dec);
            }
            return ans;
        }
    };

}

// 借鉴s135o1的思路，上坡下坡段可以为0，将每段区间内的元素都当成山脉处理
namespace s838m1
{
    class Solution {
    public:
        string pushDominoes(string dominoes) {
            int n = dominoes.size();
            for (int i = 0; i < n;) {
                int start = i;
                while (i < n && dominoes[i] == 'R') {
                    ++i;
                }
                int l = i;
                while (i < n && dominoes[i] == '.') {
                    ++i;
                }
                int r = i;
                while (i < n && dominoes[i] == 'L') {
                    ++i;
                }
                int inc = l - start;
                int dec = i - r;
                if (inc == 0 && dec == 0) continue;
                if (inc == 0) {
                    for (int card = start; card < r; ++card) {
                        dominoes[card] = 'L';
                    }
                    // fill(dominoes.begin() + start, dominoes.begin() + r, 'L');// 可替换为fill
                }
                else if (dec == 0) {
                    for (int card = l; card < i; ++card) {
                        dominoes[card] = 'R';
                    }
                    // fill(dominoes.begin() + l, dominoes.begin() + i, 'R');
                }
                else {
                    int left = l;
                    int right = r - 1;
                    while (left < right) {
                        dominoes[left] = 'R';
                        dominoes[right] = 'L';
                        ++left; --right;
                    }
                }
            }
            return dominoes;
        }
    };
}

namespace s413m1
{
    class Solution {
    public:
        int numberOfArithmeticSlices(vector<int>& nums) {
            int ans = 0, n = nums.size(), left = 0;
            while (left < n - 2) {// 最少要3个元素
                int right = left + 1;
                int diff = nums[right] - nums[left];
                while (right < n && nums[right] - nums[right - 1] == diff) {
                    ++right;
                }
                int len = right - left;
                if (len >= 3) {
                    for (int i = 3; i <= len; ++i) {
                        ans += len - i + 1;
                    }// 这里用可以计算归纳式，ans += (len - 1) * (len - 2) / 2;
                }
                left = right - 1;// 下一个等差区间可能包含上一个区间的尾巴
            }
            return ans;
        }
    };
}