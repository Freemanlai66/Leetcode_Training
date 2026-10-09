#pragma once
#include<string>
#include<vector>
#include<set>
#include<map>
#include<stack>
#include<unordered_map>
#include<unordered_set>
#include<algorithm> // max, minmax
#include<numeric> // gcd(最大公约数)
#include<intrin.h> // __popcnt
using namespace std;

// 问题待定：
/*
s456o2, o3：单调栈做法
*/

/*
模板题：
1.两数之和，用哈希表在O(1)时间内获取O(n)信息，同时枚举右，寻找左：1
2.对与两数之和进行复杂化，比如取模等：1010
3.掌握o2解法可以对枚举的变量更新规则融会贯通！：2874
4.前后缀数组 + 哈希表撤销，体会枚举中间的优势：3583
*/

// 常用枚举技巧：枚举右，维护左 + 枚举中间

// 【0.1.1】枚举右，维护左-基础 (17)
// 对于双变量问题，比如两数之和a + b = c，可以枚举右边的b，转换成单变量问题
// 也即在左边查找是否有a = c - b，这可以用哈希表来维护
/*
1.两数之和：给定一个整数数组 nums 和一个整数目标值 target。
请你在该数组中找出 和为目标值 target  的那 两个 整数，并返回它们的数组下标。
你可以假设每种输入只会对应一个答案，并且你不能使用两次相同的元素。你可以按任意顺序返回答案。

2441.与对应负数同时存在的最大正整数：给你一个 不包含 任何零的整数数组 nums ，
找出自身与对应的负数都在数组中存在的最大正整数 k 。
返回正整数 k ，如果不存在这样的整数，返回 -1 。

1512.好数对的数目：给你一个整数数组 nums 。
如果一组数字 (i,j) 满足 nums[i] == nums[j] 且 i < j ，就可以认为这是一组 好数对 。返回好数对的数目。

2001.可互换矩形的组数：用一个下标从 0 开始的二维整数数组 rectangles 来表示 n 个矩形，
其中 rectangles[i] = [widthi, heighti] 表示第 i 个矩形的宽度和高度。
如果两个矩形 i 和 j（i < j）的宽高比相同，则认为这两个矩形 可互换 。
更规范的说法是，两个矩形满足 widthi/heighti == widthj/heightj（使用实数除法而非整数除法），则认为这两个矩形 可互换 。
计算并返回 rectangles 中有多少对 可互换 矩形。

1128.等价多米诺骨牌对的数量：给你一组多米诺骨牌 dominoes 。
形式上，dominoes[i] = [a, b] 与 dominoes[j] = [c, d] 等价 
当且仅当 (a == c 且 b == d) 或者 (a == d 且 b == c) 。即一张骨牌可以通过旋转 0 度或 180 度得到另一张多米诺骨牌。
在 0 <= i < j < dominoes.length 的前提下，找出满足 dominoes[i] 和 dominoes[j] 等价的骨牌对 (i, j) 的数量。

121.买卖股票的最佳时机：给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。
你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。设计一个算法来计算你所能获取的最大利润。
返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。

2016.增量元素之间的最大差值：给你一个下标从 0 开始的整数数组 nums ，该数组的大小为 n ，
请你计算 nums[j] - nums[i] 能求得的 最大差值 ，其中 0 <= i < j < n 且 nums[i] < nums[j] 。
返回 最大差值 。如果不存在满足要求的 i 和 j ，返回 -1 。

219.存在重复元素II：给你一个整数数组 nums 和一个整数 k ，判断数组中是否存在两个 不同的索引 i 和 j ，
满足 nums[i] == nums[j] 且 abs(i - j) <= k 。如果存在，返回 true ；否则，返回 false 。

2260.必须拿起的最小连续卡牌数：给你一个整数数组 cards ，其中 cards[i] 表示第 i 张卡牌的 值 。
如果两张卡牌的值相同，则认为这一对卡牌 匹配 。
返回你必须拿起的最小连续卡牌数，以使在拿起的卡牌中有一对匹配的卡牌。如果无法得到一对匹配的卡牌，返回 -1 。

2815.数组中的最大数对和：给你一个下标从 0 开始的整数数组 nums 。
请你从 nums 中找出和 最大 的一对数，且这两个数数位上最大的数字相等。
返回最大和，如果不存在满足题意的数字对，返回 -1 。

2342.数位和相等数对的最大和：给你一个下标从 0 开始的数组 nums ，数组中的元素都是 正 整数。
请你选出两个下标 i 和 j（i != j），且 nums[i] 的数位和 与  nums[j] 的数位和相等。
请你找出所有满足条件的下标 i 和 j ，找出并返回 nums[i] + nums[j] 可以得到的 最大值。如果不存在这样的下标对，返回 -1。

1679.K 和数对的最大数目：给你一个整数数组 nums 和一个整数 k 。
每一步操作中，你需要从数组中选出和为 k 的两个整数，并将它们移出数组。
返回你可以对数组执行的最大操作数。

面试题16.24.数对和：设计一个算法，找出数组中两数之和为指定值的所有整数对。一个数只能属于一个数对。

3623.统计梯形的数目 I：给你一个二维整数数组 points，其中 points[i] = [xi, yi] 表示第 i 个点在笛卡尔平面上的坐标。
水平梯形 是一种凸四边形，具有 至少一对 水平边（即平行于 x 轴的边）。两条直线平行当且仅当它们的斜率相同。
返回可以从 points 中任意选择四个不同点组成的 水平梯形 数量。由于答案可能非常大，请返回结果对 109 + 7 取余数后的值。

3371.识别数组中的最大异常值：给你一个整数数组 nums。该数组包含 n 个元素，
其中 恰好 有 n - 2 个元素是 特殊数字 。剩下的 两个 元素中，一个是所有 特殊数字 的 和 ，另一个是 异常值 。
异常值 的定义是：既不是原始特殊数字之一，也不是所有特殊数字的和。
注意，特殊数字、和 以及 异常值 的下标必须 不同 ，但可以共享 相同 的值。返回 nums 中可能的 最大异常值。

624.数组列表中的最大距离：给定 m 个数组，每个数组都已经按照升序排好序了。
现在你需要从两个不同的数组中选择两个整数（每个数组选一个）并且计算它们的距离。
两个整数 a 和 b 之间的距离定义为它们差的绝对值 |a-b| 。返回最大距离。

2364.统计坏数对的数目：给你一个下标从 0 开始的整数数组 nums 。
如果 i < j 且 j - i != nums[j] - nums[i] ，那么我们称 (i, j) 是一个 坏数对 。
请你返回 nums 中 坏数对 的总数目。
*/
// ---------------------
// 模板题1：两数之和，用哈希表在O(1)时间内获取O(n)信息，同时枚举右，寻找左
namespace s1o1
{
    class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            unordered_map<int, int> map;
            for (int i = 0; i < nums.size(); ++i) {
                // if (map.contains(target - nums[i]) since C++ 20
                auto it = map.find(target - nums[i]);
                if (it != map.end()) {
                    return { i, it->second };
                }
                map[nums[i]] = i;
            }
            return { -1, -1 };
        }
    };
}

namespace s2441m1
{
    class Solution {
    public:
        int findMaxK(vector<int>& nums) {
            unordered_set<int> set;
            int ans = -1;
            for (int x : nums) {
                auto it = set.find(-x);
                if (it != set.end()) {
                    ans = max(abs(x), ans);
                }
                set.insert(x);
            }
            return ans;
        }
    };
}

namespace s1512m1
{
    class Solution {
    public:
        int numIdenticalPairs(vector<int>& nums) {
            unordered_map<int, int> count;
            int ans = 0;
            for (int x : nums) {
                ans += count[x]++;
            }
            return ans;
        }
    };
}

namespace s2001m1
{
    class Solution {
    public:
        long long interchangeableRectangles(vector<vector<int>>& rectangles) {
            long long ans = 0;
            unordered_map<double, int> map;
            for (auto& p : rectangles) {
                double ratio = (double)p[0] / p[1];
                if (++map[ratio] > 1) {
                    ans += map[ratio] - 1;
                }
            }
            return ans;
        }
    };
}

namespace s1128m1
{
    class Solution {
    public:
        int numEquivDominoPairs(vector<vector<int>>& dominoes) {
            map<pair<int, int>, int> map;// 因为pair没有默认哈希函数，所以无法当成unordered_map的key
            // 因为有无谓的排序，速度会慢一些：Ologn
            int ans = 0;
            for (auto& p : dominoes) {
                if (p[1] > p[0]) {
                    swap(p[1], p[0]);
                }
                pair<int, int> cur(p[0], p[1]);
                // ans += map[cur]++; // 下面的if循环可以简化为这一句
                if (++map[cur] > 1) {
                    ans += map[cur] - 1;
                }
            }
            return ans;
        }
    };
}
namespace s1128o1
{   // 利用题干中的信息：dominoes[i][j]在1至9之间，可以用10x10数组充当哈希表
    class Solution {
    public:
        int numEquivDominoPairs(vector<vector<int>>& dominoes) {
            int cnt[10][10]{};
            int ans = 0;
            for (auto& p : dominoes) {
                if (p[1] > p[0]) {
                    swap(p[1], p[0]);
                }
                ans += cnt[p[0]][p[1]]++;
            }
            return ans;
        }
    };
}

namespace s121m1
{
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int ans = 0, n = prices.size();
            int mn = prices[0];
            for (int i = 1; i < n; ++i) {
                if (prices[i] > mn) {
                    ans = max(prices[i] - mn, ans);
                }
                else {
                    mn = min(prices[i], mn);
                }
            }
            return ans;
        }
    };
}

// 同s121
namespace s2016m1
{
    class Solution {
    public:
        int maximumDifference(vector<int>& nums) {
            int mn = nums[0];
            int ans = -1;
            int n = nums.size();
            for (int i = 0; i < n; ++i) {
                if (nums[i] > mn) {
                    ans = max(ans, nums[i] - mn);
                }
                else {
                    mn = min(mn, nums[i]);
                }
            }
            return ans;
        }
    };
}

namespace s219m1
{
    class Solution {
    public:
        bool containsNearbyDuplicate(vector<int>& nums, int k) {
            // 维护左边元素的下标最大值
            unordered_map<int, int> map;
            for (int i = 0; i < nums.size(); ++i) {
                auto it = map.find(nums[i]);
                if (it != map.end() && i - it->second <= k) {
                    return true;
                }
                map[nums[i]] = i;
            }
            return false;
        }
    };
}
namespace s219o1
{   // 虽然形式类似，但是使用定长滑窗思维来解题（窗口长度为min(k + 1, n)）
    class Solution {
    public:
        bool containsNearbyDuplicate(vector<int>& nums, int k) {
            unordered_set<int> st;
            for (int i = 0; i < nums.size(); i++) {
                if (!st.insert(nums[i]).second) { // st 中有 nums[i]
                    // insert操作返回pair<iterator, bool>，如果插入成功则bool为true
                    // 这样写可以同时完成插入和判断是否集合内存在元素两个操作
                    return true;
                }
                if (i >= k) {
                    st.erase(nums[i - k]);
                }
            }
            return false;
        }
    };
}

namespace s2260m1
{
    class Solution {
    public:
        int minimumCardPickup(vector<int>& cards) {
            unordered_map<int, int> map;
            int ans = INT_MAX;
            for (int i = 0; i < cards.size(); ++i) {
                auto it = map.find(cards[i]);
                if (it != map.end()) {
                    ans = min(ans, i - it->second + 1);
                }
                map[cards[i]] = i;
            }
            return ans == INT_MAX ? -1 : ans;
        }
    };
}

namespace s2815m1
{
    class Solution {
    public:
        int maxSum(vector<int>& nums) {
            auto digit = [&](int x) -> int {
                int mx = 0;
                while (x) {
                    mx = max(mx, x % 10);
                    x /= 10;
                }
                return mx;
                };

            unordered_map<int, int> map;
            int ans = -1;
            for (int x : nums) {
                int mx = digit(x);
                auto it = map.find(mx);
                if (it != map.end()) {
                    ans = max(ans, x + it->second);
                    it->second = max(it->second, x);
                }
                else {
                    map[mx] = x;
                }
            }
            return ans;
        }
    };
}
namespace s2815o1
{   // 用数组替代哈希表提速
    class Solution {
    public:
        int maxSum(vector<int>& nums) {
            auto digit = [&](int x) -> int {
                int mx = 0;
                while (x) {
                    mx = max(mx, x % 10);
                    x /= 10;
                }
                return mx;
                };

            int map[10]{};
            int ans = -1;
            for (int x : nums) {
                int mx = digit(x);
                if (map[mx] != 0) {
                    ans = max(ans, x + map[mx]);
                    map[mx] = max(map[mx], x);
                }
                else {
                    map[mx] = x;
                }
            }
            return ans;
        }
    };
}

namespace s2342m1
{
    class Solution {
    public:
        int maximumSum(vector<int>& nums) {
            int ans = -1;
            unordered_map<int, int> map;
            for (int x : nums) {
                int temp = x;
                int digSum = 0;
                while (temp) {
                    digSum += temp % 10;
                    temp /= 10;
                }
                auto it = map.find(digSum);
                if (it != map.end()) {
                    ans = max(ans, it->second + x);
                    it->second = max(it->second, x);
                }
                else {
                    map[digSum] = x;
                }
            }
            return ans;
        }
    };
}

namespace s1679m1
{
    class Solution {
    public:
        int maxOperations(vector<int>& nums, int k) {
            unordered_map<int, int> mp;
            int ans = 0;
            for (int x : nums) {
                int target = k - x;
                auto it = mp.find(target);
                if (it != mp.end() && it->second) {
                    --it->second;
                    ++ans;
                }
                else {
                    ++mp[x];
                }
            }
            return ans;
        }
    };
}

namespace ms16_24m1 {
    class Solution {
    public:
        vector<vector<int>> pairSums(vector<int>& nums, int target) {
            unordered_map<int, int> mp;
            vector<vector<int>> ans;
            for (int x : nums) {
                int k = target - x;
                auto it = mp.find(k);
                if (it != mp.end() && it->second) {
                    --it->second;
                    ans.push_back({ x, k });
                }
                else {
                    ++mp[x];
                }
            }
            return ans;
        }
    };
}

namespace s3623m1
{
    class Solution {
    public:
        int countTrapezoids(vector<vector<int>>& points) {
            /*
            1.先找出有多少条平行x轴的线。（y值相同的点在一条线上，至少要有两个y值相同的点构成一条线）
            2.以组合的形式计算每条线上的两两组合数量，比如有3个点，那么就总共有2个组合。
            3.在所有线之间再进行两两组合，所有组合数的总和 S = a + b + c + ...， S^2 展开后会是：
            S^2 = a^2 + b^2 + c^2 + 2ab + 2ac + 2bc + ...
            我们要的是ab + ac + bc + ...，所以total_pairs = (S^2 - (a^2 + b^2 + c^2)) / 2
            */
            const int mod = 1e9 + 7;// 最好代码第一行就写这个
            unordered_map<int, int> mp;
            for (auto& p : points) {
                ++mp[p[1]];
            }
            long long sum = 0, sumSqr = 0;
            for (auto& p : mp) {
                long long x = p.second;
                if (x > 1) {// 这里的x > 1判断可以删掉，因为如果x = 0，对答案没有影响
                    long long s = 1LL * x * (x - 1) / 2;// 加1LL防止溢出
                    sum += s;
                    sumSqr += s * s;
                }
            }
            long long total = (sum * sum - sumSqr) / 2;
            return total % mod;// 这里注意要将mod转成int，因为%运算符不接收浮点数，1e9 + 7默认是double
        }
    };
}
namespace s3623o1
{   // 灵神做法没有用到数学公式推导，更好理解，效率是相近的
    class Solution {
    public:
        int countTrapezoids(vector<vector<int>>& points) {
            const int mod = 1e9 + 7;
            unordered_map<int, int> mp;
            for (auto& p : points) {
                ++mp[p[1]];
            }
            long long ans = 0, s = 0;
            for (auto& p : mp) {
                long long x = p.second;
                long long k = 1LL * x * (x - 1) / 2;
                ans += k * s;
                s += k;
            }
            return ans % mod;
        }
    };
}

// 负数对2取模可能得到-1，不是所有整数取模都能得到1或0
namespace s3371m1
{
    class Solution {
    public:
        int getLargestOutlier(vector<int>& nums) {
            // 生成哈希表，并在同次遍历中计算sum
            // target = (sum - x) / 2，需要在哈希表中
            int sum = 0;
            unordered_map<int, int> mp;
            for (int x : nums) {
                ++mp[x];
                sum += x;
            }
            int ans = -1000;
            for (auto& p : mp) {
                int target = sum - p.first;
                if (abs(target % 2) == 1) continue;// 这里要取绝对值
                auto it = mp.find(target / 2);
                if (it != mp.end()) {
                    if (it->first == p.first && p.second == 1) {// 可能target是自身，所以不能用unordered_set
                        // 要用map的频次记录功能
                        continue;
                    }
                    ans = max(ans, p.first);
                }
            }
            return ans;
        }
    };
}
namespace s3371m2
{   // m1的优化版本
    class Solution {
    public:
        int getLargestOutlier(vector<int>& nums) {
            int sum = 0;
            unordered_map<int, int> mp;
            for (int x : nums) {
                ++mp[x];
                sum += x;
            }
            int ans = -1000;
            for (auto& p : mp) {// 也可以再次遍历nums而不是mp，但两者区别不大
                if ((sum - p.first) % 2 == 0) {
                    int target = (sum - p.first) / 2;
                    auto it = mp.find(target);
                    if (it != mp.end() && (it->first != p.first || p.second > 1)) {
                        ans = max(ans, p.first);
                    }
                }
            }
            return ans;
        }
    };
}
namespace s3371o1
{   // 思路一致，但写法不一样
    class Solution {
    public:
        int getLargestOutlier(vector<int>& nums) {
            int sum = 0;
            unordered_map<int, int> mp;
            for (int x : nums) {
                ++mp[x];
                sum += x;
            }
            int ans = -1000;
            for (int x : nums) {
                --mp[x];
                if ((sum - x) % 2 == 0 && mp[(sum - x) / 2] > 0) {
                    ans = max(ans, x);
                }
                ++mp[x];
            }
            return ans;
        }
    };
}

// 还收录在10.1贪心策略1.1中
namespace s624m1
{   // 如果只是找所有数组中的最大最小值相减，可能会出现最大值和最小值同时出自一个数组的情况，所以这个思路不对
    class Solution {
    public:
        int maxDistance(vector<vector<int>>& arrays) {
            int mn = 10e4 + 1, mx = -10e4 - 1;
            int ans = 0;
            for (auto& arr : arrays) {
                ans = max({ ans, mx - arr[0], arr.back() - mn });
                // 在更新mx, mn前更新ans，规避mx, mn出自同一个数组的情况
                mn = min(mn, arr[0]);
                mx = max(mx, arr.back());
            }
            return ans;
        }
    };
}

namespace s2364m1
{
    class Solution {
    public:
        long long countBadPairs(vector<int>& nums) {
            unordered_map<int, int> mp;
            long long ans = 0;
            int n = nums.size();
            for (int i = 0; i < n; ++i) {
                int key = nums[i] - i;
                ++mp[key];
                ans += i + 1 - mp[key];
            }
            return ans;
        }
    };
}
// ---------------------
// 【0.1.2】枚举右，维护左-进阶 (6)
// 将问题转换成最简单的两数之和的过程更抽象和复杂
/*
1010.总持续时间可被 60 整除的歌曲：在歌曲列表中，第 i 首歌曲的持续时间为 time[i] 秒。
返回其总持续时间（以秒为单位）可被 60 整除的歌曲对的数量。形式上，
我们希望下标数字 i 和 j 满足  i < j 且有 (time[i] + time[j]) % 60 == 0。

3185.构成整天的下标对数目 II：给你一个整数数组 hours，表示以 小时 为单位的时间，
返回一个整数，表示满足 i < j 且 hours[i] + hours[j] 构成 整天 的下标对 i, j 的数目。
整天 定义为时间持续时间是 24 小时的 整数倍 。
例如，1 天是 24 小时，2 天是 48 小时，3 天是 72 小时，以此类推。

2748.美丽下标对的数目：给你一个下标从 0 开始的整数数组 nums 。
如果下标对 i、j 满足 0 ≤ i < j < nums.length ，
如果 nums[i] 的 第一个数字 和 nums[j] 的 最后一个数字 互质 ，则认为 nums[i] 和 nums[j] 是一组 美丽下标对 。
返回 nums 中 美丽下标对 的总数目。
对于两个整数 x 和 y ，如果不存在大于 1 的整数可以整除它们，则认为 x 和 y 互质 。
换而言之，如果 gcd(x, y) == 1 ，则认为 x 和 y 互质，其中 gcd(x, y) 是 x 和 y 的 最大公因数 。

2506.统计相似字符串对的数目：给你一个下标从 0 开始的字符串数组 words 。
如果两个字符串由相同的字符组成，则认为这两个字符串 相似 。
例如，"abca" 和 "cba" 相似，因为它们都由字符 'a'、'b'、'c' 组成。
然而，"abacba" 和 "bcfd" 不相似，因为它们不是相同字符组成的。
请你找出满足字符串 words[i] 和 words[j] 相似的下标对 (i, j) ，
并返回下标对的数目，其中 0 <= i < j <= words.length - 1 。

2874.有序三元组中的最大值 II：给你一个下标从 0 开始的整数数组 nums 。
请你从所有满足 i < j < k 的下标三元组 (i, j, k) 中，找出并返回下标三元组的最大值。
如果所有满足条件的三元组的值都是负数，则返回 0 。
下标三元组 (i, j, k) 的值等于 (nums[i] - nums[j]) * nums[k] 。

454.四数相加II：
*/
// ---------------------
// 模板题2：对与两数之和进行复杂化，比如取模等
namespace s1010m1
{   // 1000 / 60 = 16，暴力
    class Solution {
    public:
        int numPairsDivisibleBy60(vector<int>& time) {
            unordered_map<int, int> mp;
            int ans = 0;
            for (int x : time) {
                for (int i = 1; i <= 16; ++i) {
                    int target = 60 * i - x;
                    auto it = mp.find(target);
                    if (it != mp.end()) {
                        ans += it->second;
                    }
                }
                ++mp[x];
            }
            return ans;
        }
    };
}
namespace s1010o1
{   // 一般地，对于 time[i]，需要知道左边有多少个模 60 是 60 - time[i]mod60 的数
    // 特别地，如果time[i]模60是0，那么需要知道左边有多少个模60也是0的数
    // 两种情况合并为：累加左边 (60 - time[i]mod60)mod60 的出现次数
    // 进一步提速，因为是模60，可以用大小为60的速度替代哈希表提速
    class Solution {
    public:
        int numPairsDivisibleBy60(vector<int>& time) {
            int ans = 0, mp[60]{};
            for (int x : time) {
                int target = (60 - x % 60) % 60;
                ans += mp[target];
                ++mp[x % 60];
            }
            return ans;
        }
    };
}

// 同s1010
namespace s3185m1
{
    class Solution {
    public:
        long long countCompleteDayPairs(vector<int>& hours) {
            long long ans = 0;
            int mp[24]{};
            for (int x : hours) {
                ans += mp[(24 - x % 24) % 24];
                ++mp[x % 24];
            }
            return ans;
        }
    };
}

// 最大公约数gcd函数（C++17）
namespace s2748o1
{
    class Solution {
    public:
        int countBeautifulPairs(vector<int>& nums) {
            int ans = 0, mp[10]{};
            for (int x : nums) {
                for (int i = 1; i < 10; ++i) {
                    if (mp[i] > 0 && gcd(i, x % 10) == 1) {
                        ans += mp[i];
                    }
                }
                while (x >= 10) {
                    x /= 10;
                }
                ++mp[x];
            }
            return ans;
        }
    };
}

namespace s2506m1
{   // 最简单直接的想法，把每个去重+排序后的元素作为key
    class Solution {
    public:
        int similarPairs(vector<string>& words) {
            unordered_map<string, int> dic;
            int ans = 0;
            for (auto& s : words) {
                set<char> st;
                for (char c : s) {
                    st.insert(c);
                }
                string temp = "";
                for (char c : st) {
                    temp += c;
                }
                auto it = dic.find(temp);
                if (it != dic.end()) {
                    ans += it->second;
                }
                ++dic[temp];
            }
            return ans;
        }
    };
}
namespace s2506o1
{   // 经典26个小写字母的位运算加速
    class Solution {
    public:
        int similarPairs(vector<string>& words) {
            unordered_map<int, int> cnt;
            int ans = 0;
            for (auto& s : words) {
                int mask = 0;// 初始化空集合
                for (char c : s) {
                    mask |= (1 << (c - 'a'));
                }
                ans += cnt[mask]++;
            }
            return ans;
        }
    };
}

// 前缀最大数组，后缀最大数组的应用技巧
// 模板题3：掌握o2解法可以对变量更新融会贯通！
namespace s2874m1
{   // 注意数组元素为正整数
    class Solution {
    public:
        long long maximumTripletValue(vector<int>& nums) {
            long long ans = 0;
            int n = nums.size();
            vector<int> preMax(n);
            vector<int> sufMax(n);
            int premx = 0;
            for (int i = 0; i < n; ++i) {
                premx = max(premx, nums[i]);
                preMax[i] = premx;
            }
            int sufmx = 0;
            for (int i = n - 1; i >= 0; --i) {
                sufmx = max(sufmx, nums[i]);
                sufMax[i] = sufmx;
            }
            for (int i = 1; i < n - 1; ++i) {
                long long tri = 1LL * (preMax[i - 1] - nums[i]) * sufMax[i + 1];
                ans = max(ans, tri);
            }
            return ans;
        }
    };
}
namespace s2874m2
{
    class Solution {
    public:
        long long maximumTripletValue(vector<int>& nums) {
            // 将前缀最大值数组的计算和更新答案的计算放在一个循环内，省一次遍历
            long long ans = 0;
            int n = nums.size();
            vector<int> preMax(n);
            vector<int> sufMax(n);
            int sufmx = 0;
            for (int i = n - 1; i >= 0; --i) {
                sufmx = max(sufmx, nums[i]);
                sufMax[i] = sufmx;
            }
            int premx = nums[0]; preMax[0] = premx;
            for (int i = 1; i < n - 1; ++i) {
                premx = max(premx, nums[i]);
                preMax[i] = premx;
                long long tri = 1LL * (preMax[i - 1] - nums[i]) * sufMax[i + 1];
                ans = max(ans, tri);
            }
            return ans;
        }
    };
}
namespace s2874m3
{   // 其实可以不用分配数组空间给preMax，直接用一个变量表示即可
    class Solution {
    public:
        long long maximumTripletValue(vector<int>& nums) {
            int n = nums.size();
            vector<int> sufMax(n);
            int sufmx = 0;
            for (int i = n - 1; i > 1; --i) {// 后缀最大值数组也不需要下标从0开始，而是从2开始
                sufmx = max(sufmx, nums[i]);
                sufMax[i] = sufmx;
            }
            long long ans = 0;
            int premx = nums[0];
            for (int i = 1; i < n - 1; ++i) {
                ans = max(ans, 1LL * (premx - nums[i]) * sufMax[i + 1]);
                premx = max(premx, nums[i]);
            }
            return ans;
        }
    };
}
namespace s2874o1
{   // 整体框架和思路和m1,m2,m3一致，但是求sufMax的过程值得学习
    class Solution {
    public:
        long long maximumTripletValue(vector<int>& nums) {
            int n = nums.size();
            vector<int> sufMax(n + 1, 0);
            for (int i = n - 1; i > 1; --i) {
                sufMax[i] = max(sufMax[i + 1], nums[i]);// 类似递归的技巧
            }
            long long ans = 0;
            int premx = nums[0];
            for (int i = 1; i < n - 1; ++i) {
                ans = max(ans, 1LL * (premx - nums[i]) * sufMax[i + 1]);
                premx = max(premx, nums[i]);
            }
            return ans;
        }
    };
}
namespace s2874o2
{   // 空间复杂度O(1)的解法，i, j , k枚举k，需要知道 k 左边 nums[i] - nums[j] 的最大值
    /* 是s121的进阶版本
    首先更新 ans，此时 maxDiff 还没有更新，表示在当前元素左边的两个数的最大差值。
    然后更新 maxDiff，此时 preMax 还没有更新，表示在当前元素左边的最大值。
    最后更新 preMax。变量更新的顺序不能改变
    */
    class Solution {
    public:
        long long maximumTripletValue(vector<int>& nums) {
            long long ans = 0;
            int preMax = 0, maxDiff = 0;
            for (int x : nums) {
                ans = max(ans, 1LL * maxDiff * x);
                maxDiff = max(maxDiff, preMax - x);
                preMax = max(preMax, x);
            }
            return ans;
        }
    };
}

namespace s454o1
{   // 将四数之和两两拆解，转换为两数之和
    // 这题比较简单的原因在于只需要返回满足条件的元组的个数，而不要求返回序号
    class Solution {
    public:
        int fourSumCount(vector<int>& nums1, vector<int>& nums2, vector<int>& nums3, vector<int>& nums4) {
            unordered_map<int, int> abMap;
            for (int a : nums1) {
                for (int b : nums2) {
                    ++abMap[a + b];
                }
            }
            int ans = 0;
            for (int c : nums3) {
                for (int d : nums4) {
                    auto it = abMap.find(-c - d);
                    if (it != abMap.end()) {
                        ans += it->second;
                    }
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【0.2】枚举中间 (7)
// 对于三个或者四个变量的问题，枚举中间的变量往往更好算
// 对i < j < k，若枚举 i，后续计算中还需保证 j < k
// 若枚举 j，那么 i 和 k 自动被 j 隔开，互相独立，后续计算中无需关心 i 和 k 的位置关系
/*
2909.元素和最小的山形三元组II：给你一个下标从 0 开始的整数数组 nums 。
如果下标三元组 (i, j, k) 满足下述全部条件，则认为它是一个 山形三元组 ：i < j < k
nums[i] < nums[j] 且 nums[k] < nums[j]
请你找出 nums 中 元素和最小 的山形三元组，并返回其 元素和 。如果不存在满足条件的三元组，返回 -1 。

3583.统计特殊三元组：给你一个整数数组 nums。
特殊三元组 定义为满足以下条件的下标三元组 (i, j, k)：
0 <= i < j < k < n，其中 n = nums.length
nums[i] == nums[j] * 2; nums[k] == nums[j] * 2
返回数组中 特殊三元组 的总数。由于答案可能非常大，请返回结果对 109 + 7 取余数后的值。

1930.长度为 3 的不同回文子序列：给你一个字符串 s ，返回 s 中 长度为 3 的不同回文子序列 的个数。
即便存在多种方法来构建相同的子序列，但相同的子序列只计数一次。回文 是正着读和反着读一样的字符串。
子序列 是由原字符串删除其中部分字符（也可以不删除）且不改变剩余字符之间相对顺序形成的一个新字符串。
例如，"ace" 是 "abcde" 的一个子序列。

3128.直角三角形：给你一个二维 boolean 矩阵 grid 。
如果 grid 的 3 个元素的集合中，一个元素与另一个元素在 同一行，并且与第三个元素在 同一列，
则该集合是一个 直角三角形。3 个元素 不必 彼此相邻。
请你返回使用 grid 中的 3 个元素可以构建的 直角三角形 数目，且满足 3 个元素值 都 为 1 。

447.回旋镖的数量：给定平面上 n 对 互不相同 的点 points ，其中 points[i] = [xi, yi] 。
回旋镖 是由点 (i, j, k) 表示的元组 ，其中 i 和 j 之间的欧式距离和 i 和 k 之间的欧式距离相等（需要考虑元组的顺序）。
返回平面上所有回旋镖的数量。

456.132模式：给你一个整数数组 nums ，
数组中共有 n 个整数。132 模式的子序列 由三个整数 nums[i]、nums[j] 和 nums[k] 组成，
并同时满足：i < j < k 和 nums[i] < nums[k] < nums[j] 。
如果 nums 中存在 132 模式的子序列 ，返回 true ；否则，返回 false 。

1534.统计好三元组：给你一个整数数组 arr ，以及 a、b 、c 三个整数。请你统计其中好三元组的数量。
如果三元组 (arr[i], arr[j], arr[k]) 满足下列全部条件，则认为它是一个 好三元组 。
0 <= i < j < k < arr.length
|arr[i] - arr[j]| <= a; |arr[j] - arr[k]| <= b; |arr[i] - arr[k]| <= c
其中 |x| 表示 x 的绝对值。返回 好三元组的数量 。
*/
// ---------------------
namespace s2909m1
{
    class Solution {
    public:
        int minimumSum(vector<int>& nums) {
            int n = nums.size();
            vector<int> sufMIn(n + 1, INT_MAX);
            for (int i = n - 1; i >= 2; --i) {
                sufMIn[i] = min(nums[i], sufMIn[i + 1]);
            }
            /*
            // 计算sufMin的过程也可以用下面的，全部初始化0会比初始化为INT_MAX更快些
            vector<int> sufMIn(n);
            sufMIn[n - 1] = nums[n - 1];
            for (int i = n - 2; i >= 2; --i) {
                sufMIn[i] = min(nums[i], sufMIn[i + 1]);
            }
            */
            int preMIn = nums[0], ans = INT_MAX;
            for (int i = 1; i < n - 1; ++i) {
                if (nums[i] > preMIn && nums[i] > sufMIn[i + 1]) {
                    ans = min(ans, nums[i] + preMIn + sufMIn[i + 1]);
                }
                preMIn = min(preMIn, nums[i]);
            }
            return ans == INT_MAX ? -1 : ans;
        }
    };
}

// 模板题4：前后缀数组 + 哈希表撤销，体会枚举中间的优势
namespace s3583m1
{   // 写法比较复杂，多用了sufVec的空间
    class Solution {
    public:
        int specialTriplets(vector<int>& nums) {
            const int mod = 1e9 + 7;
            long long ans = 0;
            int n = nums.size();
            vector<int> sufVec(n);
            unordered_map<int, int> sufMap;
            for (int i = n - 1; i >= 0; --i) {
                auto it = sufMap.find(2 * nums[i]);
                if (it != sufMap.end()) {
                    sufVec[i] = it->second;
                }
                ++sufMap[nums[i]];
            }
            unordered_map<int, int> preMap;
            for (int i = 0; i < n; ++i) {
                auto it = preMap.find(2 * nums[i]);
                if (it != preMap.end()) {
                    ans += 1LL * it->second * sufVec[i];
                }
                ++preMap[nums[i]];
            }
            return ans % mod;
        }
    };
}
namespace s3583o1
{   // 用后缀map撤销的做法，省去sufVec的空间占用
    class Solution {
    public:
        int specialTriplets(vector<int>& nums) {
            const int mod = 1e9 + 7;
            long long ans = 0;
            int n = nums.size();
            unordered_map<int, int> sufMap;
            unordered_map<int, int> preMap;
            for (int x : nums) {
                ++sufMap[x];
            }
            for (int x : nums) {
                --sufMap[x];
                int leftCnt = preMap.count(x * 2) ? preMap[x * 2] : 0;// 比map.find()更简洁
                int rightCnt = sufMap.count(x * 2) ? sufMap[x * 2] : 0;
                ans += 1LL * leftCnt * rightCnt;// 注意用1LL，leftCnt * rightCnt最大为1e5 * 1e5 = 1e10 > INT_MAX
                ++preMap[x];
            }
            return ans % mod;
        }
    };
}

namespace s1930m1
{   // 记录每个字符的首尾位置，再统计之间的不同字符，思路比较简单，但是比较慢
    class Solution {
    public:
        int countPalindromicSubsequence(string s) {
            int n = s.size();
            unordered_map<char, pair<int, int>> char_indices; // 存储每个字符的first和last出现位置

            // 记录每个字符的首次和最后出现位置
            for (int i = 0; i < n; ++i) {
                char c = s[i];
                if (char_indices.find(c) == char_indices.end()) {
                    char_indices[c] = { i, i };
                }
                else {
                    char_indices[c].second = i;
                }
            }

            int count = 0;

            // 对于每个字符，计算其first和last之间有多少不同字符
            for (auto& item : char_indices) {
                int first = item.second.first;
                int last = item.second.second;

                if (first + 1 >= last) {
                    continue; // 无法形成长度为3的回文子序列
                }

                unordered_set<char> unique_chars;
                for (int i = first + 1; i < last; ++i) {
                    unique_chars.insert(s[i]);
                }
                count += unique_chars.size();
            }

            return count;
        }
    };
}
namespace s1930o1
{
    class Solution {
    public:
        int countPalindromicSubsequence(string s) {
            int pre[26]{};
            int suf[26]{};
            unsigned int mask[26]{};
            int n = s.size();
            for (char c : s) {
                ++suf[c - 'a'];
            }
            for (char c : s) {
                --suf[c - 'a'];
                for (int i = 0; i < 26; ++i) {
                    if (suf[i] && pre[i]) {
                        mask[c - 'a'] |= (1 << i);
                    }
                }
                ++pre[c - 'a'];
            }
            int ans = 0;
            for (int x : mask) {
                ans += __popcnt(x);// 计算bit位中1的个数 __popcnt接收unsigned int, 
                // has用int也能通过，但是用unsigned int更好
            }
            return ans;
        }
    };
}

namespace s3128m1
{
    class Solution {
    public:
        long long numberOfRightTriangles(vector<vector<int>>& grid) {
            int m = grid.size();
            int n = grid[0].size();
            vector<int> row(m);
            vector<int> col(n);
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (grid[i][j]) {
                        ++row[i];
                        ++col[j];
                    }
                }
            }
            long long ans = 0;
            for (int i = 0; i < m; ++i) {
                if (row[i] >= 2) {
                    for (int j = 0; j < n; ++j) {
                        if (grid[i][j] && col[j] >= 2) {
                            ans += (row[i] - 1) * (col[j] - 1);
                        }
                    }
                }
            }
            return ans;
        }
    };
}

// s2874, 见【0.1】
namespace s2874
{

}

namespace s447m1
{   // O(n^2)时间复杂度，有时候复杂度就是这么高，不要一直纠结，先把题目做出来才是第一要务
    class Solution {
    public:
        int numberOfBoomerangs(vector<vector<int>>& points) {
            // 所有点互不相同
            unordered_map<int, int> cnt;
            int ans = 0;
            for (auto& p1 : points) {
                cnt.clear();
                for (auto& p2 : points) {
                    int d = (p1[0] - p2[0]) * (p1[0] - p2[0]) +
                        (p1[1] - p2[1]) * (p1[1] - p2[1]);
                    ans += cnt[d]++ * 2;
                }
            }
            return ans;
        }
    };
}

namespace s456o1
{   // 时间复杂度：O(nlogn)，最好理解的方法
    class Solution {
    public:
        bool find132pattern(vector<int>& nums) {
            // 左边：左边数里的最小值
            // 右边：二分查找第一个比左边最小值大的数
            // 枚举i, j, k中的j，也即是枚举中间，如果挨个遍历会是n^2复杂度超时，用multiset + 二分提速
            int n = nums.size();
            if (n < 3) {
                return false;
            }
            int lmn = nums[0];
            multiset<int> rightSet;
            for (int k = 2; k < n; ++k) {
                rightSet.insert(nums[k]);
            }
            for (int j = 1; j < n - 1; ++j) {
                if (nums[j] > lmn) {
                    auto it = rightSet.upper_bound(lmn);
                    if (it != rightSet.end() && *it < nums[j]) {
                        return true;
                    }
                }
                lmn = min(lmn, nums[j]);
                rightSet.erase(rightSet.find(nums[j + 1]));
            }
            return false;
        }
    };
}
namespace s456o2
{   // 单调栈做法，O(n)做法，枚举i, j, k中的i
    class Solution {
    public:
        bool find132pattern(vector<int>& nums) {
            int n = nums.size();
            stack<int> candidate_k;
            candidate_k.push(nums[n - 1]);
            int max_k = INT_MIN;

            for (int i = n - 2; i >= 0; --i) {
                if (nums[i] < max_k) {
                    return true;
                }
                while (!candidate_k.empty() && nums[i] > candidate_k.top()) {
                    max_k = candidate_k.top();
                    candidate_k.pop();
                }
                if (nums[i] > max_k) {
                    candidate_k.push(nums[i]);
                }
            }

            return false;
        }
    };
}
namespace s456o3
{   // O(n)复杂度，枚举i, j, k中的k
    class Solution {
    public:
        bool find132pattern(vector<int>& nums) {
            // 维护前缀最小值+单调栈
            // 先从单调栈中找到左侧离 i 最近的比 nums[i] 大的元素
            // 再查询这个值左侧的最小值，只要这个最小值小于 nums[i] 即可
            const int n = nums.size();
            stack<int> st;
            vector<int> pmin{ INT_MAX };
            for (int i = 0; i < n; ++i) {
                while (!st.empty() && nums[st.top()] <= nums[i])
                    st.pop();
                if (!st.empty() && pmin[st.top()] < nums[i])
                    return true;
                st.push(i);
                pmin.push_back(min(pmin.back(), nums[i]));
            }
            return false;
        }
    };
}

// m1：暴力枚举 O(n^3)
// o1：因本题arr[i]范围最大为1000，可以用前缀和做法，时间复杂度O(n(n+U))，其中 n 是 arr 的长度，U=max(arr)，
// o2：下标数组排序 + 枚举中间 + 三指针，时间复杂度O(n^2)
namespace s1534m1
{   // 暴力枚举，O(n^3)时间复杂度
    class Solution {
    public:
        int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
            int n = arr.size();
            int count = 0;
            for (int i = 0; i < n - 2; ++i) {
                for (int j = i + 1; j < n - 1; ++j) {
                    int diff1 = abs(arr[i] - arr[j]);
                    if (diff1 > a)
                        continue;
                    for (int k = j + 1; k < n; ++k) {
                        int diff2 = abs(arr[j] - arr[k]);
                        int diff3 = abs(arr[i] - arr[k]);
                        if (diff2 > b || diff3 > c)
                            continue;
                        ++count;
                    }
                }
            }
            return count;
        }
    };
}
namespace s1534o1
{   
    /*枚举 j 和 k，可以确定ai的范围
        aj - a <= ai <= aj + a
        ak - c <= ai <= ak + c
        还有0 <= ai <= max(A)
        求交集即为ai的范围：max(aj - a, ak - c, 0) - min(aj + a, ak + c, max(A))
        问题转换为求交集中ai的个数
        该题前缀和为 prefix[i] = prefix[i - 1] + nums[i - 1]，prefix代表数组中“x < i”的个数
        统计每个元素值出现的次数，将次数作为前缀和
    */
    class Solution {
    public:
        int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
            // 前缀和数组的大小：下标是数值范围，因为错位定义
            // 代表小于当前下标的值的个数，要包含mx最大值，
            // 那么下标必须至少为mx + 1，所以大小为mx + 1 + 1 = mx + 2
            int n = arr.size();
            int ans = 0;
            int mx = *max_element(arr.begin(), arr.end());
            vector<int> prefix(mx + 2);
            for (int j = 0; j < n - 1; ++j) {// j 从0开始是为了prefix[1]的更新
                int y = arr[j];
                for (int k = j + 1; k < n; ++k) {
                    int z = arr[k];
                    if (abs(y - z) > b) {
                        continue;
                    }
                    int l = max({ 0, y - a, z - c });
                    int r = min({ mx, y + a, z + c });
                    ans += max(0, prefix[r + 1] - prefix[l]);// 如果 l > r + 1，s[r + 1] - s[l] 可能是负数
                }
                for (int v = y + 1; v < mx + 2; ++v) {
                    ++prefix[v];// 直接维护前缀和数组，读取到的当前arr[j] = y，要更新后面前缀和数组中的所有值
                }
            }
            return ans;
        }
    };
}
namespace s1534o2
{   // 下标数组 + 三指针做法，对于arr[i]范围更大的情况适用，本题范围只有1000，所以用前缀和没问题
    class Solution {
    public:
        int countGoodTriplets(vector<int>& arr, int a, int b, int c) {
            int n = arr.size(), ans = 0;
            vector<int> idx(n);
            iota(idx.begin(), idx.end(), 0);
            sort(idx.begin(), idx.end(),
                [&](int a, int b) { return arr[a] < arr[b]; });// 下标数组排序

            for (int j : idx) {// 枚举中间
                vector<int> left, right;
                int y = arr[j];
                for (int i : idx) {
                    if (i < j && abs(arr[i] - y) <= a) {
                        left.push_back(arr[i]);
                    }
                }// 虽然这两个循环可以放在一个循环内，但分成两个效率不会变低，同时一个用i一个用k，代码可读性更好
                for (int k : idx) {
                    if (k > j && abs(arr[k] - y) <= b) {
                        right.push_back(arr[k]);
                    }
                }
                // 类似前缀和思想，遍历left，在right中寻找满足[x - c, x + c]范围的元素个数
                int k1 = 0, k2 = 0;
                for (int x : left) {
                    while (k1 < right.size() && right[k1] <= x + c) {
                        ++k1;
                    }
                    while (k2 < right.size() && right[k2] < x - c) {
                        ++k2;
                    }
                    ans += k1 - k2;
                }
            }
            return ans;
        }
    };
}