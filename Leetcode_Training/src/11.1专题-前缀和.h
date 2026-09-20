#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<algorithm>
using namespace std;

// 问题待定：
/*

*/

/*
模板题：
1.标准错位前缀和，s[i] = s[i - 1] + nums[i - 1]，规避边界特判 ：303
2.枚举右，维护左 + 前缀和结合：1749
3.o2做法为枚举右，维护左 + 哈希表 + 前缀和，因为用了哈希表，没有o1快，可以结合s560加深理解：930
4.C++取模运算的理解加深：a % b = a - (a / b) * b，运算结果的符号与a保持一致，以及对负数取模的处理：974
5.o2解法，前缀和 + 有序集合 + 定长滑窗，比较难：3364
6.数学推导 + 前缀和：1685
7.前缀异或，a ^ a = 0, a ^ 0 = a，利用这两个性质，s[r + 1] ^ s[l]即可获得[l, r]区间内的连续异或值：1310
8.二维前缀和，注意二维矩阵的初始化：304
*/

// 前缀和：基础 + 结合哈希表 + 距离和 + 前缀异或和 + 其他一维前缀和 + 二维前缀和

// 【1.1】前缀和基础 (7)
/*
303.区域和检索 - 数组不可变：给定一个整数数组  nums，处理以下类型的多个查询:
计算索引 left 和 right （包含 left 和 right）之间的 nums 元素的 和 ，其中 left <= right
实现 NumArray 类：
NumArray(int[] nums) 使用数组 nums 初始化对象
int sumRange(int i, int j) 返回数组 nums 中索引 left 和 right 之间的元素的 总和 ，
包含 left 和 right 两点（也就是 nums[left] + nums[left + 1] + ... + nums[right] )

3427.变长子数组求和：给你一个长度为 n 的整数数组 nums 。对于 每个 下标 i（0 <= i < n），
定义对应的子数组 nums[start ... i]（start = max(0, i - nums[i])）。
返回为数组中每个下标定义的子数组中所有元素的总和。子数组 是数组中的一个连续、非空 的元素序列。

2559.统计范围内的元音字符串数：给你一个下标从 0 开始的字符串数组 words 以及一个二维整数数组 queries 。
每个查询 queries[i] = [li, ri] 会要求我们统计在 words 中下标在 li 到 ri 范围内（包含 这两个值）
并且以元音开头和结尾的字符串的数目。返回一个整数数组，其中数组的第 i 个元素对应第 i 个查询的答案。
注意：元音字母是 'a'、'e'、'i'、'o' 和 'u' 。

3152.特殊数组 II：如果数组的每一对相邻元素都是两个奇偶性不同的数字，则该数组被认为是一个 特殊数组 。
你有一个整数数组 nums 和一个二维整数矩阵 queries，对于 queries[i] = [fromi, toi]，
请你帮助你检查 子数组 nums[fromi..toi] 是不是一个 特殊数组 。
返回布尔数组 answer，如果 nums[fromi..toi] 是特殊数组，则 answer[i] 为 true ，否则，answer[i] 为 false 。

53.最大子数组和：给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

1749.任意子数组和的绝对值的最大值：给你一个整数数组 nums 。
一个子数组 [numsl, numsl+1, ..., numsr-1, numsr] 的 和的绝对值 为 abs(numsl + numsl+1 + ... + numsr-1 + numsr) 。
请你找出 nums 中 和的绝对值 最大的任意子数组（可能为空），并返回该 最大值 。abs(x) 定义如下：
如果 x 是负整数，那么 abs(x) = -x 。如果 x 是非负整数，那么 abs(x) = x 。

1523.在区间范围内统计奇数数目：给你两个非负整数 low 和 high 。请你返回 low 和 high 之间（包括二者）奇数的数目。
*/
// ---------------------
// 模板题1：标准错位前缀和，s[i] = s[i - 1] + nums[i - 1]，规避边界特判
// 此时s[i]代表左闭右开区间内的元素，s[i]不包含下标i对应的元素，若想取[l, r]内元素，则返回s[r + 1] - s[l]
namespace s303o1
{
    class NumArray {
        vector<int> prefix;
    public:
        NumArray(vector<int>& nums) {
            int n = nums.size();
            prefix.reserve(n + 1);
            prefix[0] = 0;// 这里不写也没问题，因为默认初始化为0
            for (int i = 1; i < n + 1; ++i) {
                prefix[i] = prefix[i - 1] + nums[i - 1];
            }
        }
        int sumRange(int left, int right) {
            return prefix[right + 1] - prefix[left];
        }
    };
}

namespace s3427m1
{
    class Solution {
    public:
        int subarraySum(vector<int>& nums) {
            int ans = 0, n = nums.size();
            vector<int> s(n + 1);
            for (int i = 0; i < n; ++i) {
                s[i + 1] = s[i] + nums[i];// 前缀和初始化的过程可以和更新答案的过程放在同一个循环内
                int l = max(0, i - nums[i]);
                ans += s[i + 1] - s[l];// r = i
            }
            return ans;
        }
    };
}

namespace s2559m1
{
    class Solution {
    public:
        vector<int> vowelStrings(vector<string>& words, vector<vector<int>>& queries) {
            int n = words.size();
            vector<int> s(n + 1);
            unordered_set<char> vowels = { 'a', 'e', 'i', 'o', 'u' };
            for (int i = 0; i < n; ++i) {
                auto it1 = vowels.find(words[i][0]);
                auto it2 = vowels.find(words[i].back());
                s[i + 1] = s[i] + int(it1 != vowels.end() && it2 != vowels.end());
            }
            vector<int> ans;//这里可以不用ranged for loop，提前初始化为n，0,然后用int i = 0...下标更新ans
            // ranged for loop是n次push_back构造（用emplace_back没区别），下标更新是n次赋值
            ans.reserve(queries.size());
            for (auto& q : queries) {
                ans.push_back(s[q[1] + 1] - s[q[0]]);
            }
            return ans;
        }
    };
}

namespace s3152m1
    // 将原数组nums分解成多个特殊子数组
{   // 用数组s储存nums每个下标对应的tag，如果属于相同特殊子数组，那么tag相同
    // 查询时只需判断左右两序号对应tag是否一致
    class Solution {
    public:
        vector<bool> isArraySpecial(vector<int>& nums, vector<vector<int>>& queries) {
            int n = nums.size();
            vector<int> s(n);
            for (int i = 1; i < n; ++i) {
                if ((nums[i] % 2) ^ (nums[i - 1] % 2)) {// 奇偶性不同
                    s[i] = s[i - 1];
                }
                else {
                    s[i] = s[i - 1] + 1;// 改变tag值
                }
            }
            vector<bool> ans;
            ans.reserve(queries.size());
            for (auto& q : queries) {
                ans.push_back(s[q[0]] == s[q[1]]);
            }
            return ans;
        }
    };
}

// 枚举右，维护左 + 前缀和
// o2为动态规划，见6.1专题1.3
namespace s53o1 
{   // 转变为s121，股票的最佳买卖时机
    // 问题变成求当前前缀和 - 最小前缀和的差值的最大值
    // 要求数组至少要有一个元素，那么意味着股票不能当天买当天卖，最小前缀和要最后更新
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int presum = 0;
            int premn = 0;
            int ans = INT_MIN;
            for (int x : nums) {
                presum += x;// 更新当前前缀和
                ans = max(ans, presum - premn);// 更新答案
                premn = min(premn, presum);// 维护最小前缀和
            }
            return ans;
        }
    };
}

// 模板题2：枚举 + 前缀和结合
// 求最大子数组和(s53) + 最小子数字和，两者abs后取最大
// o1为前缀和，灵神解法，更简洁，m1大体思路相同，但逊色一些
namespace s1749o1
{   // 子数组绝对值和为abs(s[j] - s[i])，要让答案最大，那么得找到相差最大的s[i], s[j]
    // 也即最大前缀和s[j]，以及最小前缀和s[i]
    // 如果i < j，那么s[j] - s[i]求的是子数组最大值
    // 如果j < i，那么s[j] - s[i]求的是子数组最小值的绝对值
    // 直接整体考虑，相较于m1省去了不少计算步骤
    class Solution {
    public:
        int maxAbsoluteSum(vector<int>& nums) {
            int preSum = 0, mn = 0, mx = 0;// 注意，这里mn不能初始化为INT_MAX，mx不能初始化为INT_MIN
            // 因为对于全是正数或全是负数的数组，答案是mx - 0或0 - mn，所以需要将mn和mx的初始值设为0
            for (int x : nums) {
                preSum += x;
                mx = max(mx, preSum);
                mn = min(mn, preSum);
            }
            return mx - mn;
        }
    };
}
namespace s1749m1
{
    class Solution {
    public:
        int maxAbsoluteSum(vector<int>& nums) {
            int presum = 0;
            int ans = 0;
            int premx = 0;
            int premn = 0;

            for (int x : nums) {
                presum += x;
                // presum - premx代表子数组的最小和，加个负号就是下式，不需要额外加abs
                // （加了负号还为正数说明最小和都是正的，那答案一定在presum - premn里，不影响答案）
                // presum - premn不加abs的分析同理
                ans = max({ ans, presum - premn, premx - presum });
                premx = max(premx, presum);
                premn = min(premn, presum);
            }

            return ans;
        }
    };
}

// 见专题9.1.2 前缀和 + 二分查找
namespace s2389
{

}

namespace s1523m1
{   // 分类讨论 + 合并，没有用到前缀和思想
    class Solution {
    public:
        int countOdds(int low, int high) {
            if (low % 2 == 0 && high % 2 == 0) {
                return (high - low) / 2;
            }
            else {
                return (high - low) / 2 + 1;
            }
        }
    };
}
namespace s1523o1
{   // 设pre(x)代表[0, x]内的奇数数量，则pre(x) = (x + 1) / 2
    // 那么[x, y]内的奇数数量可以推出为pre[y] - pre[x - 1]
    // 也即 (high + 1) / 2 - (low - 1 + 1) / 2
    class Solution {
    public:
        int countOdds(int low, int high) {
            return (high + 1) / 2 - (low / 2);
        }
    };
}
// ---------------------
// 【1.2】前缀与哈希表 (5)
// 通常要用到维护左，枚举右的技巧
// 跟哈希表结合时，一般维护的是某个前缀和出现的次数，且用闭区间的前缀和比较容易，可以手动cnt[0] = 1；
/*
930.和相同的二元子数组：给你一个二元数组 nums ，和一个整数 goal ，
请你统计并返回有多少个和为 goal 的 非空 子数组。子数组 是数组的一段连续部分。

560.和为 K 的子数组：给你一个整数数组 nums 和一个整数 k ，
请你统计并返回 该数组中和为 k 的子数组的个数 。子数组是数组中元素的连续非空序列。

1524.和为奇数的子数组数目：给你一个整数数组 arr 。请你返回和为 奇数 的子数组数目。
由于答案可能会很大，请你将结果对 10^9 + 7 取余后返回。

974.和可被 K 整除的子数组：给定一个整数数组 nums 和一个整数 k ，
返回其中元素之和可被 k 整除的非空 子数组 的数目。子数组 是数组中 连续 的部分。

3364.最小正和子数组：给你一个整数数组 nums 和 两个 整数 l 和 r。
你的任务是找到一个长度在 l 和 r 之间（包含）且和大于 0 的 子数组 的 最小 和。
返回满足条件的子数组的 最小 和。如果不存在这样的子数组，则返回 -1。
子数组 是数组中的一个连续 非空 元素序列。
*/
// ---------------------
// o1做法为恰好型滑动窗口，转变为越长越合型滑窗
// 模板题3：o2做法为枚举右，维护左 + 哈希表 + 前缀和，因为用了哈希表，没有o1快，可以结合s560加深理解
// 最好还是掌握恰好型滑窗做法（详见8.2专题-不定长滑窗-【2.3.3】恰好型滑动窗口），因为没有利用二元数组的特性，不够快
namespace s930o2
{   /*
    用哈希表记录对应前缀和出现的次数，规避麻烦的下标、边界情况，只需要确保哈希表准确记录次数即可
    只要找某个历史前缀和(非当前前缀和)出现的次数，完全不用管是闭区间的前缀和还是左闭右开的前缀和
    即，寻找满足式：sum - history_sum = goal 的某个历史前缀和出现的次数（包括空数组的前缀和）
    */ 
    class Solution {
    public:
        int numSubarraysWithSum(vector<int>& nums, int goal) {
            int sum = 0;
            unordered_map<int, int> cnt;
            int ans = 0;
            ++cnt[0]; // 空子数组也要计数一次
            for (int num : nums) {// 
                sum += num;
                auto it = cnt.find(sum - goal);
                if (cnt.count(sum - goal)) {
                    ans += cnt[sum - goal];
                }
                // 计算 ans 要在更新 cnt 之前（保证当前 sum 匹配的是之前的所有前缀和）
                ++cnt[sum];
                /*  计算ans和更新cnt的顺序不能调换
                ++cnt[sum]; 
                ans += cnt[sum - goal]; 
                在 goal = 0 时，sum - goal = sum，这是个致命的问题
                因为此时ans计算的是"前缀和等于当前 sum 和 sum - goal 的所有可能的子数组"
                会出现重复计算的情况，比如[1, 1, 0], goal = 0(只要goal为0都会重复计算）
                */
            }
            return ans;
        }
    };
}

// 与930类似，但不能用滑动窗口，因为数组元素有正有负，没有单调性(越长不一定越合法)
namespace s560m1
{
    class Solution {
    public:
        int subarraySum(vector<int>& nums, int k) {
            unordered_map<int, int> cnt;
            int ans = 0, sum = 0;
            cnt[0] = 1;// 显式的加入0比较保险，也符合思维

            for (int x : nums) {
                sum += x;
                if (cnt.count(sum - k)) {
                    ans += cnt[sum - k];
                }
                ++cnt[sum];
            }

            return ans;
        }
    };
}
namespace s560m2
{   // m1写法更标准，有多余时间可以看看m2写法
    class Solution {
    public:
        int subarraySum(vector<int>& nums, int k) {
            int ans = 0, sum = 0;
            unordered_map<int, int> cnt;
            for (int x : nums) {
                cnt[sum]++;//这样写可以不用显式初始化cnt[0]=1，但cnt[total_sum]的频次计算不到
                sum += x;   // 而若k = total_sum时，此时查询对应的是sum - k = 0，仍旧在记录范围内，还是推荐m1写法
                auto it = cnt.find(sum - k);
                if (it != cnt.end()) {
                    ans += it->second;
                }
            }
            return ans;
        }
    };
}

// 套模板
namespace s1524m1
{   // 记录前缀和分别为奇数或偶数的频次
    class Solution {
    public:
        int numOfSubarrays(vector<int>& arr) {
            const int mod = 1e9 + 7;
            int cnt[2]{};
            int sum = 0;
            long long ans = 0;
            cnt[0] = 1;
            for (int x : arr) {
                sum += x;
                ans += cnt[1 - sum % 2];
                ++cnt[sum % 2];
            }
            return ans % mod;
        }
    };
}

// 模板题4：C++取模运算的理解加深：a % b = a - (a / b) * b，运算结果的符号与a保持一致，以及对负数取模的处理
namespace s974m1
{
    // C++: (-3) % 5 = -3;  Python:(-3) % 5 = 2;
    // 想要判定两个数a, b的差值是否能被k整除，等价与判定(a % k + k) % k == (b % k + k) % k，将两个数对k取模的余数都换成正数
    // 比如a = 7, b = -3，k = 5; 
    // a % k = 2, b % k = -3; 
    // (a % k + k) % k = 2, (b % k + k) % k = 2 
    // 但记住，若k为负数，还需要对k取一次绝对值
    class Solution {
    public:
        int subarraysDivByK(vector<int>& nums, int k) {
            vector<int> cnt(k);// 如果用哈希表速度要慢些，但内存占用可能更低
            cnt[0] = 1;
            int ans = 0, sum = 0;
            for (int x : nums) {
                sum += x;
                ans += cnt[(sum % k + k) % k];
                ++cnt[(sum % k + k) % k];
            }
            return ans;
        }
    };
}

// 模板题5：o2解法，前缀和 + 有序集合 + 定长滑窗，比较难
namespace s3364m1
{   // 用multimap记录前缀和与对应下标 + 枚举，类似暴力做法，因为遍历到后面之后，一些超过l, r范围的元素仍然在，是无效的
    class Solution {
    public:
        int minimumSumSubarray(vector<int>& nums, int l, int r) {
            multimap<int, int> mp;
            int ans = INT_MAX, sum = 0, n = nums.size();
            mp.emplace(0, -1);
            for (int i = 0; i < n; ++i) {
                sum += nums[i];
                auto it = mp.lower_bound(sum);
                while (it != mp.begin()) {
                    --it;
                    int len = i - it->second;
                    if (len >= l && len <= r) {
                        ans = min(ans, sum - it->first);
                    }
                }
                mp.emplace(sum, i);
            }
            return ans == INT_MAX ? -1 : ans;
        }
    };
}
namespace s3364o1
{   // 最简单的暴力解法，因为n只到100
    class Solution {
    public:
        int minimumSumSubarray(vector<int>& nums, int l, int r) {
            int ans = INT_MAX;
            int n = nums.size();
            for (int i = 0; i <= n - l; ++i) {
                int sum = 0;
                for (int j = i; j < n && j - i + 1 <= r; ++j) {
                    sum += nums[j];
                    if (sum > 0 && j - i + 1 >= l) {
                        ans = min(ans, sum);
                    }
                }
            }
            return ans == INT_MAX ? -1 : ans;
        }
    };
}
namespace s3364o2
{
    class Solution {
    public:
        int minimumSumSubarray(vector<int>& nums, int l, int r) {
            int ans = INT_MAX, n = nums.size();
            vector<int> s(n + 1);
            multiset<int> s_set;
            for (int j = 1; j <= n; j++) {
                s[j] = s[j - 1] + nums[j - 1];// 初始化前缀和数组
                if (j < l) {
                    continue;// 第一个窗口右端点还未出现，窗口在枚举j的左边，右端点为j - l,左端点为j - r
                }
                s_set.insert(s[j - l]);// 随着j的增大，逐渐将满足条件的元素加入窗口
                auto it = s_set.lower_bound(s[j]);
                if (it != s_set.begin()) {
                    ans = min(ans, s[j] - *--it);// 前期窗口虽然未完全建立，但也可以进行查询，没有关系
                }
                if (j >= r) {
                    s_set.erase(s_set.find(s[j - r]));// 窗口长度已满，将窗口左端点元素移出窗口
                }
            }
            return ans == INT_MAX ? -1 : ans;
        }
    };
}
// ---------------------
// 【1.3】距离和(1) 
/*
1685.有序数组中差绝对值之和：给你一个 非递减 有序整数数组 nums 。
请你建立并返回一个整数数组 result，它跟 nums 长度相同，且result[i] 等于 nums[i] 与数组中所有其他元素差的绝对值之和。
换句话说， result[i] 等于 sum(|nums[i]-nums[j]|) ，其中 0 <= j < nums.length 且 j != i （下标从 0 开始）。
*/
// ---------------------
// 模板题6：数学推导 + 前缀和
namespace s1685m1
{   // ans[i] = (ai - a0) + (ai - a1) + (ai - a2) + ... + (ai - ai-1)
    //          + (ai+1 - ai) + (ai+2 -ai) + ... + (an-1 - ai)
    // 合并后为：i * ai - s[i]   +    (i - n + 1) * ai + s[n] - s[i + 1]
    class Solution {
    public:
        vector<int> getSumAbsoluteDifferences(vector<int>& nums) {
            int n = nums.size();
            vector<int> s(n + 1);
            vector<int> ans(n);
            for (int i = 0; i < n; ++i) {
                s[i + 1] = s[i] + nums[i];
            }
            for (int i = 0; i < n; ++i) {
                ans[i] = (2 * i - n + 1) * nums[i] + s[n] - s[i + 1] - s[i];
            }
            return ans;
        }
    };
}
// ---------------------
// 【1.4】前缀异或和(0)
/*

*/
// ---------------------

// ---------------------
// 【1.5】其他一维前缀和(2)
/*
1310.子数组异或查询：有一个正整数数组 arr，现给你一个对应的查询数组 queries，其中 queries[i] = [Li, Ri]。
对于每个查询 i，请你计算从 Li 到 Ri 的 XOR 值（即 arr[Li] xor arr[Li+1] xor ... xor arr[Ri]）作为本次查询的结果。
并返回一个包含给定查询 queries 所有结果的数组。
*/
// ---------------------
// 模板题7：前缀异或，a ^ a = 0, a ^ 0 = a，利用这两个性质，s[r + 1] ^ s[l]即可获得[l, r]区间内的连续异或值
namespace s1310o1
{
    class Solution {
    public:
        vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
            int n = arr.size();
            vector<int> s(n + 1);
            for (int i = 0; i < n; ++i) {// s[0] = 0是没问题的，因为a ^ 0 = a，不影响结果
                s[i + 1] = s[i] ^ arr[i];
            }
            int m = queries.size();
            vector<int> ans(m);
            for (int i = 0; i < m; ++i) {
                auto& q = queries[i];
                ans[i] = s[q[1] + 1] ^ s[q[0]];
            }
            return ans;
        }
    };
}

// 见枚举技巧-枚举中间11.0.2，元素频次的前缀和
namespace s1534o1
{

}
// ---------------------
// 【1.6】二维前缀和(2)
/*
304.二维区域和检索 - 矩阵不可变：给定一个二维矩阵 matrix，以下类型的多个请求：
计算其子矩形范围内元素的总和，该子矩阵的 左上角 为 (row1, col1) ，右下角 为 (row2, col2) 。
实现 NumMatrix 类：NumMatrix(int[][] matrix) 给定整数矩阵 matrix 进行初始化
int sumRegion(int row1, int col1, int row2, int col2) 返回 左上角 (row1, col1) 、
右下角 (row2, col2) 所描述的子矩阵的元素 总和 。

1314.矩阵区域和：给你一个 m x n 的矩阵 mat 和一个整数 k ，请你返回一个矩阵 answer ，
其中每个 answer[i][j] 是所有满足下述条件的元素 mat[r][c] 的和： 
i - k <= r <= i + k, j - k <= c <= j + k 且 (r, c) 在矩阵内。
*/
// ---------------------
// 模板题8：二维前缀和，注意二维矩阵的初始化
namespace s304o1
{
    class NumMatrix {
    private:
        vector<vector<int>> sMatrix;
    public:
        NumMatrix(vector<vector<int>>& matrix) {
            int m = matrix.size();
            int n = matrix[0].size();
            sMatrix.resize(m + 1, vector<int>(n + 1));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    sMatrix[i + 1][j + 1] = sMatrix[i + 1][j] +
                        sMatrix[i][j + 1] - sMatrix[i][j] + matrix[i][j];
                }
            }
        }

        int sumRegion(int row1, int col1, int row2, int col2) {
            return sMatrix[row2 + 1][col2 + 1] - sMatrix[row2 + 1][col1]
                - sMatrix[row1][col2 + 1] + sMatrix[row1][col1];
        }
    };
}

// 套模板，本质和s304一样
namespace s1510m1
{
    class Solution {
    public:
        vector<vector<int>> matrixBlockSum(vector<vector<int>>& mat, int k) {
            int m = mat.size(), n = mat[0].size();
            vector<vector<int>> sum(m + 1, vector<int>(n + 1));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    sum[i + 1][j + 1] = sum[i][j + 1] + sum[i + 1][j]
                        - sum[i][j] + mat[i][j];
                }
            }
            vector<vector<int>> ans(m, vector<int>(n));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    int row1 = max(i - k, 0);
                    int col1 = max(j - k, 0);
                    int row2 = min(i + k, m - 1);
                    int col2 = min(j + k, n - 1);
                    ans[i][j] = sum[row2 + 1][col2 + 1] - sum[row2 + 1][col1]
                        - sum[row1][col2 + 1] + sum[row1][col1];
                }
            }
            return ans;
        }
    };
}