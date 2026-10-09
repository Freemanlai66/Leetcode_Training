#pragma once
#include<string>
#include<vector>
#include<algorithm>
#include<numeric> // iota
#include<unordered_map>
#include<unordered_set>
using namespace std;
// ls40o2：快速选择排序
// 1262：动态规划（通用做法：找一个数组中满足n的倍数的最大子序列和）
// 1.4节中题目和线性DP，状态机DP的区别
// 2571：普通做法和动态规划记忆化搜索做法
// 1.5节中题目与划分型DP的区别

/*
模板题：
1.田忌赛马/优势发牌，不能排序的情况下的下标数组运用：870
2.将问题n转化为问题n - 1：3191
3.等价于需要构造一个尽量长的，相邻元素不同的序列：1953
4.将1953的序列输出成答案：767
*/

 //一、贪心策略 (72)
/*从最小/最大开始贪心 + 单序列配对 + 双序列配对 +
从最左/最右开始贪心 + 划分型贪心 + 先枚举，再贪心 + 
交换论证法 + 相邻不同 + 反悔贪心*/

// 【1.1】从最小/最大开始贪心(32)
// 主要思路是排序后再处理最大、最小值
/*

3074.重新分装苹果：给你一个长度为 n 的数组 apple 和另一个长度为 m 的数组 capacity 。
一共有 n 个包裹，其中第 i 个包裹中装着 apple[i] 个苹果。
同时，还有 m 个箱子，第 i 个箱子的容量为 capacity[i] 个苹果。
请你选择一些箱子来将这 n 个包裹中的苹果重新分装到箱子中，返回你需要选择的箱子的 最小 数量。
注意，同一个包裹中的苹果可以分装到不同的箱子中。

3545.不同字符数量最多为 K 时的最少删除数：给你一个字符串 s（由小写英文字母组成）和一个整数 k。
你的任务是删除字符串中的一些字符（可以不删除任何字符），使得结果字符串中的 不同字符数量 最多为 k。
返回为达到上述目标所需删除的 最小 字符数量。

2279.装满石头的背包的最大数量：现有编号从 0 到 n - 1 的 n 个背包。
给你两个下标从 0 开始的整数数组 capacity 和 rocks 。第 i 个背包最大可以装 capacity[i] 块石头，
当前已经装了 rocks[i] 块石头。另给你一个整数 additionalRocks ，
表示你可以放置的额外石头数量，石头可以往 任意 背包中放置。
请你将额外的石头放入一些背包中，并返回放置后装满石头的背包的 最大 数量。

1833.雪糕的最大数量：夏日炎炎，小男孩 Tony 想买一些雪糕消消暑。
商店中新到 n 支雪糕，用长度为 n 的数组 costs 表示雪糕的定价，其中 costs[i] 表示第 i 支雪糕的现金价格。
Tony 一共有 coins 现金可以用于消费，他想要买尽可能多的雪糕。
注意：Tony 可以按任意顺序购买雪糕。
给你价格数组 costs 和现金量 coins ，请你计算并返回 Tony 用 coins 现金能够买到的雪糕的 最大数量 。
你必须使用计数排序解决此问题。

1005.K 次取反后最大化的数组和：给你一个整数数组 nums 和一个整数 k ，按以下方法修改该数组：
选择某个下标 i 并将 nums[i] 替换为 -nums[i] 。
重复这个过程恰好 k 次。可以多次选择同一个下标 i 。
以这种方式修改数组后，返回数组 可能的最大和 。

1481.不同整数的最少数目：给你一个整数数组 arr 和一个整数 k 。
现需要从数组中恰好移除 k 个元素，请找出移除后数组中不同整数的最少数目。


*/
// ---------------------
// 排序贪心，sort降序排列的写法，第三参数greater<int>()
namespace s3074m1
{
    class Solution {
    public:
        int minimumBoxes(vector<int>& apple, vector<int>& capacity) {
            sort(capacity.begin(), capacity.end());
            int n = apple.size(), m = capacity.size();
            int pack = 0, rest = 0;
            for (int i = m - 1; i >= 0; --i) {
                int cap = capacity[i] + rest;
                for (; pack < n; ++pack) {
                    cap -= apple[pack];
                    if (cap < 0) {
                        rest = apple[pack] + cap;// rest代表上一个被放满的篮子中分担的苹果个数
                        break;
                    }
                }
                if (pack == n) return m - i;
            }
            return m;
        }
    };
}
namespace s3074o1
{
    class Solution {
    public:
        int minimumBoxes(vector<int>& apple, vector<int>& capacity) {
            int sum = accumulate(apple.begin(), apple.end(), 0);
            sort(capacity.begin(), capacity.end());
            int i = capacity.size() - 1;
            while (sum > 0) {
                sum -= capacity[i--];
            }
            return capacity.size() - 1 - i;
        }
    };
}
namespace s3074o2
{   // 降序排列的写法
    class Solution {
    public:
        int minimumBoxes(vector<int>& apple, vector<int>& capacity) {
            int sum = accumulate(apple.begin(), apple.end(), 0);
            sort(capacity.begin(), capacity.end(), greater<int>());
            int i = 0;
            while (sum > 0) {
                sum -= capacity[i++];
            }
            return i;
        }
    };
}
// C风格数组的sort, accumulate使用
namespace s3545o1
{   // 因为不需要知道删除的是哪些字符，是需要知道删除的数量，所以可以生成字典后直接对频次进行排序
    class Solution {
    public:
        int minDeletion(string s, int k) {
            vector<int> cnt(26, 0);
            for (char c : s) {
                ++cnt[c - 'a'];
            }
            sort(cnt.begin(), cnt.end());
            return accumulate(cnt.begin(), cnt.begin() + 26 - k, 0);
        }
    };
}
namespace s3545m1
{   // 改成c风格数组的方式(sort, accumulate的使用)
    class Solution {
    public:
        int minDeletion(string s, int k) {
            int cnt[26]{ 0 };
            for (char c : s) {
                ++cnt[c - 'a'];
            }
            sort(cnt, cnt + 26);
            return accumulate(cnt, cnt + 26 - k, 0);
        }
    };
}

namespace s2279m1
{   // 注意：答案要求的是装满的袋子的数量，与s3074不同
    class Solution {
    public:
        int maximumBags(vector<int>& capacity, vector<int>& rocks, int additionalRocks) {
            int n = capacity.size();
            vector<int> diff(n);
            for (int i = 0; i < n; ++i) {
                diff[i] = capacity[i] - rocks[i];
            }
            sort(diff.begin(), diff.end());
            int ans = 0;
            while (ans < n && additionalRocks >= diff[ans]) {// 必须加ans < n的判断，因为additionalRocks可能很大
                // 没有说多余的石头必须要用完
                additionalRocks -= diff[ans++];
            }
            return ans;
        }
    };
}

namespace s1833m1
{   // 排序 + 贪心
    class Solution {
    public:
        int maxIceCream(vector<int>& costs, int coins) {
            sort(costs.begin(), costs.end());
            int cnt = 0;
            for (int i = 0; i < costs.size(); ++i) {
                coins -= costs[i];
                if (coins >= 0) {
                    ++cnt;
                }
                else {
                    break;
                }
            }
            return cnt;
        }
    };
}
namespace s1833o1
{   // 计数排序
    class Solution {
    public:
        int maxIceCream(vector<int>& costs, int coins) {
            vector<int> freq(100001, 0);// 大容量数组还是老老实实分配到堆上用vector，别用array或是c风格
            for (int i : costs) {
                ++freq[i];
            }
            int cnt = 0;
            for (int i = 1; i <= 1e5; ++i) {
                if (coins >= i) {
                    int curCnt = min(freq[i], coins / i);
                    cnt += curCnt;
                    coins -= i * curCnt;
                }
                else {
                    break;
                }
            }
            return cnt;
        }
    };
}

namespace s1005m1
{   // 痛苦面具写法，折腾半天的条件判断，要考虑每种情况
    class Solution {
    public:
        int largestSumAfterKNegations(vector<int>& nums, int k) {
            sort(nums.begin(), nums.end());
            int tmp = -101, sub = 0;
            for (int i = 0; i < nums.size() && k > 0; ++i) {
                if (nums[i] == 0) {
                    k = 0;
                    break;
                }
                if (nums[i] < 0) {
                    tmp = max(tmp, nums[i]);
                    nums[i] = -nums[i];
                    --k;
                }
                else {
                    if (k % 2 == 1) {
                        sub = min(-tmp, nums[i]);
                        k = 0;
                        break;
                    }
                    else {
                        k = 0;
                        break;
                    }
                }
            }
            if (k > 0 && k % 2 == 1) {
                sub = -tmp;
            }
            int sum = accumulate(nums.begin(), nums.end(), 0);
            return sum - 2 * sub;
        }
    };
}
namespace s1005o1
{   // AI优化之后的结果，太简洁了
    class Solution {
    public:
        int largestSumAfterKNegations(vector<int>& nums, int k) {
            sort(nums.begin(), nums.end());
            // 先处理所有的负数
            for (int i = 0; i < nums.size() && k > 0 && nums[i] < 0; ++i) {
                nums[i] = -nums[i];
                k--;
            }
            int sum = accumulate(nums.begin(), nums.end(), 0);
            // 如果还有剩余的k是奇数，就减去两倍的最小元素
            if (k % 2 == 1) {
                // 找到当前最小的元素
                int min_num = *min_element(nums.begin(), nums.end());
                sum -= 2 * min_num;
            }
            return sum;
        }
    };
}

namespace s1481m1
{   // 先生成频次哈希表，然后将频次储存至vector中并排序
    class Solution {
    public:
        int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
            unordered_map<int, int> freq;
            for (int num : arr) {
                ++freq[num];
            }
            vector<int> vec;
            for (auto& p : freq) {
                vec.push_back(p.second);
            }
            sort(vec.begin(), vec.end());
            int cnt = 0;
            for (int i : vec) {
                if (k >= i) {
                    k -= i;
                    ++cnt;
                }
                else {
                    break;
                }
            }
            return vec.size() - cnt;
        }
    };
}
namespace s1481o1
{   // 用整个hashmap去初始化vector<pair>，占用的内存和效率不见得比我的好
    class Solution {
    public:
        int findLeastNumOfUniqueInts(vector<int>& arr, int k) {
            unordered_map<int, int> freq;
            for (int num : arr) {
                ++freq[num];
            }
            vector<pair<int, int>> vec(freq.begin(), freq.end());
            sort(vec.begin(), vec.end(), [](const auto& u, const auto& v) {return u.second < v.second; });
            int ans = vec.size();
            for (auto& p : vec) {
                if (k >= p.second) {
                    k -= p.second;
                    --ans;
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

namespace s1403m1
{
    class Solution {
    public:
        vector<int> minSubsequence(vector<int>& nums) {
            // 返回的答案按 非递增顺序排列
            sort(nums.begin(), nums.end(), greater<int>());
            int sum = accumulate(nums.begin(), nums.end(), 0);
            int target = sum / 2;
            vector<int> ans;
            for (int i = 0; i < nums.size(); ++i) {
                if (target >= 0) {
                    target -= nums[i];
                    ans.push_back(nums[i]);
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

namespace s3010m1
{   // nums[0]必选，在剩下的元素里找两个最小的
    class Solution {
    public:
        int minimumCost(vector<int>& nums) {
            int min1 = INT_MAX, min2 = INT_MAX;
            for (int i = 1; i < nums.size(); ++i) {
                if (nums[i] < min1) {
                    min2 = min1;
                    min1 = nums[i];
                }
                else {
                    min2 = min(min2, nums[i]);
                }
            }
            return min1 + min2 + nums[0];
        }
    };
}

namespace s1338m1
{   // 计数 + 贪心
    class Solution {
    public:
        int minSetSize(vector<int>& arr) {
            unordered_map<int, int> freq;
            for (int i : arr) {
                ++freq[i];
            }
            vector<int> vec;
            for (auto& p : freq) {
                vec.push_back(p.second);
            }
            sort(vec.begin(), vec.end(), greater<int>());
            int target = arr.size() / 2;
            int ans = 0;
            for (int i = 0; i < arr.size(); ++i) {
                if (target > 0) {
                    ++ans;
                    target -= vec[i];
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

namespace s1710m1
{
    class Solution {
    public:
        int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
            sort(boxTypes.begin(), boxTypes.end(),
                [&](const auto& u, const auto& v) {return u[1] > v[1]; });
            int cnt = 0;
            int tmpUnit = 0;
            for (auto& p : boxTypes) {
                if (truckSize >= p[0]) {
                    truckSize -= p[0];
                    cnt += p[0] * p[1];
                }
                else {
                    tmpUnit = p[1];
                    break;
                }
            }
            cnt += tmpUnit * (truckSize);
            return cnt;
        }
    };
}

namespace s3075m1
{
    class Solution {
    public:
        long long maximumHappinessSum(vector<int>& happiness, int k) {
            sort(happiness.begin(), happiness.end(), greater<int>());
            int sub = 0; long long ans = 0;
            for (int i = 0; i < happiness.size(); ++i) {
                if (k > 0 && happiness[i] - sub >= 0) {
                    ans += happiness[i] - sub;
                    ++sub;
                    --k;
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

namespace s2554m1
{
    class Solution {
    public:
        int maxCount(vector<int>& banned, int n, int maxSum) {
            unordered_set<int> banSet;
            for (int i : banned) {
                banSet.insert(i);
            }
            int sum = 0, ans = 0;
            for (int i = 1; i <= n; ++i) {
                if (banSet.find(i) == banSet.end()) {
                    if (sum + i <= maxSum) {
                        sum += i;
                        ++ans;
                    }
                    else {
                        break;
                    }
                }
            }
            return ans;
        }
    };
}

namespace s2126m1
{
    class Solution {
    public:
        bool asteroidsDestroyed(int mass, vector<int>& asteroids) {
            sort(asteroids.begin(), asteroids.end());
            long long newMass = mass;
            for (int i = 0; i < asteroids.size(); ++i) {
                if (newMass >= asteroids[i]) {
                    newMass += asteroids[i];
                }
                else {
                    return false;
                }
            }
            return true;
        }
    };
}

namespace s2587m1
{
    class Solution {
    public:
        int maxScore(vector<int>& nums) {
            sort(nums.begin(), nums.end(), greater<int>());
            int ans = 0, n = nums.size();
            long long sum = 0;
            for (int i = 0; i < n; ++i) {
                sum += nums[i];
                if (sum > 0) {
                    ++ans;
                }
                else {
                    break;
                }
            }
            return ans;
        }
    };
}

namespace s976m1
{
    class Solution {
    public:
        int largestPerimeter(vector<int>& nums) {
            // ans = a + b + c
            // a + b > c
            // 无满足情况的，返回0
            sort(nums.begin(), nums.end(), greater<int>());
            int a = 0, b = 0, c = 0;
            for (int i = 0; i < nums.size() - 2; ++i) {
                int c = nums[i];
                int j = i + 1, k = j + 1;
                int b = nums[j], a = nums[k];
                if (a + b > c) {
                    return a + b + c;
                }
            }
            return 0;
        }
    };
}

namespace s1561m1
{   // 排序后固定拿每堆的第二个即可，bob从堆尾拿
    class Solution {
    public:
        int maxCoins(vector<int>& piles) {
            sort(piles.begin(), piles.end(), greater<int>());
            int n = piles.size() / 3, ans = 0;
            for (int i = 1; i < piles.size() - n; i = i + 2) {
                ans += piles[i];
            }
            return ans;
        }
    };
}

namespace s3462m1
{   // 先每行内进行排序，再把每行limits内的元素取出来组成一个大数组并排序，取前k个元素的和即为答案
    class Solution {
    public:
        long long maxSum(vector<vector<int>>& grid, vector<int>& limits, int k) {
            int n = grid.size();
            vector<int> allEle;
            for (int i = 0; i < n; ++i) {
                vector<int>& row = grid[i];
                sort(row.begin(), row.end(), greater<int>());
                for (int j = 0; j < limits[i]; ++j) {
                    allEle.push_back(row[j]);
                }
                // for 循环可以简化为：
                // allEle.insert(allEle.end(), row.begin(), row.begin() + limits[i]);
            }
            sort(allEle.begin(), allEle.end(), greater<int>());
            return accumulate(allEle.begin(), allEle.begin() + k, 0LL);
        }
    };
}

namespace s2099m1
{
    class Solution {
    public:
        vector<int> maxSubsequence(vector<int>& nums, int k) {
            unordered_map<int, int> freq;
            vector<int> copy = nums;
            sort(copy.begin(), copy.end(), greater<int>());
            for (int i = 0; i < k; ++i) {
                ++freq[copy[i]];
            }
            vector<int> ans(k);
            int j = 0;
            for (int i = 0; i < nums.size(); ++i) {
                if (freq.find(nums[i]) != freq.end()) {
                    ans[j++] = nums[i];
                    if (--freq[nums[i]] == 0) {
                        freq.erase(nums[i]);
                    }
                    if (j == k) return ans;
                }
            }
            return ans;
        }
    };
}

namespace s3301m1
{   // 写法稍微比o1繁琐了些
    class Solution {
    public:
        long long maximumTotalSum(vector<int>& maximumHeight) {
            sort(maximumHeight.begin(), maximumHeight.end(), greater<int>());
            long long sum = 0;
            int pre = INT_MAX;
            for (int i = 0; i < maximumHeight.size(); ++i) {
                int height = maximumHeight[i];
                if (height >= pre) {
                    height = pre - 1;
                    if (height == 0) return -1;
                }
                pre = height;
                sum += height;
            }
            return sum;
        }
    };
}
namespace s3301o1
{
    class Solution {
    public:
        long long maximumTotalSum(vector<int>& maximumHeight) {
            sort(maximumHeight.begin(), maximumHeight.end(), greater<int>());
            long long sum = maximumHeight[0];
            for (int i = 1; i < maximumHeight.size(); ++i) {
                maximumHeight[i] = min(maximumHeight[i], maximumHeight[i - 1] - 1);
                if (maximumHeight[i] == 0) return -1;
                sum += maximumHeight[i];
            }
            return sum;
        }
    };
}

// 和s3301类似
namespace s945m1
{
    class Solution {
    public:
        int minIncrementForUnique(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int ans = 0, n = nums.size();
            if (n == 1) return 0;
            for (int i = 1; i < nums.size(); ++i) {
                int tmp = nums[i];
                nums[i] = max(nums[i], nums[i - 1] + 1);
                ans += nums[i] - tmp;
            }
            return ans;
        }
    };
}

namespace s1846m1
{
    class Solution {
    public:
        int maximumElementAfterDecrementingAndRearranging(vector<int>& arr) {
            sort(arr.begin(), arr.end());
            int n = arr.size();
            if (n == 1) return 1;
            arr[0] = 1;
            for (int i = 1; i < n; ++i) {
                arr[i] = min(arr[i - 1] + 1, arr[i]);
            }
            return arr[n - 1];
        }
    };
}

namespace s1647m1
{
    class Solution {
    public:
        int minDeletions(string s) {
            vector<int> freq(26, 0);
            for (char c : s) {
                ++freq[c - 'a'];
            }
            sort(freq.begin(), freq.end(), greater<int>());
            int ans = 0;
            for (int i = 1; i < 26; ++i) {
                if (freq[i] >= freq[i - 1]) {
                    int temp = freq[i];
                    freq[i] = max(freq[i - 1] - 1, 0);
                    ans += temp - freq[i];
                }
            }
            return ans;
        }
    };
}

namespace s2971m1
{
    class Solution {
    public:
        long long largestPerimeter(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            long long sum = nums[0], ans = -1;
            for (int i = 1; i < nums.size() - 1; ++i) {
                sum += nums[i];
                if (sum > nums[i + 1]) {
                    ans = sum + nums[i + 1];
                }
            }
            return ans;
        }
    };
}

namespace s2178m1
{
    class Solution {
    public:
        vector<long long> maximumEvenSplit(long long finalSum) {
            // 奇数
            vector<long long> ans;
            if (finalSum % 2) return ans;
            // 偶数
            if (finalSum == 2) return{ 2 };
            long long sum = 0, i = 1;
            while (true) {
                sum += 2 * i;
                if (sum < finalSum) {
                    ans.push_back(2 * i);
                }
                else {
                    long long temp = finalSum - sum + 2 * i;
                    if (temp <= ans.back()) {
                        ans.back() = ans.back() + temp;
                    }
                    else {
                        ans.push_back(temp);
                    }
                    break;
                }
                ++i;
            }
            return ans;
        }
    };
}
namespace s2178o1
{   // 思路和m1一样，当时写法更简洁，省去了很多不必要的判断
    class Solution {
    public:
        vector<long long> maximumEvenSplit(long long finalSum) {
            // 奇数
            vector<long long> ans;
            if (finalSum % 2) return ans;
            // 偶数
            if (finalSum == 2) return{ 2 };
            for (long long i = 2; i <= finalSum; i += 2) {
                ans.push_back(i);
                finalSum -= i;
            }
            ans.back() += finalSum;
            return ans;
        }
    };
}

namespace s2567m1
{   // 只有三种情况
    class Solution {
    public:
        int minimizeSum(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int n = nums.size();
            int a = nums[n - 2] - nums[1];
            int b = nums[n - 1] - nums[2];
            int c = nums[n - 3] - nums[0];
            return min({ a, b, c });
        }
    };
}

// 与s2567一致
namespace s1509m1
{
    class Solution {
    public:
        int minDifference(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int n = nums.size();
            if (n <= 4) return 0;
            int a = nums[n - 3] - nums[1];
            int b = nums[n - 2] - nums[2];
            int c = nums[n - 1] - nums[3];
            int d = nums[n - 4] - nums[0];
            return min({ a, b, c, d });
        }
    };
}

// clamp函数的使用
namespace s3397o1
{
    class Solution {
    public:
        int maxDistinctElements(vector<int>& nums, int k) {
            int n = nums.size();
            if (2 * k + 1 >= n) {
                return n;
            }
            sort(nums.begin(), nums.end());
            int ans = 0;
            int pre = INT_MIN; // 记录每个人左边的人的位置
            for (int x : nums) {
                //x = clamp(pre + 1, x - k, x + k); // since c++17
                x = min(max(x - k, pre + 1), x + k);
                if (x > pre) {
                    ans++;
                    pre = x;
                }
            }
            return ans;
        }
    };
}

// 与s1561不同，更复杂
namespace s3457m1
{   // 先把最重的披萨全部留给奇数天
    // 思考题：如果是求最小重量，思路则是先三个三个地分配小的给偶数天（Z 取最大的数），然后再四个四个地分给奇数天
    class Solution {
    public:
        long long maxWeight(vector<int>& pizzas) {
            sort(pizzas.begin(), pizzas.end(), greater<int>());
            long long ans = 0;
            int n = pizzas.size();
            int days = n / 4;
            int odd = (days + 1) / 2;
            int even = days / 2;
            for (int i = 0; i < odd; ++i) {
                ans += pizzas[i];
            }
            for (int i = 0; i < even; ++i) {
                ans += pizzas[odd + 2 * i + 1];
            }
            return ans;
        }
    };
}

// 反悔贪心 + 判断两个数奇偶性不同 + 快速选择排序
namespace ls40m1
{   // 先排序，然后将元素按奇偶分离开，各自两两加入ans中，确保每次加入都是偶数
    // 如果cnt是奇数，那预先在偶数列中选一个
    class Solution {
    public:
        int maximumScore(vector<int>& cards, int cnt) {
            int n = cards.size();
            sort(cards.begin(), cards.end(), greater<int>());
            vector<int> odd;
            vector<int> even;
            int ans = 0;
            // 可以vector<int> a[2], a[i % 2].push_back，来简化代码
            for (int i : cards) {
                if (i % 2) {
                    odd.push_back(i);
                }
                else {
                    even.push_back(i);
                }
            }
            int indexOdd = 0, sumOdd = 0;
            int indexEven = 0, sumEven = 0;
            if (cnt % 2) {
                if (even.size() == 0) return 0;
                ans += even[0];
                indexEven = 1;
                cnt--;
            }
            int loop = cnt / 2;
            while (loop--) {
                if (indexOdd + 1 < odd.size()) {
                    sumOdd = odd[indexOdd] + odd[indexOdd + 1];
                }
                if (indexEven + 1 < even.size()) {
                    sumEven = even[indexEven] + even[indexEven + 1];
                }
                if (sumEven == 0 && sumOdd == 0) {
                    return 0;// cards = {1, 3, 4, 5}, cnt = 4
                }
                if (sumOdd > sumEven) {
                    ans += sumOdd;
                    indexOdd += 2;
                }
                else {
                    ans += sumEven;
                    indexEven += 2;
                }
                sumOdd = 0, sumEven = 0;
            }
            return ans;
        }
    };
}
namespace ls40o1
{   // 这个方法和m1效率差不多，也是nlogn
    // 如果前cnt个元素的和是奇数，那么需要：
    // 从前cnt个元素中去除掉一个最小的奇数或偶数，并从后n - cnt个元素中加进来一个与去掉元素奇偶性相反的最大的数
    // 最多有两种情况，两种情况取最大的ans进行返回
    // 细节：前cnt个数中，x = card[cnt - 1]一定是最小的奇数或偶数，那么剩下一种情况就是前cnt中与x奇偶性不同的最小的数
    class Solution {
    public:
        int maximumScore(vector<int>& cards, int cnt) {
            sort(cards.begin(), cards.end(), greater<int>());
            int s = accumulate(cards.begin(), cards.begin() + cnt, 0); // 最大的 cnt 个数之和
            if (s % 2 == 0) { // s 是偶数
                return s;
            }
            auto replaced_sum = [&](int x) -> int {
                for (int i = cnt; i < cards.size(); i++) {
                    if (cards[i] % 2 != x % 2) { // 找到一个最大的奇偶性和 x 不同的数
                        return s - x + cards[i]; // 用 cards[i] 替换 s
                    }
                }
                return 0;
                };
            int x = cards[cnt - 1];
            int ans = replaced_sum(x); // 替换 x
            for (int i = cnt - 2; i >= 0; i--) { // 前 cnt-1 个数
                if (cards[i] % 2 != x % 2) { // 找到一个最小的奇偶性和 x 不同的数
                    ans = max(ans, replaced_sum(cards[i])); // 替换
                    break;
                }
            }
            return ans;
        }
    };
}
namespace ls40o2 
{
    // 通过快速选择排序，将时间复杂度降为O(n)
    class Solution {
    public:
        int maximumScore(vector<int>& cards, int cnt) {
            nth_element(cards.begin(), cards.end(),cards.end() - cnt); // 快速选择
            int s = accumulate(cards.end() - cnt, cards.end(), 0); // 最大的 cnt 个数之和
            if (s % 2 == 0) { // s 是偶数
                return s;
            }

            int n = cards.size();
            // 加进来的最大偶数/奇数
            int mx[2] = { INT_MIN / 2, INT_MIN / 2 }; // 除 2 防止最下面减法溢出
            for (int i = 0; i < n - cnt; i++) {
                int v = cards[i];
                mx[v % 2] = max(mx[v % 2], v);
            }

            // 要去掉的最小偶数/奇数
            int mn[2] = { INT_MAX / 2, INT_MAX / 2 };
            for (int i = n - cnt; i < n; i++) {
                int v = cards[i];
                mn[v % 2] = min(mn[v % 2], v);
            }

            return max(s + max(mx[0] - mn[1], mx[1] - mn[0]), 0);
        }
    };
}

// 与ls40类似，但更复杂一点，o1为分类讨论+贪心，o2为动态规划
namespace s1262o1
{   // 反悔贪心
    class Solution {
    public:
        int maxSumDivThree(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 3 == 0) return sum;
            sort(nums.begin(), nums.end());
            vector<int> a[3];
            for (int x : nums) {
                a[x % 3].push_back(x);
            }
            if (sum % 3 == 2) {
                swap(a[1], a[2]);// 代码复用，且只交换内部指针，swap对vector的时间复杂度为O(1)
            }
            int sum1 = 0, sum2 = 0;
            if (a[1].size() != 0) {
                sum1 = sum - a[1][0];
            }
            if (a[2].size() >= 2) {
                sum2 = sum - a[2][0] - a[2][1];
            }
            return max(sum1, sum2);
        }
    };
}
namespace s1262o2 
{
//https://leetcode.cn/problems/greatest-sum-divisible-by-three/solutions/2313700/liang-chong-suan-fa-tan-xin-dong-tai-gui-tsll/
}

namespace s948m1
{   // 用尽可能少的能量换1分，用1分换尽可能多的能量
    // 先花光所有能量，然后比较当前没换分的能量最小牌和队尾牌的能量大小，如果左手倒右手能白嫖能量就换，不行就拉倒
    class Solution {
    public:
        int bagOfTokensScore(vector<int>& tokens, int power) {
            // 牌可以不全部用完，只用一部分
            int n = tokens.size();
            if (n == 0) return 0;// 可能没有元素，此时访问tokens[0]会报错
            // if (tokens.empty()) return 0; //用这句更好
            int ans = 0;
            sort(tokens.begin(), tokens.end());
            int left = 0, right = n - 1;
            while (left < n && power - tokens[left] >= 0) {// 注意要加left < n防止越界
                power -= tokens[left];
                ++ans, ++left;
            }
            if (ans == 0) return 0;// 初始power连1分都换不到，没有操作空间，直接返回0
            while (left < right) {// 这个循环中ans只会增加
                if (tokens[right] > tokens[left]) {
                    power += tokens[right] - tokens[left];
                    ++left, --right;
                }
                else {
                    break;// 队尾最大的牌的分都不比现在正向牌的大，没有操作空间了
                }
                while (power - tokens[left] >= 0) {
                    power -= tokens[left];
                    ++ans, ++left;
                }
            }
            return ans;
        }
    };
}

// 枚举右，维护左（还不太懂什么是左和右，但这道题的题解能看懂）
namespace s624o1
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
// ---------------------
// 【1.2】单序列匹配(5)
// 同上，从最大/最小值处开始贪心
/*

*/
// ---------------------
namespace s2144m1
{
    class Solution {
    public:
        int minimumCost(vector<int>& cost) {
            sort(cost.begin(), cost.end(), greater<int>());
            int ans = 0, n = cost.size();
            int loop = n / 3, index = 0;
            while (loop--) {
                ans += cost[index] + cost[index + 1];
                index += 3;
            }
            for (int i = index; i < n; ++i) {
                ans += cost[i];
            }
            return ans;
        }
    };
}

namespace s561m1
{
    class Solution {
    public:
        int arrayPairSum(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int ans = 0;
            for (int i = 0; i < nums.size(); i += 2) {
                ans += nums[i];
            }
            return ans;
        }
    };
}

namespace s1877m1
{
    class Solution {
    public:
        int minPairSum(vector<int>& nums) {
            int ans = 0;
            sort(nums.begin(), nums.end());
            int left = 0, right = nums.size() - 1;
            while (left < right) {
                ans = max(ans, nums[left] + nums[right]);
                ++left, --right;
            }
            return ans;
        }
    };
}

namespace s881m1
{   // 思路是对的，但写复杂了（直接正向求解就够了）
    class Solution {
    public:
        int numRescueBoats(vector<int>& people, int limit) {
            sort(people.begin(), people.end(), greater<int>());
            int n = people.size();
            int left = 0, right = n - 1;
            int ans = 0;
            while (people[left] >= limit) {
                ++ans, ++left;
            }
            while (left <= right) {
                if (people[left] + people[right] <= limit) {
                    --right;
                }
                ++ans, ++left;
            }
            return ans;// 就算最后left = right,进循环后不管怎么样都会ans + 1，答案不会受到影响
        }
    };
}
namespace s881o1
{
    class Solution {
    public:
        int numRescueBoats(vector<int>& people, int limit) {
            sort(people.begin(), people.end());
            int left = 0, right = people.size() - 1;
            int ans = 0;
            while (left <= right) {
                if (people[left] + people[right] <= limit) {
                    ++left;
                }
                --right;
                ++ans;
            }
            return ans;// 就算最后left = right,进循环后不管怎么样都会ans + 1，答案不会受到影响
        }
    };
}

// o1为O(nlogn)做法，常规贪心思路
// o2为O(n)做法，田忌赛马类型，找最大的错开值，双指针之间的距离的最大值
namespace s2592m1
{   // 错位排序贪心 + 双指针，nlogn，常规思路，但写法比较乱
    class Solution {
    public:
        int maximizeGreatness(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int left = 0, right = 0;
            int ans = 0, n = nums.size();
            while (right < n) {
                while (right < n && nums[right] <= nums[left]) {
                    ++right;
                }
                if (right == n)
                    break;
                ++left, ++right, ++ans;
            }
            return ans;
        }
    };
}
namespace s2592o1
{   // m1思路的简洁写法
    class Solution {
    public:
        int maximizeGreatness(vector<int>& nums) {
            sort(nums.begin(), nums.end());
            int i = 0;
            for (int x : nums)
                if (x > nums[i])
                    ++i;
            return i;
        }
    };
}
namespace s2592o2
{   // O(n)做法，不要求掌握，能熟练掌握o1做法已经ok
    // 碰到类似的题目可以尝试直接用最高频次来做，照猫画虎
    class Solution {
    public:
        int maximizeGreatness(vector<int>& nums) {
            int mx = 0;
            unordered_map<int, int> cnt;
            for (int x : nums)
                mx = max(mx, ++cnt[x]);// 找出现频次最高的数字，其频次就是mx
            return nums.size() - mx;
        }
    };
}
// ---------------------
// 【1.3】双序列配对(5)
// 思路同上，但是输入为两个序列
/*

*/
// ---------------------
namespace s2037m1
{
    class Solution {
    public:
        int minMovesToSeat(vector<int>& seats, vector<int>& students) {
            sort(seats.begin(), seats.end());
            sort(students.begin(), students.end());
            int ans = 0;
            for (int i = 0; i < seats.size(); ++i) {
                ans += abs(seats[i] - students[i]);
            }
            return ans;
        }
    };
}

// 与s2592的简洁写法类似，答案与2410一致
namespace s455m1
{
    class Solution {
    public:
        int findContentChildren(vector<int>& g, vector<int>& s) {
            sort(g.begin(), g.end());
            sort(s.begin(), s.end());
            int i = 0, n = g.size();;
            for (int cookie : s) {
                if (cookie >= g[i]) {
                    ++i;

                }
                if (i == n) break;
            }
            return i;
        }
    };
}

namespace s1433m1
{
    class Solution {
    public:
        bool checkIfCanBreak(string s1, string s2) {
            sort(s1.begin(), s1.end());
            sort(s2.begin(), s2.end());
            int equalCnt = 0, bigCnt = 0;
            int n = s1.size();
            for (int i = 0; i < n; ++i) {
                if (s1[i] == s2[i]) {
                    ++equalCnt;
                }
                else if (s1[i] > s2[i]) {
                    ++bigCnt;
                }
            }
            return (bigCnt + equalCnt == n) || (bigCnt == 0);
        }
    };
}

// 模板题1，田忌赛马/优势发牌，对于不能排序的情况，下标数组（lambda自定义比较规则，比较其他对象，相当于投影）的运用
namespace s870o1
{
    class Solution {
    public:
        // 灵神的题解用的全是c++20中的ranges版本
        vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
             sort(nums1.begin(), nums1.end());
            int n = nums1.size();
            vector<int> idx(n);
            // C++14及以下替代ranges::iota(idx, 0);
            iota(idx.begin(), idx.end(), 0);

            // 替代ranges::sort(idx, {}, [&](int i) { return nums2[i]; });
            // 中间{}代表默认的升序排序，不省略就是std::less()
            // C++14及以下使用标准sort + lambda作为比较函数
            sort(idx.begin(), idx.end(), [&](int a, int b) {
                return nums2[a] < nums2[b];
                });

            vector<int> ans(n);
            int left = 0, right = n - 1;
            for (int x : nums1) {
                // 为每个num1元素确定序号，为确保答案正确需要贪心地对num1进行排序后再遍历
                int i = x > nums2[idx[left]] ? idx[left++] : idx[right--];
                ans[i] = x;
            }
            return ans;
        }
    };
}

// m1：下标数组技巧练习
// o1：用pair绑定两个数组，时间复杂度和m1相同，但这个技巧也值得学习
namespace s826m1
{   // 创建下标数组idx，按照profit大小升序排列，然后从idx队尾来依次检查当前能力最强工人是否能完成工作
    class Solution {
    public:
        int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
            // 分配难度最低，且收益最大的工作
            int n = profit.size();
            sort(worker.begin(), worker.end(), greater<int>());
            vector<int> idx(n);
            iota(idx.begin(), idx.end(), 0);
            sort(idx.begin(), idx.end(),
                [&](int a, int b) {
                    return profit[a] < profit[b];
                });
            int right = n - 1;
            int ans = 0;
            for (int man : worker) {
                while (right >= 0 && man < difficulty[idx[right]]) {
                    --right;
                }
                if (right == -1) break;
                ans += profit[idx[right]];
            }
            return ans;
        }
    };
}
namespace s826o1
{
    class Solution {
    public:
        int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
            int n = difficulty.size();
            vector<pair<int, int>> jobs(n);
            for (int i = 0; i < n; i++) {
                jobs[i] = { difficulty[i], profit[i] };
            }
            sort(jobs.begin(), jobs.end());
            sort(worker.begin(), worker.end());
            int ans = 0, j = 0, max_profit = 0;
            for (int w : worker) {
                while (j < n && jobs[j].first <= w) {
                    max_profit = max(max_profit, jobs[j++].second);
                }
                ans += max_profit;
            }
            return ans;
        }
    };
}
// ---------------------
// 【1.4】从最左/最右开始贪心(14)
// 对于无法排序的题目，尝试从左到右/从右到左贪心。
// 思考第一个数/最后一个数的贪心策略，把 n 个数的原问题转换成 n-1 个数（或更少）的子问题。
// 之后复刷可以与线性DP、状态机DP对比，判断何时只能贪心不能DP，加深对「局部最优」和「全局最优」的理解
/*

*/
// ---------------------
namespace s3402m1
{
    class Solution {
    public:
        int minimumOperations(vector<vector<int>>& grid) {
            int m = grid.size();
            int n = grid[0].size();
            int ans = 0;
            for (int j = 0; j < n; ++j) {
                for (int i = 1; i < m; ++i) {
                    int temp = grid[i][j];
                    if (temp > grid[i - 1][j]) {
                        continue;
                    }
                    grid[i][j] = grid[i - 1][j] + 1;
                    ans += grid[i][j] - temp;
                }
            }
            return ans;
        }
    };
}

// 模板题2，将问题n转化为问题n-1
namespace s3191m1
{   // nums[0]如果是0，想要变成1就只有将头三个一起反转，之后不管nums[0]，问题简化
    class Solution {
    public:
        int minOperations(vector<int>& nums) {
            // 三个元素必须是连续的，且要一次性全部反转
            int ans = 0;
            int n = nums.size();
            for (int i = 0; i < n - 2; ++i) {
                if (nums[i] == 0) {
                    // nums[i] ^= 1; // 后面的不会遍历到nums[i]了，所以当前的元素不用改
                    nums[i + 1] ^= 1;
                    nums[i + 2] ^= 1;
                    ++ans;
                }
            }
            return (nums[n - 1] == 1 && nums[n - 2] == 1) ? ans : -1;
        }
    };
}

namespace s1827m1
{
    class Solution {
    public:
        int minOperations(vector<int>& nums) {
            int ans = 0;
            int n = nums.size();
            if (n == 1) return 0;
            for (int i = 1; i < n; ++i) {
                if (nums[i] <= nums[i - 1]) {
                    int temp = nums[i];
                    nums[i] = nums[i - 1] + 1;
                    ans += nums[i] - temp;
                }
            }
            return ans;
        }
    };
}

// 和s3191类似
namespace s2027m1
{
    class Solution {
    public:
        int minimumMoves(string s) {
            int n = s.size();
            int ans = 0;
            for (int i = 0; i < n - 2; ++i) {
                if (s[i] == 'X') {
                    s[i] = 'O';
                    s[i + 1] = 'O';
                    s[i + 2] = 'O';
                    ++ans;
                }
            }
            return (s[n - 1] == 'X' || s[n - 2] == 'X') ? ans + 1 : ans;
        }
    };
}

// 不难，但是坑很多，很容易错
namespace s605m1
{   // 每次跳过两个元素，只需要管右边
    class Solution {
    public:
        bool canPlaceFlowers(vector<int>& flowerbed, int n) {
            int m = flowerbed.size();
            int i = 0;
            for (; i < m - 1; i += 2) {
                if (flowerbed[i] == 0) {
                    if (flowerbed[i + 1] == 0) {
                        flowerbed[i] = 1;
                        --n;
                    }
                    else {
                        ++i;
                    }
                }
            }
            if (i == m - 1 && flowerbed.back() == 0) --n;
            return n <= 0;
        }
    };
}
namespace s605o1
{   // 前后补一个0，简单粗暴
    class Solution {
    public:
        bool canPlaceFlowers(vector<int>& flowerbed, int n) {
            flowerbed.insert(flowerbed.begin(), 0);
            flowerbed.push_back(0);
            for (int i = 1; i + 1 < flowerbed.size(); i++) {
                if (flowerbed[i - 1] == 0 && flowerbed[i] == 0 && flowerbed[i + 1] == 0) {
                    flowerbed[i] = 1; // 种花！
                    n--;
                }
            }
            return n <= 0;
        }
    };
}

// m1：需要额外复制nums两次，比较耗时（且无需对当前元素乘-1，后面不会再遍历到了）
// o2：不需要复制nums，用mul乘数表示对下一个元素的效果
namespace s3576m1
{
    // check函数因为要修改nums，所以要复制一份，拖慢了整体效率
    class Solution {
    public:
        bool canMakeEqual(vector<int>& nums, int k) {
            int n = nums.size();
            if (n == 1)
                return true;
            auto check = [=](int x) -> bool {
                vector<int> vec = nums;
                int m = k;
                int i = 0;
                for (; i < n - 1; ++i) {
                    if (vec[i] == -x) {
                        vec[i] *= -1;
                        vec[i + 1] *= -1;
                        if (--m == 0) break;
                    }
                }
                for (; i < n; ++i) {
                    if (vec[i] == -x) {
                        return false;
                    }
                }
                return true;
                };
            return check(1) || check(-1);
        }
    };
}
namespace s3576o1
{   
    class Solution {
    public:
        bool canMakeEqual(vector<int>& nums, int k) {
            int n = nums.size();
            if (n == 1)
                return true;
            auto check = [&](int x) -> bool {
                int i = 0;
                int mul = 1;
                int m = k;
                for (; i < n; ++i) {
                    if (nums[i] * mul == x) {
                        mul = 1;
                        continue;
                    }
                    // 判断m和i的时机和m1也有所不同
                    if (m == 0 || i + 1 == n) return false;
                    mul = -1;
                    --m;

                }
                return true;
                };
            return check(1) || check(-1);
        }
    };
}

namespace s3111m1
{
    class Solution {
    public:
        int minRectanglesToCoverPoints(vector<vector<int>>& points, int w) {   
            // 可以是非矩形，退化成一条竖线，矩形的宽(x2 - x1) <= w
            // 在矩形的边上也算覆盖
            // 1.策略：全部用最宽的矩形来覆盖，高度没有限制
            // 2.按照x大小对points排序
            // 3.xmin - xmin + w的区间，ans + 1
            // 4.下一个点的更新为xmin，重复，直到没有剩余的点
            sort(points.begin(), points.end(),
                [&](const auto& a, const auto& b) {
                    return a[0] < b[0];
                });
            int n = points.size(), ans = 1;
            int xmin = points[0][0];
            for (int i = 0; i < n; ++i) {
                vector<int>& p = points[i];
                if (p[0] > xmin + w) {
                    xmin = p[0];
                    ++ans;
                }
            }
            return ans;
        }
    };
}

// 竟然这个没自己写出来，还需要继续变强啊...
namespace s2957o1
{
    class Solution {
    public:
        int removeAlmostEqualCharacters(string word) {
            int n = word.size(), ans = 0;
            for (int i = 1; i < n; ++i) {
                if (abs(word[i] - word[i - 1]) <= 1) {
                    ++ans;
                    ++i;
                }
            }
            return ans;
        }
    };
}

// 异或与二进制数组练习
namespace s3192m1
{
    class Solution {
    public:
        int minOperations(vector<int>& nums) {
            int mul = 0, n = nums.size();
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                if ((nums[i] ^ mul) == 0) {
                    ++ans;
                    mul ^= 1;// 从s3576中吸取的灵感
                }
            }
            return ans;
        }
    };
}
namespace s3192o1
{   // 直接比较相邻两个元素，最大化异或的作用，比m1要快
    class Solution {
    public:
        int minOperations(vector<int>& nums) {
            int ans = nums[0] ^ 1;
            for (int i = 1; i < nums.size(); i++) {
                ans += nums[i - 1] ^ nums[i];
            }
            return ans;
        }
    };
}
namespace s3192o2
{   // 考虑反转次数的奇偶来做，速度没有o1快
    class Solution {
    public:
        int minOperations(vector<int>& nums) {
            int k = 0;
            for (int x : nums) {
                if (x == k % 2) { // 当反转次数为奇数时，取模为1，此时若x = 1，则意味着已经被反转成了0，还要再反转一次
                    k++;          // 当反转次数为偶数时，取模为0，此时若x = 0，则意味着没有被反转，是0，所以要再反转一次
                }
            }
            return k;
        }
    };
}

namespace s2789m1
{
    class Solution {
    public:
        long long maxArrayValue(vector<int>& nums) {
            long long ans = nums.back();
            int n = nums.size();
            for (int i = n - 2; i >= 0; --i) {
                if (ans >= nums[i]) {
                    ans += nums[i];
                }
                else {
                    ans = nums[i];
                }
            }
            return ans;
        }
    };
}

// 同s3192
namespace s1529m1
{
    class Solution {
    public:
        int minFlips(string target) {
            int ans = (target[0] == '1');
            for (int i = 1; i < target.size(); ++i) {
                ans += target[i] ^ target[i - 1];
            }
            return ans;
        }
    };
}

namespace s1144m1
{   // 纯硬来... 分奇偶讨论，代码量大
    class Solution {
    public:
        int movesToMakeZigzag(vector<int>& nums) {
            int n = nums.size();
            if (n == 2 && nums[0] == nums[1]) return 1;
            // 偶为峰值
            int ans1 = 0, i = 1;
            vector<int> vec = nums;
            for (; i < n - 1; i += 2) {
                if (vec[i] >= vec[i - 1] || vec[i] >= vec[i + 1]) {
                    int temp = vec[i];
                    vec[i] = min(vec[i - 1], vec[i + 1]) - 1;
                    ans1 += temp - vec[i];
                }
                // ans1 += max(0, vec[i] - min(vec[i - 1], vec[i + 1]) + 1); // 从o1中吸取经验，这一段if判断可以改成这样
            }
            if (i == n - 1) {
                if (vec[n - 1] >= vec[n - 2]) {
                    ans1 += vec[n - 1] - vec[n - 2] + 1;
                }
            }
            // 奇为峰值
            int ans2 = 0, j = 1;
            for (; j < n - 1; j += 2) {
                if (nums[j] <= nums[j - 1] || nums[j] <= nums[j + 1]) {
                    int temp1 = nums[j - 1];
                    int temp2 = nums[j + 1];
                    nums[j - 1] = min(nums[j - 1], nums[j] - 1);
                    nums[j + 1] = min(nums[j + 1], nums[j] - 1);
                    ans2 += temp1 - nums[j - 1] + temp2 - nums[j + 1];
                }
            }
            if (j == n - 1) {
                if (nums[n - 1] <= nums[n - 2]) {
                    ans2 += nums[n - 2] - nums[n - 1] + 1;
                }
            }
            return min(ans1, ans2);
        }
    };
}
namespace s1144o1
{   // 其实思路和m1本质上是类似的分类讨论，但写法非常聪明
    class Solution {
    public:
        int movesToMakeZigzag(vector<int>& nums) {
            int n = nums.size();
            int s[2]{};
            for (int i = 0; i < n; ++i) {
                int left = (i ? nums[i - 1] : INT_MAX);// 首尾的处理技巧
                int right = (i == n - 1 ? INT_MAX : nums[i + 1]);
                s[i % 2] += max(0, nums[i] - min(left, right) + 1);
            }
            return min(s[0], s[1]);
        }
    };
}

namespace s3228m1
{   // 有点抽象，但还是凭自己写出来了
    class Solution {
    public:
        int maxOperations(string s) {
            int n = s.size();
            int ans = 0, cnt = 0;
            bool flag = false;
            for (int i = 0; i < n; ++i) {
                if (s[i] == '1') {
                    ++cnt;
                    flag = true;
                }
                else if (flag) {// 必须是已经碰到1的情况下，才能改变结果
                    flag = false;
                    ans += cnt;
                }
            }
            return ans;
        }
    };
}
namespace s3228o1
{   // 思路和我的解法一致
    class Solution {
    public:
        int maxOperations(string s) {
            int n = s.size();
            int ans = 0, cnt = 0;
            for (int i = 0; i < n; ++i) {
                if (s[i] == '1') {
                    ++cnt;
                }
                else if (i != 0 && s[i - 1] == '1') {// 跟我本质上是一样的，但是没我的写法速度快
                    ans += cnt;
                }
            }
            return ans;
        }
    };
}

namespace s2086m1
{   // 硬来，贪心在仓鼠右边放食物
    class Solution {
    public:
        int minimumBuckets(string hamsters) {
            int n = hamsters.size();
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                char left = (i == 0 ? 'H' : hamsters[i - 1]);
                char right = (i == n - 1 ? 'H' : hamsters[i + 1]);
                if (hamsters[i] == 'H') {
                    if (left == 'H' && right == 'H') {
                        return -1;
                    }
                    if (left == 'F') {
                        continue;
                    }
                    if (right == '.') {// left为H或.
                        hamsters[i + 1] = 'F';
                        ++i; ++ans;
                        continue;
                    }
                    if (left == '.' && right == 'H') {
                        ++ans;
                    }
                }
            }
            return ans;
        }
    };
}
namespace s2086m2
{   // m1的优化版
    class Solution {
    public:
        int minimumBuckets(string hamsters) {
            int n = hamsters.size();
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                char left = (i == 0 ? 'H' : hamsters[i - 1]);
                char right = (i == n - 1 ? 'H' : hamsters[i + 1]);
                if (hamsters[i] == 'H') {
                    if (left == 'F') {
                        continue;
                    }
                    else if (right == '.') {
                        hamsters[i + 1] = 'F';
                        ++i; ++ans;
                    }
                    else if (left == '.') {
                        ++ans;
                    }
                    else {
                        return -1;
                    }
                }
            }
            return ans;
        }
    };
}
namespace s2086o1
{   // 灵神写法，速度不一定有我的m2快，整体思路一致，都是贪心在右边放食物
    class Solution {
    public:
        int minimumBuckets(string hamsters) {
            int ans = 0, n = hamsters.size();
            int bucketPos = -2; // 上一个食物的放置位置
            for (int i = 0; i < n; ++i) {
                if (hamsters[i] == 'H') { // 遍历每个仓鼠
                    if (bucketPos == i - 1) {
                        // 左侧已有食物，不做任何处理
                        continue;
                    }
                    else if (i + 1 < n && hamsters[i + 1] == '.') {
                        // 贪心：尽可能在右侧放食物
                        bucketPos = i + 1;
                        ans++;
                    }
                    else if (i > 0 && hamsters[i - 1] == '.') {
                        // 在左侧放食物
                        ans++;
                    }
                    else {
                        // 左右均无空位，无解
                        return -1;
                    }
                }
            }
            return ans;
        }
    };
}

// o1：位运算做法（不要求掌握）
// o2：本题常规做法为DP记忆化搜索（回头看）
namespace s2571o1
{


}
namespace s2571o2
{

}
// ---------------------
// 【1.5】划分型贪心(5)
// 还是从左/右开始进行贪心，思考与DP中的划分型DP的区别
/*

*/
// ---------------------
namespace s1221m1
{
    class Solution {
    public:
        int balancedStringSplit(string s) {
            int cnt = 0, ans = 0;
            for (int i = 0; i < s.size(); ++i) {
                if (s[i] == 'L') {
                    ++cnt;
                }
                else {
                    --cnt;
                }
                if (cnt == 0) ++ans;
            }
            return ans;
        }
    };
}

namespace s2405m1
{
    class Solution {
    public:
        int partitionString(string s) {
            unordered_map<char, int> map;// 也可以用int dic[26]替代
            int ans = 1;
            for (char c : s) {
                ++map[c];
                if (map[c] >= 2) {
                    map.clear();
                    ++ans;
                    ++map[c];
                }
            }
            return ans;
        }
    };
}
namespace s2405m2
{   // 用unordered_set的做法
    class Solution {
    public:
        int partitionString(string s) {
            unordered_set<char> dic;
            int ans = 1;
            for (char c : s) {
                if (dic.find(c) != dic.end()) {
                    dic.clear();
                    ++ans;
                }
                dic.insert(c);
            }
            return ans;
        }
    };
}
namespace s2405o1
{   // 位运算 + O(1)空间
    class Solution {
    public:
        int partitionString(string s) {
            int ans = 1;
            int vis = 0;// vis为掩码，一个bit位代表一个小写字符
            for (char c : s) {
                if ((vis >> (c & 31)) & 1) {// c & 31意味着截取二进制的后5位
                    vis = 0;// 'a'的ascii值为97，后5位正好是1，而'z'是122，后5位正好是26，正好一一对应
                    ans++;  // vis向右移动至当前字符的offset值（c & 31），与1取与来判断vis中是否包含当前字符
                }
                vis |= 1 << (c & 31);// 将当前字符纳入vis中
            }
            return ans;
        }
    };
}

namespace s2294m1
{
    class Solution {
    public:
        int partitionArray(vector<int>& nums, int k) {
            sort(nums.begin(), nums.end());
            int mn = nums[0];
            int ans = 1;
            for (int i = 0; i < nums.size(); ++i) {
                if (nums[i] > mn + k) {
                    mn = nums[i];
                    ++ans;
                }
            }
            return ans;
        }
    };
}

namespace s2358m1
{
    class Solution {
    public:
        int maximumGroups(vector<int>& grades) {
            sort(grades.begin(), grades.end());
            int cnt = 1, ans = 1, cur = 0;
            for (int i = 1; i < grades.size(); ++i) {
                if (++cur > cnt) {
                    cnt = cur;
                    cur = 0;
                    ++ans;
                }
            }
            return ans;
        }
    };
}
namespace s2358o1
{   // 直接用求和公式O(1)时间复杂度
    class Solution {
    public:
        int maximumGroups(vector<int>& grades) {
            int n = grades.size();
            return int((sqrt(1 + 8 * n) - 1) / 2);
        }
    };
}

namespace s2522m1
{
    // 拉眼睛的做法，不用看，仅作纪念
    class Solution {
    public:
        int minimumPartition(string s, int k) {
            string sub = "";
            bool flag = false;
            int ans = 0;
            for (char c : s) {
                sub += c;
                if (stoll(sub) <= k) {
                    flag = true;
                    continue;
                }
                else if (flag) {
                    flag = false;
                    ++ans;
                    sub.clear();
                    sub += c;
                    if (stoll(sub) <= k) {
                        flag = true;
                    }
                    else {
                        return -1;
                    }
                }
                else {
                    sub.clear();
                }
            }
            if (flag) {
                ++ans;
            }
            return ans == 0 ? -1 : ans;
        }
    };
}
namespace s2522o1 {
    // 灵神写法，用字符与'0'的差值转换为整数值，最大程度利用了所有字符都在0-9之间的条件
    class Solution {
    public:
        int minimumPartition(string s, int k) {
            int ans = 0; long long num = 0;
            for (char c : s) {
                int x = c - '0';
                if (x > k) return -1;
                num = num * 10 + x;
                if (num > k) {
                    num = x;
                    ++ans;
                }
            }
            return ans + 1;
        }
    };
}
// 【1.6】先枚举，后贪心(1)
/*
2171.拿出最少数目的魔法豆：给定一个 正整数 数组 beans ，其中每个整数表示一个袋子里装的魔法豆的数目。
请你从每个袋子中 拿出 一些豆子（也可以 不拿出），使得剩下的 非空 袋子中（即 至少还有一颗 魔法豆的袋子）魔法豆的数目相等。
一旦把魔法豆从袋子中取出，你不能再将它放到任何袋子中。请返回你需要拿出魔法豆的 最少数目。
*/
// ---------------------
// 正难则反，计算拿掉的豆子数难，但是剩下的豆子数简单
namespace s2171o1
{   // 总豆数 = 取出的豆子数 + 剩下的豆子数
    // 枚举要让多少袋为空
    // o1为看会思路后自己写的，o2为灵神写法，更简洁
    class Solution {
    public:
        long long minimumRemoval(vector<int>& beans) {
            int n = beans.size();
            sort(beans.begin(), beans.end());
            long long sum = accumulate(beans.begin(), beans.end(), 0ll);
            long long ans = sum;
            for (int x = 0; x < n; ++x) {
                long long rest = (n - x) * (long long)beans[x];
                ans = min(sum - rest, ans);
            }
            return ans;
        }
    };
}
namespace s2171o2
{   // 不要执着于min取出的豆子，直接求max剩下的豆子，最后return的时候减一次就行
    class Solution {
    public:
        long long minimumRemoval(vector<int>& beans) {
            sort(beans.begin(), beans.end());
            int n = beans.size();
            long long sum = 0, save = 0;
            for (int i = 0; i < n; ++i) {
                sum += beans[i];// 直接把求和的过程也放在这次遍历中，省一次遍历时间
                save = max(save, 1LL * (n - i) * beans[i]);// 直接求max save，省下n次减法运算
            }
            return sum - save;
        }
    };
}
// ---------------------
// 【1.7】交换论证法(3)
/*
2895.最小处理时间：你有 n 颗处理器，每颗处理器都有 4 个核心。现有 n * 4 个待执行任务，每个核心只执行 一次 任务。
给你一个下标从 0 开始的整数数组 processorTime ，表示每颗处理器最早空闲时间。
另给你一个下标从 0 开始的整数数组 tasks ，表示执行每个任务所需的时间。返回所有任务都执行完毕需要的 最小时间 。
注意：每个核心独立执行任务。

179.最大数：给定一组非负整数 nums，重新排列每个数的顺序（每个数不可拆分）使之组成一个最大的整数。
注意：输出结果可能非常大，所以你需要返回一个字符串而不是整数。

3309.连接二进制表示可形成的最大数值：现以某种顺序 连接 数组 nums 中所有元素的 二进制表示 ，
请你返回可以由这种方法形成的 最大 数值。注意 任何数字的二进制表示 不含 前导零。
*/
// ---------------------
namespace s2895m1
{    class Solution {
    public:
        int minProcessingTime(vector<int>& processorTime, vector<int>& tasks) {
            sort(processorTime.begin(), processorTime.end());
            sort(tasks.begin(), tasks.end(), greater<int>());
            int n = processorTime.size();
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                int time = processorTime[i] + tasks[4 * i];
                ans = max(time, ans);
            }
            return ans;
        }
    };

}

// 字典序排列字符串的另一种考虑
namespace s179o1
{
    class Solution {
    public:
        string largestNumber(vector<int>& nums) {
            sort(nums.begin(), nums.end(), [&](const int a, const int b) {
                string left = to_string(a);
                string right = to_string(b);// 升序排列
                return left + right > right + left;// 比如9,90,900，如果只比较left和right，从小到大排列会是9, 90, 900
                // 而我们需要的是900，90，9，所以混合left和right进行比较
                });
            if (nums[0] == 0) return "0";// 如果nums = [0, 0]，那么返回的是"00"，而正确答案是"0"
            string ans = "";
            for (int i : nums) {
                ans += to_string(i);
            }
            return ans;
        }
    };
}

namespace s3309m1
{  
    class Solution {
    public:
        int maxGoodNumber(vector<int>& nums) {
            auto trans = [](int num) -> string {
                if (num == 0) return "0";// 将num转换为无前导0的二进制字符串
                unsigned mask = 1U << 31;// 这里必须为无符号整数
                while (!(mask & num)) {
                    mask >>= 1;
                }
                string ans = "";
                while (mask != 0) {
                    ans += (mask & num ? "1" : "0");
                    mask >>= 1;
                }
                return ans;
                };
            /*  //下面的trans版本更高效一点
            auto trans = [](int num) -> string {
            if (num == 0) return "0";
            string s = "";
            while (num) {
                s += (num & 1) ? "1" : "0";
                num >>= 1;
            }
            reverse(s.begin(), s.end());
            return s;
            };
            // 或者直接用库函数，利用stoi可以自动跳过前导0的特性, #include<bitset>
            string s = bitset<32> (num).to_string();
            return to_string(stoi(s, nullptr, 10));
            */
            
            vector<string> vec = { trans(nums[0]), trans(nums[1]), trans(nums[2]) };
            sort(vec.begin(), vec.end(), [&](const auto& a, const auto& b) {
                return a + b > b + a;// 进阶字典序，仿照s2895
                });
            string s = vec[0] + vec[1] + vec[2];
            return stoi(s, nullptr, 2);// 以2为基底将字符串转换为int（十进制）
        }
    };
}
// ---------------------
// 【1.8】相邻不同(7)
// 模仿模板题进行问题转化
/*

*/
// ---------------------
// 记录三个值里的最大两个值
namespace s2335m1
{
    class Solution {
    public:
        int fillCups(vector<int>& amount) {
            sort(amount.begin(), amount.end());
            int first = amount[2];
            int second = amount[1];
            int third = amount[0];
            int ans = 0;
            while (first && second) {
                --first, --second, ++ans;
                if (first < third) {
                    swap(first, third);
                    swap(third, second);
                }
                else if (second < third) {
                    swap(second, third);
                }
            }
            return ans + first;
        }
    };
}
namespace s2335m2
{
    class Solution {
    public:
        int fillCups(vector<int>& amount) {
            int mx = 0, sum = 0;
            for (int i : amount) {
                sum += i;
                mx = max(i, mx);
            }
            if (mx == 0) return 0;
            return (sum - mx >= mx - 1) ? (sum - 1) / 2 + 1 : mx;
        }
    };
}

// 同s2335，也可根据1953思路改写
namespace s1753m1 {
    class Solution {
    public:
        int maximumScore(int a, int b, int c) {
            vector<int> vec = { a, b, c };
            sort(vec.begin(), vec.end());
            int first = vec[2];
            int second = vec[1];
            int third = vec[0];
            int ans = 0;
            while (first && second) {
                --first, --second, ++ans;
                if (first < third) {
                    swap(first, third);
                    swap(third, second);
                }
                else if (second < third) {
                    swap(third, second);
                }
            }
            return ans;
        }
    };
}
namespace s1753m2
{
    // 根据s1953思路改写，更快
    class Solution {
    public:
        int maximumScore(int a, int b, int c) {
            int mx = max({ a, b, c });
            int sum = a + b + c;
            return (sum - mx >= mx - 1) ? sum / 2 : sum - mx;
        }
    };
}

// 模板题3，s2335中的数字个数从固定的3变成不固定的n，泛化
// 等价于需要构造一个尽量长的，相邻元素不同的序列，且元素 x 的出现次数不能超过 vec[x]
namespace s1953o1
{   /*
    将元素分成出现频次最多的元素单独一堆共m个，以及剩下的所有元素一堆共n个
    如果要在m个元素中插空放元素，那么全部插满需要m - 1个元素
    如果n >= m - 1，则代表所有元素都可以插空放满，比如[5,4,4]，最多的元素有5个，可以放成012 012 012 012 0
    标准填法如下：共13个元素，最多的元素从偶序号开始填，填完最多的填第二多的： 0_0_0_0_0_1_1
    然后开始填奇序号01 01 02 02 02 12 1
    如果n < m - 1，代表无法将m个元素中的所有空隙插满，所以最多有n + n + 1个元素
    */   
    class Solution {
    public:
        long long numberOfWeeks(vector<int>& milestones) {
            int mx = 0;
            long long sum = 0;
            for (int i : milestones) {
                mx = max(mx, i);
                sum += i;
            }
            if (sum - mx >= mx - 1) {
                return sum;
            }
            else {
                return (sum - mx) * 2 + 1;
            }
            // return (sum - mx >= mx - 1) ? sum : (sum - mx) * 2 + 1;
        }
    };
}

// 模板题4，将1953中的相邻元素不同的序列构造并输出出来
namespace s767o1
{
    string reorganizeString(string s) {
        // 对字符按照出现频次进行排序
        unordered_map<char, int> dic;
        for (char c : s) {
            ++dic[c];
        }
        vector<pair<char, int>> pairs(dic.begin(), dic.end());
        sort(pairs.begin(), pairs.end(), [&]
        (const auto& p1, const auto& p2) {
                return p1.second > p2.second;
            });
        // 判断是否能构成字符串
        int n = s.size();
        int mx = pairs[0].second;
        if (n - mx < mx - 1) return "";
        // 填充字符串并输出
        string ans(n, 0);
        int i = 0;
        for (auto& p : pairs) {
            while (p.second--) {
                ans[i] = p.first;
                i += 2;
                if (i >= n) {
                    i = 1; // 填完偶数填奇数
                }
            }
        }
        return ans;
    }
}
namespace s767o2
{   // 不排序的做法，只找出最高频的字符，其他字符不排序直接放，整体更快一点点
    class Solution {
    public:
        string reorganizeString(string s) {
            int n = s.size();
            int dic[26]{}, m = 0;
            char mch = '\n';
            for (char c : s) {
                if (++dic[c - 'a'] > m) {
                    mch = c;
                    m = dic[c - 'a'];
                }
            }
            if (n - m < m - 1) return "";
            string ans(n, 0);
            int i = 0;
            for (; m--; i += 2) {
                ans[i] = mch;
            }
            dic[mch - 'a'] = 0;

            for (int j = 0; j < 26; ++j) {
                int cnt = dic[j];
                while (cnt--) {
                    if (i >= n) {
                        i = 1;// 注意，这里需要将i >= n判断提前，因为之前的ans[i] = mch循环可能导致i越界
                    }
                    ans[i] = 'a' + j;
                    i += 2;
                }
            }
            return ans;
        }
    };
}

// 同s767
namespace s1054m1
{
    class Solution {
    public:
        vector<int> rearrangeBarcodes(vector<int>& barcodes) {
            unordered_map<int, int> count;
            int mx, mf = 0;
            for (int i : barcodes) {
                if (++count[i] > mf) {
                    mf = count[i];
                    mx = i;
                }
            }
            int n = barcodes.size();
            vector<int> ans(n);
            int k = count.size();
            int i = 0;
            for (; mf--; i += 2) {
                ans[i] = mx;
            }
            count[mx] = 0;
            for (auto& p : count) {
                while (p.second--) {
                    if (i >= n) {
                        i = 1;
                    }
                    ans[i] = p.first;
                    i += 2;
                }
            }
            return ans;
        }
    };
}

namespace s2856m1
{   // m1做法为套模板，但是没有利用到题干中的非递减性质，o1为logn做法
    class Solution {
    public:
        int minLengthAfterRemovals(vector<int>& nums) {
            // 可以匹配完，n为偶数，0，n为奇数，1
            // 匹配不完，n - 2 * (n - mx)
            unordered_map<int, int> count;
            int mx = 0;
            for (int i : nums) {
                mx = max(++count[i], mx);
            }
            int n = nums.size();
            if (n - mx >= mx) {
                return n % 2;
            }
            else {
                return 2 * mx - n;
                //return n - 2 * (n - mx);
            }
            // 也可以直接取max
            // return max(n % 2, 2 * mx - n);
        }
    };
}
namespace s2856o1
{    /*
     利用排序的性质可以将时间复杂度降为Ologn
     换个思路，不是用是否能匹配完最大频率数，而是比对mx 和 n / 2的关系
     如果mx > n / 2，相当于无法全部匹配完，答案为n - 2 * (n - mx)
     如果mx <= n / 2，相当于可以匹配完，看n是否为奇数，返回n % 2
     而此时问题进一步转化为看nums[n / 2]处的元素
     如果mx > n / 2，此时nums[n / 2]处元素必定为最大频率数
     如果mx <= n / 2，此时无需管最大频率数以及最大频率是具体是什么
     也即只需要求nums[n / 2]处元素的频次即可,此时可以用二分来加速至Ologn
     */
    class Solution {
    public:
        int minLengthAfterRemovals(vector<int>& nums) {
            int n = nums.size();
            int x = nums[n / 2];
            int maxCnt = upper_bound(nums.begin(), nums.end(), x) -
                lower_bound(nums.begin(), nums.end(), x);
            return max(2 * maxCnt - n, n % 2);
        }
    };
}

namespace s984m1
{
    // 假设字母a数量更多，让其两个一组，则与其配对的b的组数为(a + 1) / 2
    // 注意最后一组若是a，其对应的b也一定是单个的，否则会与假设“字母a更多”相矛盾
    // aabb aabb aabb ab
    class Solution {
    public:
        string strWithout3a3b(int a, int b) {
            char mxch = 'a';
            char mnch = 'b';
            if (a < b) {
                swap(mxch, mnch);
                swap(a, b);
            }
            int gap = (a + 1) / 2;
            int rest = b - gap;
            string ans(a + b, 0);
            for (int i = 0; a > 0; ) {
                ans[i] = mxch;
                ans[i + 1] = mxch;
                a -= 2;
                if (a < 0) {
                    ans[i + 1] = mnch;
                    break;
                }
                if (b == 0) break;
                if (rest-- > 0) {
                    ans[i + 2] = mnch;
                    ans[i + 3] = mnch;
                    b -= 2;
                    i += 4;
                }
                else {
                    ans[i + 2] = mnch;
                    b -= 1;
                    i += 3;
                }
            }
            return ans;
        }
    };
}
namespace s984o1
{
    // 有点像环形链表，a和b互相追赶，o1写法更简洁也不容易出错
    class Solution {
    public:
        string strWithout3a3b(int a, int b) {
            string ans = "";
            // reserve只预留空间，但还没真正分配，与string ans(a + b)不同，后者已经分配内存并可以用operator[]进行访问了
            ans.reserve(a + b);// 不要用string ans(a + b)，这样ans的大小最后会变成2 * (a + b)，前面是a + b个'\n'
            while (a > 0 && b > 0) {
                if (a == b) {
                    --a;
                    --b;
                    ans += "ab";
                }
                else if (a > b) {
                    a -= 2;
                    --b;
                    ans += "aab";
                }
                else {
                    --a;
                    b -= 2;
                    ans += "bba";
                }
            }
            if (a > 0) {
                ans += string(a, 'a');
            }
            if (b > 0) {
                ans += string(b, 'b');
            }
            return ans;
        }
    };
}
// ---------------------------------------------------------------
//二、区间策略 (1)
/*区间贪心有如下经典问题：
不相交区间（单机器调度/活动安排）：给定一些区间，从中选出尽量多的两两互不相交的区间。
区间分组（任务调度/会议室）：给定一些区间，把这些区间分成最少的组，使得每组内的区间互不相交。
区间选点（射气球，Interval Stabbing）：给定一些区间，在数轴上放置最少的点，使得每个区间都包含至少一个点。最少要放置多少个点？
区间覆盖（灌溉花园）：给定一些区间，从中选出尽量少的区间，覆盖一条指定线段 [s,t]。
一般来说，区间合并问题（2.5 节）是按左端点排序，其余大多数区间贪心是按右端点排序。请读者在做题后，仔细体会为什么要这样排序。
*/

// 【2.1】不相交区间()
// 
/*

*/
// ---------------------



// ---------------------
// 【2.2】区间分组()
// 
/*

*/
// ---------------------



// ---------------------
// 【2.3】区间选点()
// 
/*

*/
// ---------------------



// ---------------------
// 【2.4】区间覆盖(1)
// 
/*
45.跳跃游戏 II：给定一个长度为 n 的 0 索引整数数组 nums。初始位置在下标 0。
每个元素 nums[i] 表示从索引 i 向后跳转的最大长度。换句话说，如果你在索引 i 处，你可以跳转到任意 (i + j) 处：
0 <= j <= nums[i] 且 i + j < n，返回到达 n - 1 的最小跳跃次数。测试用例保证可以到达 n - 1。
*/
// ---------------------
// 同样也是自己写出来了，虽然有点磕磕绊绊
namespace s45m1
{
    class Solution {
    public:
        int jump(vector<int>& nums) {
            int n = nums.size();
            if (n == 1) return 0;// 不需要进行跳跃

            int i = 0;
            int mx = i + nums[i];// 实际上就是nums[0]，但是为了前后变量的可读性这样写

            // 只要尝试进行mx < n - 1的判断就说明已经发生了一次跳跃
            // 这个判断条件判断了几次就跳跃了几次
            int cnt = 1;
            while (mx < n - 1) {
                int temp = mx;
                for (int j = i; j <= mx; ++j) {// 每次在[i, mx]上寻找最大的右边界
                    temp = max(nums[j] + j, temp);
                }
                i = mx;// 更新i和mx
                mx = temp;

                ++cnt;// 在尝试判断mx < n - 1前（也即准备跳跃时），增加跳跃次数
            }
            return cnt;
        }
    };
}
namespace s45m2
{   // 隔了半年自己写出来的
    class Solution {
    public:
        int jump(vector<int>& nums) {
            int n = nums.size();
            if (n == 1) return 0;// 这次漏了不跳跃的特例

            int cnt = 0;
            int right = nums[0];
            int next = 0;

            for (int i = 0; right < n - 1; ++i) {
                if (i <= right) {
                    next = max(next, i + nums[i]);
                }
                if (i == right) {
                    ++cnt;
                    right = next;
                }
            }

            return cnt + 1;
        }
    };
}
namespace s45m3
{   // 根据o1对m2进行改进
    class Solution {
    public:
        int jump(vector<int>& nums) {
            int ans = 0, n = nums.size();
            int cur = 0, next = 0;

            for (int i = 0; cur < n - 1; ++i) {
                next = max(next, i + nums[i]);

                if (i == cur) {
                    cur = next;
                    ++ans;
                }
            }

            return ans;
        }
    };
}
namespace s45o1
{   // 灵神的写法，把问题抽象成造桥，代码更简洁，两种思路没有优劣之分
    // o1思路其实跟我写的m2更像
    class Solution {
    public:
        int jump(vector<int>& nums) {
            int n = nums.size();
            int ans = 0;
            int cur_right = 0; // 已建造的桥的右端点
            int next_right = 0; // 下一座桥的右端点的最大值

            // for 循环计算的是在到达终点之前需要造多少座桥。n - 1 已经是终点了，不需要造桥
            for (int i = 0; i < n - 1; i++) {
                // 循环条件换成cur_right < n - 1可以更快退出循环
                // 遍历的过程中，记录下一座桥的最远点
                next_right = max(next_right, i + nums[i]);
                if (i == cur_right) { // 无路可走，必须建桥，走到了当前区间上的末端，下一个跳跃点的最大值已经计算完毕
                    cur_right = next_right; // 建桥后，最远可以到达 next_right
                    ans++;
                }
            }
            return ans;
        }
    };
}

// ---------------------
// 【2.5】合并区间(1)
// 跟11.2专题-差分【2.1】一维差分题目重叠了不少，重叠的题目都可以用差分或者贪心来做
/*
56.合并区间：以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi] 。
请你合并所有重叠的区间，并返回 一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间 。

57.插入区间：给你一个 无重叠的 ，按照区间起始端点排序的区间列表 intervals，
其中 intervals[i] = [starti, endi] 表示第 i 个区间的开始和结束，
并且 intervals 按照 starti 升序排列。同样给定一个区间 newInterval = [start, end] 表示另一个区间的开始和结束。
在 intervals 中插入区间 newInterval，使得 intervals 依然按照 starti 升序排列，
且区间之间不重叠（如果有必要的话，可以合并区间）。返回插入之后的 intervals。
注意 你不需要原地修改 intervals。你可以创建一个新数组然后返回它。

2848.与车相交的点：给你一个下标从 0 开始的二维整数数组 nums 表示汽车停放在数轴上的坐标。
对于任意下标 i，nums[i] = [starti, endi] ，其中 starti 是第 i 辆车的起点，endi 是第 i 辆车的终点。
返回数轴上被车 任意部分 覆盖的整数点的数目。

55.跳跃游戏：给你一个非负整数数组 nums ，你最初位于数组的 第一个下标 。数组中的每个元素代表你在该位置可以跳跃的最大长度。
判断你是否能够到达最后一个下标，如果可以，返回 true ；否则，返回 false 。
*/
// ---------------------
// 56.合并区间 + 57.插入区间 + 2848.与车相交的点 详见11.2专题-差分【2.1】一维差分

// 竟然自己写出来了，难以置信哈哈哈哈（有我一开始就知道题型是贪心的原因），灵神的思路不一样，可以提前返回，更好
namespace s55m1
{   // 不断更新最长的可行路径的左边下标
    // 看两个例子更容易明白：
    // 例1：[2,3,1,1,4]
    // 例2：[3,2,1,0,4]
    class Solution {
    public:
        bool canJump(vector<int>& nums) {
            int n = nums.size();
            if (n == 1) return true;

            int targetIndex = n - 1;

            for (int i = targetIndex - 1; i >= 0; --i) {
                int len = targetIndex - i;
                if (nums[i] < len) continue; // 当前节点跳不到终点，继续往右尝试
                targetIndex = i;    // 当前节点可以跳到终点，将其设为新终点
            }
            return targetIndex == 0;
        }
    };
}
namespace s55o1
{   // 灵神思路跟我不同，通过mx记录最右可达节点的下标，可以提前返回false，比我的写法更好
    // update：半年后复刷写出的代码跟o1一样
    class Solution {
    public:
        bool canJump(vector<int>& nums) {
            int n = nums.size();
            int mx = 0;
            for (int i = 0; i < n; ++i) {
            // 循环条件也可以改成下面这样，当mx >= n - 1时就说明已经可以到终点了，可以提前退出，进一步提速
            // for (int i = 0; mx < n - 1; ++i)
                if (i > mx) { // 无法到达 i
                    return false;
                }
                mx = max(mx, i + nums[i]); // 从 i 最右可以跳到 i + nums[i]
            }
            return true;
        }
    };
}

// 合并每种字母出现的最左最右下标的区间，思路也是自己想出来的，看来确实变强了一点
namespace s763m1
{   // 问题转化 + s56合并区间贪心做法
    // 我的做法用到了排序，还用到了额外的哈希表和数组，整体思路虽然正确，但是太粗糙，效率太低，还是看o1灵神写法吧
    class Solution {
    public:
        vector<int> partitionLabels(string s) {
            unordered_map<char, pair<int, int>> mp;
            int n = s.size();

            // 遍历一遍得到区间对
            for (int i = 0; i < n; ++i) {
                char c = s[i];
                if (mp.count(c)) {
                    mp[c].second = i;
                }
                else {
                    mp[c] = { i, i };
                }
            }
            // 转成vector<pair<int, int>>
            vector<pair<int, int>> intervals;
            intervals.reserve(mp.size());
            for (const auto& pair : mp) {
                intervals.push_back(pair.second);  // 拷贝版本
            }

            // 按区间左端点排序(我可以直接排序unordered_map内的pair吗？)
            sort(intervals.begin(), intervals.end(),
                [](const auto& a, const auto& b) {return a.first < b.first; });

            // 合并区间
            vector<int> ans;
            int k = intervals.size();
            auto [left, right] = intervals[0];
            for (int i = 1; i < k; ++i) {
                auto [l, r] = intervals[i];
                if (l < right) {
                    right = max(right, r);
                }
                else {
                    ans.push_back(right - left + 1);
                    left = l;
                    right = r;
                }
            }
            ans.push_back(right - left + 1);
            return ans;
        }
    };
}
namespace s763o1
{
    class Solution {
    public:
        vector<int> partitionLabels(string s) {
            int n = s.size();
            int last[26]{};
            for (int i = 0; i < n; i++) {
                last[s[i] - 'a'] = i; // 每个字母最后出现的下标
            }

            vector<int> ans;
            int start = 0, end = 0;
            for (int i = 0; i < n; i++) {
                end = max(end, last[s[i] - 'a']); // 更新当前区间右端点的最大值
                if (end == i) { // 当前区间合并完毕
                    ans.push_back(end - start + 1); // 区间长度加入答案
                    start = i + 1; // 下一个区间的左端点
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【2.6】其他区间贪心()
// 
/*

*/
// ---------------------



// ---------------------------------------------------------------
//三、字符串贪心 ()

// 【3.1】字典序最小/最大()
// 
/*

*/
// ---------------------



// ---------------------
// 【3.2】回文串贪心()
// 
/*

*/
// ---------------------



// ---------------------------------------------------------------
//四、数学贪心 ()

// 【4.1】基础 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.2】乘积贪心 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.3】排序不等式 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.4】均值不等式 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.5】中位数贪心 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.6】归纳法 ()
// 
/*

*/
// ---------------------


// ---------------------
// 【4.7】其他数学贪心 ()
// 
/*

*/
// ---------------------


// ---------------------