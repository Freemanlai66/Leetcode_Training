#pragma once
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

// 问题待定：
/*

*/

/*
模板题：
1.差分数组最简单的应用示例：2848
2.用offset节省diff数组的空间：1854
3.o2差分数组解法，但对下标乘2，来规避诸如[0, 1],[2,3]的区间相连问题：57
4.二维差分（暂时没做）：2536
*/

// 差分：一维差分（扫描线） + 二维差分
// 差分与前缀和的关系，类似导数与积分的关系，数组 a 的差分的前缀和就是数组 a。
// 【2.1】一维差分 (9)
/*
2848.与车相交的点：给你一个下标从 0 开始的二维整数数组 nums 表示汽车停放在数轴上的坐标。
对于任意下标 i，nums[i] = [starti, endi] ，其中 starti 是第 i 辆车的起点，endi 是第 i 辆车的终点。
返回数轴上被车 任意部分 覆盖的整数点的数目。

1893.检查是否区域内所有整数都被覆盖：给你一个二维整数数组 ranges 和两个整数 left 和 right 。
每个 ranges[i] = [starti, endi] 表示一个从 starti 到 endi 的 闭区间 。
如果闭区间 [left, right] 内每个整数都被 ranges 中 至少一个 区间覆盖，那么请你返回 true ，否则返回 false 。
已知区间 ranges[i] = [starti, endi] ，如果整数 x 满足 starti <= x <= endi ，那么我们称整数x 被覆盖了。

1854.人口最多的年份：给你一个二维整数数组 logs ，其中每个 logs[i] = [birthi, deathi] 表示第 i 个人的出生和死亡年份。
年份 x 的 人口 定义为这一年期间活着的人的数目。第 i 个人被计入年份 x 的人口需要满足：
x 在闭区间 [birthi, deathi - 1] 内。注意，人不应当计入他们死亡当年的人口中。返回 人口最多 且 最早 的年份。

2960.统计已测试设备：给你一个长度为 n 、下标从 0 开始的整数数组 batteryPercentages ，表示 n 个设备的电池百分比。
你的任务是按照顺序测试每个设备 i，执行以下测试操作：
如果 batteryPercentages[i] 大于 0：增加 已测试设备的计数。
将下标 j 在 [i + 1, n - 1] 的所有设备的电池百分比减少 1，确保它们的电池百分比 不会低于 0 ，
即 batteryPercentages[j] = max(0, batteryPercentages[j] - 1)。
移动到下一个设备。否则，移动到下一个设备而不执行任何测试。
返回一个整数，表示按顺序执行测试操作后 已测试设备 的数量。

1094.拼车：车上最初有 capacity 个空座位。车 只能 向一个方向行驶（也就是说，不允许掉头或改变方向）
给定整数 capacity 和一个数组 trips ,  trip[i] = [numPassengersi, fromi, toi] 表示第 i 次旅行有 numPassengersi 乘客，
接他们和放他们的位置分别是 fromi 和 toi 。这些位置是从汽车的初始位置向东的公里数。
当且仅当你可以在所有给定的行程中接送所有乘客时，返回 true，否则请返回 false。

1109.航班预订统计：这里有 n 个航班，它们分别从 1 到 n 进行编号。
有一份航班预订表 bookings ，表中第 i 条预订记录 bookings[i] = [firsti, lasti, seatsi] 意味着
在从 firsti 到 lasti （包含 firsti 和 lasti ）的 每个航班 上预订了 seatsi 个座位。
请你返回一个长度为 n 的数组 answer，里面的元素是每个航班预定的座位总数。

56.合并区间以数组 intervals 表示若干个区间的集合，
其中单个区间为 intervals[i] = [starti, endi] 。请你合并所有重叠的区间，并返回 一个不重叠的区间数组，
该数组需恰好覆盖输入中的所有区间 。

57.插入区间：给你一个 无重叠的 ，按照区间起始端点排序的区间列表 intervals，
其中 intervals[i] = [starti, endi] 表示第 i 个区间的开始和结束，并且 intervals 按照 starti 升序排列。
同样给定一个区间 newInterval = [start, end] 表示另一个区间的开始和结束。
在 intervals 中插入区间 newInterval，使得 intervals 依然按照 starti 升序排列，
且区间之间不重叠（如果有必要的话，可以合并区间）。返回插入之后的 intervals。
注意 你不需要原地修改 intervals。你可以创建一个新数组然后返回它。
*/
// ---------------------
// m1：对区间排序，然后遍历合并区间，时间复杂度O(nlogn)
// 模板题1：o1为差分数组做法，遍历每个区间，将区间内的对应元素下标全部+1，最后遍历元素值数组的范围，统计非0的个数，O(n)
// 拓展：迭代器it[0],it[1]代表的含义
namespace s2848m1
{   
    class Solution {
    public:
        int numberOfPoints(vector<vector<int>>& nums) {
            sort(nums.begin(), nums.end(),
                [&](const auto& a, const auto& b) {
                    return a[0] < b[0];
                });
            int l = nums[0][0], r = nums[0][1], n = nums.size();
            int ans = 0;
            for (int i = 1; i < n; ++i) {
                auto& p = nums[i];
                if (p[0] > r) {// 当前区间和维护的区间没有交集，无法合并，新开一个区间并更新答案
                    ans += r - l + 1;
                    l = p[0];
                    r = p[1];
                }
                else {
                    r = max(r, p[1]);
                }
            }
            return ans + r - l + 1;
        }
    };
}
namespace s2848o1
{   // 将区间内累加的n步操作，降为O(1)
    class Solution {
        // index: 0, 1, 2, 3, 4
        // 原数组:0, 0, 0, 0, 0
        // 新数组:0, 1, 1, 1, 0
        // diff  :0, 1, 0, 0, -1(累加回去后就变成了新数组)
    public:
        int numberOfPoints(vector<vector<int>>& nums) {
            auto it = max_element(nums.begin(), nums.end(),
                [](const auto& a, const auto& b) { return a[1] < b[1]; });
            int mx = (*it)[1];
            vector<int> diff(mx + 2);// 注意差分数组用错位的方式定义，数值范围为[1, mx + 1]
            for (auto& p : nums) {
                ++diff[p[0]];
                --diff[p[1] + 1];
            }
            int ans = 0, s = 0;
            for (int i = 1; i < mx + 2; ++i) {// 这里也可以写for (int d : diff)，i = 0时, s += 0也不影响答案
                s += diff[i];
                if (s > 0) {
                    ++ans;// 这里可以写成ans += s > 0;
                }
            }
            return ans;
        }
    };
}

// 套模板，与s2848类似
namespace s1893m1
{
    class Solution {
    public:
        bool isCovered(vector<vector<int>>& ranges, int left, int right) {
            int mn = INT_MAX, mx = INT_MIN;
            for (auto& p : ranges) {
                mn = min(mn, p[0]);
                mx = max(mx, p[1]);
            }
            if (mn > left || mx < right) return false;
            vector<int> diff(mx + 2);
            for (auto& p : ranges) {
                ++diff[p[0]];
                --diff[p[1] + 1];
            }
            int s = 0;
            for (int i = 0; i < mx + 2; ++i) {
                s += diff[i];
                if (i >= left && i <= right && s <= 0) {
                    return false;
                }
            }
            return true;
        }
    };
}

// 模板题2：用offset节省diff数组的空间
namespace s1854m1
{
    class Solution {
    public:
        int maximumPopulation(vector<vector<int>>& logs) {
            vector<int> diff(2052, 0);
            for (auto& p : logs) {
                ++diff[p[0]];
                --diff[p[1]];
            }
            int s = 0, mx = 0, ans = 0;
            for (int i = 1950; i < 2052; ++i) {
                s += diff[i];
                if (s > mx) {
                    ans = i;
                    mx = s;
                }
            }
            return ans;
        }
    };
}
namespace s1854o1
{   // 用offset节省diff空间
    class Solution {
    private:
        static constexpr int offset = 1950;
    public:
        int maximumPopulation(vector<vector<int>>& logs) {
            vector<int> diff(101, 0);
            for (const auto& p : logs) {
                ++diff[p[0] - offset];
                --diff[p[1] - offset];
            }
            int s = 0, mx = 0, ans = 0;
            for (int i = 0; i < 101; ++i) {
                s += diff[i];
                if (s > mx) {
                    ans = i;
                    mx = s;
                }
            }
            return ans + offset;
        }
    };
}

// 这道题基本跟差分没关系，最基础的简单模拟题
namespace s2960m1
{   // i + 1之后扣减的电量会逐步增加
    class Solution {
    public:
        int countTestedDevices(vector<int>& batteryPercentages) {
            int ans = 0;
            int offset = 0;
            for (int x : batteryPercentages) {
                if (x - offset > 0) {
                    ++ans;
                    ++offset;
                }// 可以简化为: ans += x > ans; // ans和offset是相同的，所以可以这样简化，但可读性没我的好
            }
            return ans;
        }
    };
}

// 标准差分，套模板
namespace s1094m1
{   // 要注意是from - (to - 1)，因为乘客在to处会下车
    // [i, j]内全部加上x，那么diff[i] + x， diff[j + 1] - x，所以正好是diff[to] - x
    class Solution {
    public:
        bool carPooling(vector<vector<int>>& trips, int capacity) {
            auto it = max_element(trips.begin(), trips.end(),
                [](const auto& a, const auto& b) {return a[2] < b[2]; });
            int mx = (*it)[2];

            vector<int> diff(mx + 1);
            for (auto& t : trips) {
                diff[t[1]] += t[0];
                diff[t[2]] -= t[0];
            }
            int s = 0;
            for (int d : diff) {
                s += d;
                if (s > capacity) {
                    return false;
                }
            }
            return true;
        }
    };
}

namespace s1109m1
{
    class Solution {
    public:
        vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
            vector<int> ans(n);
            vector<int> diff(n + 2);
            for (auto& b : bookings) {
                diff[b[0]] += b[2];
                diff[b[1] + 1] -= b[2];
            }
            int s = 0;
            for (int i = 1; i < n + 1; ++i) {// 这里注意i < n + 1
                s += diff[i];
                ans[i - 1] = s;// 注意ans的下标与diff错开
            }
            return ans;
        }
    };
}

// 和s2848的m1解法几乎一样，都是先排序后合并，也可以用差分来做，但是因为[1,4][5,6]区间不能合并，下标要放大1倍
namespace s56m1
{   // 时间复杂度O(nlogn)，瓶颈在排序上，空间复杂度O(1)
    // 先按照左端点排序所有区间，然后维护右端点，逐个合并，很好理解
    class Solution {
    public:
        vector<vector<int>> merge(vector<vector<int>>& intervals) {
            sort(intervals.begin(), intervals.end(),
                [](const auto& a, const auto& b) {return a[0] < b[0]; });

            int l = intervals[0][0], r = intervals[0][1];
            vector<vector<int>> ans;
            for (const auto& p : intervals) {
                if (p[0] > r) {
                    // 这里不能用emplace_back，因为vector<int>的构造参数如果是l, r，那么会得到一个长为l，元素全是r的数组
                    ans.push_back({ l, r });
                    l = p[0];
                    r = p[1];
                }
                else {// l不用更新
                    r = max(r, p[1]);
                }
            }
            ans.push_back({ l, r });// 注意：最后一个区间别忘了
            return ans;
        }
    };
}
namespace s56o1
{   // 时间复杂度是O(max(U, n))，U是intervals最大的右端点, n是intervals的区间个数；空间复杂度是O(U)，空间换时间
    // 这个解法稍微复杂一点，更容易错
    class Solution {
    public:
        vector<vector<int>> merge(vector<vector<int>>& intervals) {
            // 题干：0 <= start_i <= end_i <= 10^4，可以直接用10e4，但这样时间复杂度相较于o1就完全没优势了，最好先遍历一遍
            auto max_it = max_element(intervals.begin(), intervals.end(),
                [&](const auto& a, const auto& b) {return a[1] < b[1]; });

            int MAX = (*max_it)[1];
            // 或者直接遍历一遍intervals获取MAX也没问题

            // 差分数组，避免[0, 0]这种区间中只有一个点的区间遗漏，下标放大一倍，并将区间尾下标加一
            // 大小需要覆盖到最大值+2（因为要用到end * 2 + 1的下标）
            vector<int> diff(MAX * 2 + 2, 0);

            // 1. 构建差分数组，给[2 * start, 2 * end]区间上全部加上1
            for (const auto& interval : intervals) {
                int start = interval[0] * 2;
                int end = interval[1] * 2;
                diff[start]++;        
                diff[end + 1]--;      // 差分做法是要末端点是要+1的
            }

            vector<vector<int>> result;
            int count = 0;           // 当前覆盖的区间数
            int currentStart = -1;   // 当前合并区间的起始位置

            // 2. 扫描差分数组，重构区间
            for (int i = 0; i < diff.size(); ++i) {
                count += diff[i];    // 更新当前覆盖的区间数

                if (count > 0 && currentStart == -1) {
                    // 计数器从0变为正数，开始一个新的合并区间
                    currentStart = i;
                }
                else if (count == 0 && currentStart != -1) {
                    // 计数器变为0，结束当前合并区间，除以2还原下标
                    result.push_back({ currentStart / 2, (i - 1) / 2 });
                    currentStart = -1;
                }
            }

            return result;
        }
    };
}

// m1, o1：模拟计数解法，考验细心能力
// 模板题3：o2差分数组解法，但对下标乘2，来规避诸如[0, 1],[2,3]的区间相连问题
namespace s57m1
{   // 逆天屎山
    class Solution {
    public:
        vector<vector<int>> insert(vector<vector<int>>& intervals,
            vector<int>& newInterval) {
            vector<vector<int>> ans;
            int i = 0, n = intervals.size();
            if (n == 0) {
                ans.push_back(newInterval);// 如果目前区间为空
                return ans;
            }

            if (intervals[n - 1][1] < newInterval[0]) {
                intervals.push_back(newInterval);// 如果插入区间在所有区间右边
                return intervals;
            }
            if (intervals[0][0] > newInterval[1]) {
                ans.reserve(n + 1);
                ans.push_back(newInterval);
                for (auto& p : intervals) {// 如果插入区间在所有区间左边
                    ans.push_back(p);
                }
                return ans;
            }
            while (i < n && intervals[i][1] < newInterval[0]) {
                ans.push_back(intervals[i++]);// 将插入区间左边的区间加入答案
            }
            if (newInterval[1] < intervals[i][0]) {
                ans.push_back(newInterval);// 如果插入区间在区间中间，且与其他区间没有交集
            }
            else {
                int l = min(newInterval[0], intervals[i][0]);
                int r = max(newInterval[1], intervals[i][1]);
                while (i < n && intervals[i][0] <= newInterval[1]) {// 如果插入区间和某个区间有交集
                    r = max(r, intervals[i++][1]);
                }
                ans.push_back({ l, r });
            }

            while (i < n) {
                ans.push_back(intervals[i++]);// 将插入区间右边的区间加入答案
            }
            return ans;
        }
    };
}
namespace s57o1
{   // 沿用m1的思路，AI改进的代码
    class Solution {
    public:
        vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
            vector<vector<int>> ans;
            int n = intervals.size();
            int i = 0;

            // 添加所有在新区间左侧且不重叠的区间
            while (i < n && intervals[i][1] < newInterval[0]) {
                ans.push_back(intervals[i++]);
            }

            // 合并重叠的区间
            int start = newInterval[0];// 这样初始化start, end可以兼顾n == 0，以及插入区间在所有区间左/右侧的情况
            int end = newInterval[1];
            while (i < n && intervals[i][0] <= end) {// 结合intervals[i][1] >= newInterval[0]，可确保有重叠部分
                start = min(start, intervals[i][0]);
                end = max(end, intervals[i][1]);
                i++;
            }
            ans.push_back({ start, end });

            // 添加剩余的区间
            while (i < n) {
                ans.push_back(intervals[i++]);
            }
            return ans;
        }
    };
}
namespace s57o2
{   // 将遍历到的区间元素对应值+1，最后遍历diff，将s > 0的连续区间放入答案
    // 但因为[0,1][2,4]这样的非连续区间在遍历diff时也是连续的，为了相隔开，将区间对应下标乘2再放入diff中
    class Solution {
    public:
        vector<vector<int>> insert(vector<vector<int>>& intervals,
            vector<int>& newInterval) {
            int n = intervals.size();
            int mx = newInterval[1];
            for (auto& interval : intervals) {
                mx = max(mx, interval[1]);
            }
            vector<int> diff(2 * mx + 2, 0);
            for (auto& interval : intervals) {
                int start = interval[0], end = interval[1];
                ++diff[2 * start];
                --diff[2 * end + 1];
            }
            ++diff[2 * newInterval[0]];
            --diff[2 * newInterval[1] + 1];
            vector<vector<int>> ans;
            int start = -1;
            int s = 0;
            for (int i = 0; i < diff.size(); i++) {
                s += diff[i];
                if (s > 0 && start == -1) {
                    start = i;
                }
                else if (s == 0 && start != -1) {
                    ans.push_back({ start / 2, i / 2 });
                    start = -1;
                    // 会遍历到2 * mx + 1，所以最后一个连续的区间[..., 2 *
                    // mx]也能记录到
                }
            }
            return ans;
        }
    };
}
// ---------------------
// 【2.2】二维差分 (0)
// 时间有限，暂时不做
/*

*/
// ---------------------

namespace s25361o1
{

}

