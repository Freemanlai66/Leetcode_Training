#pragma once
#include <vector>
#include <algorithm> // max_element, fab
#include <numeric> // accumulate, reduce
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <intrin.h> // __popcnt
#include <string>
#include <sstream> // stringstream
#include <iomanip> // setprecision
using namespace std;

// 二分算法：二分查找 + 二分答案
// 二、二分答案：花费一个 log 的时间，增加了一个条件，题目求什么，就二分什么(13+)

// 【2.1】求最小：二分搜索的高级应用，与求最大相呼应(5)
/*

1283.使结果不超过阈值的最小除数：给你一个整数数组 nums 和一个正整数 threshold  ，你需要选择一个正整数作为除数，
	然后将数组里每个数都除以它，并对除法结果求和。
	请你找出能够使上述结果小于等于阈值 threshold 的除数中 最小 的那个。
	每个数除以除数后都向上取整，比方说 7/3 = 3 ， 10/2 = 5 。题目保证一定有解。

2187.完成旅途的最少时间：给你一个数组 time ，其中 time[i] 表示第 i 辆公交车完成 一趟旅途 所需要花费的时间。
    每辆公交车可以 连续 完成多趟旅途，也就是说，一辆公交车当前旅途完成后，可以 立马开始 下一趟旅途。
    每辆公交车 独立 运行，也就是说可以同时有多辆公交车在运行且互不影响。
    给你一个整数 totalTrips ，表示所有公交车 总共 需要完成的旅途数目。请你返回完成 至少 totalTrips 趟旅途需要花费的 最少 时间。

1011.在 D 天内送达包裹的能力：传送带上的包裹必须在 days 天内从一个港口运送到另一个港口。
    传送带上的第 i 个包裹的重量为 weights[i]。每一天，我们都会按给出重量（weights）的顺序往传送带上装载包裹。
    我们装载的重量不会超过船的最大运载重量。返回能在 days 天内将传送带上的所有包裹送达的船的最低运载能力。

875.爱吃香蕉的珂珂：珂珂喜欢吃香蕉。这里有 n 堆香蕉，第 i 堆中有 piles[i] 根香蕉。警卫已经离开了，将在 h 小时后回来。
    珂珂可以决定她吃香蕉的速度 k （单位：根/小时）。每个小时，她将会选择一堆香蕉，从中吃掉 k 根。
    如果这堆香蕉少于 k 根，她将吃掉这堆的所有香蕉，然后这一小时内不会再吃更多的香蕉。  
    珂珂喜欢慢慢吃，但仍然想在警卫回来前吃掉所有的香蕉。
    返回她可以在 h 小时内吃掉所有香蕉的最小速度 k（k 为整数）。

思维拓展：这里不需要熟练掌握，开拓下思维即可

1870. 准时到达的列车最小时速：给你一个浮点数 hour ，表示你到达办公室可用的总通勤时间。要到达办公室，你必须按给定次序乘坐 n 趟列车。
    另给你一个长度为 n 的整数数组 dist ，其中 dist[i] 表示第 i 趟列车的行驶距离（单位是千米）。
    每趟列车均只能在整点发车，所以你可能需要在两趟列车之间等待一段时间。
    例如，第 1 趟列车需要 1.5 小时，那你必须再等待 0.5 小时，搭乘在第 2 小时发车的第 2 趟列车。
    返回能满足你在时限前到达办公室所要求全部列车的 最小正整数 时速（单位：千米每小时），如果无法准时到达，则返回 -1 。
    生成的测试用例保证答案不超过 10e7 ，且 hour 的 小数点后最多存在两位数字 。

*/
// ---------------------
// 模板题1
namespace s1283
{
    class Solution {
    public:
        int smallestDivisor(vector<int>& nums, int threshold) {
            auto check = [&](int m) ->bool {
                int sum = 0;
                for (int x : nums) {
                    sum += (x - 1 + m) / m;// a/b（向上去整） 等价于 (a-1)/b + 1（向下去整）
                    if (sum > threshold) {
                        return false;
                    }
                }
                return true;
            };

            int left = 0, right = *max_element(nums.begin(), nums.end());// 左右端点取不到
            // 开区间做法，二分的范围是不确定的返回，left初始值一定不满足要求，right初始值一定满足要求
            // 如果取left = 1可行吗？ 不行，因为如果最后在区间最左边退出循环，那么left = 1，返回的是right（一定可行的值）
            // 而真正的答案left = 1就取不到了，所以必须让开区间左端点取不可能的值
            // 如果取right = max - 1可行吗？不行，因为如果最后在区间最右边退出循环，那么right = max - 1，返回的是right，此时并不满足条件
            // 而真正的答案right = max就取不到了，所以必须让开区间最右端取一定满足条件的值
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                if (check(mid)) {// 这一段可以简化为(check(mid) ? right : left) = mid;因为三目运算法的后两个操作数都是左值
                    // right和left都是左值（不是常量或临时对象）
                    right = mid;// check函数返回true时，更新的是什么，返回的就是什么
                } else {
                    left = mid;
                }
            }
            return right;// check为true时，更新right，所以返回right
            /*
            「求最小」和二分查找求「排序数组中某元素的第一个位置」是类似的，
            按照红蓝染色法，左边是不满足要求的（红色），右边则是满足要求的（蓝色）。
            「求最大」的题目则相反，左边是满足要求的（蓝色），右边是不满足要求的（红色）。
            这会导致二分写法和上面的「求最小」有一些区别。
            判断是求最小还是求最大，看二分区间左右的颜色即可！
            以开区间二分为例：
            求最小：check(mid) == true 时更新 right = mid，反之更新 left = mid，最后返回 right。
            求最大：check(mid) == true 时更新 left = mid，反之更新 right = mid，最后返回 left。
            对于开区间写法，简单来说 check(mid) == true 时更新的是谁，最后就返回谁。
            相比其他二分写法，开区间写法不需要思考加一减一等细节
            */
        }
    };
}

// 套模板
namespace s2187m1
{   
    class Solution {
    public:
        long long minimumTime(vector<int>& time, int totalTrips) {

            auto check = [&](long long& t) ->bool {
                long long sum = 0;
                for (int x : time) {
                    sum += t / x;
                    if (sum >= totalTrips) {
                        return true;
                    }
                }
                return false;
                };

            int minTime = *min_element(time.begin(), time.end());
            long long left = minTime - 1;
            // 如果给的时间连最快的车都走不完一趟，那绝对不可能完成totalTrips，即left = minimun - 1
            // 在这个时间上减1就是不可能的最小的时间，开区间要无法满足条件的左端点，所以-1
            long long right = (long long)totalTrips * minTime;
            // 右端点也能优化，但是感觉比较复杂就没看了，暂时不追求那么快
            while (left + 1 < right) {
                long long mid = left + (right - left) / 2;
                if (check(mid)) {
                    right = mid;
                } else {
                    left = mid;
                }
            }
            return right;
        }
    };
}

// 模板题
namespace s1011m
{
    class Solution {
    public:
        int shipWithinDays(vector<int>& weights, int days) {
            auto check = [&](int cap) ->bool {
                int sum = 0;
                int currentDays = 1;
                for (int w : weights) {
                    sum += w;
                    if (sum <= cap) {
                       continue; // 当天还能接着装货
                    } else {
                        ++currentDays;
                        if (currentDays > days) {
                            return false;
                        }
                        sum = w;
                    }
                }
                return true;
            };
            long long left = *max_element(weights.begin(), weights.end()) - 1;
            long long right = accumulate(weights.begin(), weights.end(), 0);
            while (left + 1 < right) {
                long long mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}

// 继续复习x / y(upceil) == (x - 1)/y + 1 (downceil)，数据量大时，check函数的效率也很关键
namespace s875o1
{
    class Solution {
    public:
        int minEatingSpeed(vector<int>& piles, int h) {
            auto check = [&](int k) {// 数据量大的情况下，check函数的效率也很关键，
                int sumHours = piles.size();// 用公式推或直接理解（n堆必定要吃n小时）
                for (int p : piles) {
                    sumHours += (p - 1) / k;
                    if (sumHours > h) {
                        return false;
                    }
                }
                return true;
             };
            /*  我自己写的check，就超时了
            auto check = [&](int k) {
                int curHours = 0;
                for (int i : piles) {
                    ++curHours;
                    while (i > k) {
                        i -= k;
                        ++curHours;
                    }
                    if (curHours > h) {
                        return false;
                    }
                }
                return true;
             };
            */
            int left = 0;
            int right = *max_element(piles.begin(), piles.end());
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}

// 思维拓展：避免浮点数技巧（如果有两位小数，那么统一乘100变成int比较）
// 这个技巧使用起来有难度，而且容易错，现阶段不要求掌握，左右端点大些就大些吧，不搞花哨的
namespace s1870m
{   // 将最后一个元素的判断单独拿出来
    // 整体思路还是沿用int的做法
    class Solution {
    public:
        int minSpeedOnTime(vector<int>& dist, double hour) {
            int n = dist.size();
            if (hour <= n - 1) return -1;// 这里必须是 <= ，比如hour = 1, dist = [1, 1]
            // 除了最后一趟列车外，其他列车算上等待时间最少花费一个小时，而最后一趟列车花费时间是大于0的
            auto check = [&](int v) ->bool {
                int sum = n - 1;
                for (int i = 0; i < n - 1; ++i) {// 遍历n - 1个元素
                    sum += (dist[i] - 1) / v;
                    if (sum > hour) {
                        return false;
                    }
                }
                double last = sum + (double)dist.back() / v;// 单独判断最后一个元素
                return last > hour ? false : true;
                };
            int left = 0;
            int right = *max_element(dist.begin(), dist.end()) * 100;
            // 因为已经做了hour <= n - 1的判断， 所以此处hour >= n必然成立
            // 乘100是因为保留小数点后两位小时，当hour = 2.01, dist = [1, 1, 100000]时，right取最大为1e7
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
namespace s1870m2
{   // 加上一个特判，缩小右端点后提速明显
    class Solution {
        double decimal_safe_ceil(double x, int decimal_places = 2) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(decimal_places) << x;
            double rounded = std::stod(ss.str());// 通过字符串来回转换，截断掉小数后x位的数
            // 避免2.000001这样的数因为ceil转换成3
            return std::ceil(rounded);
        }
    public:
        int minSpeedOnTime(vector<int>& dist, double hour) {
            int n = dist.size();
            if (hour <= n - 1) return -1;// 这里必须是 <= ，比如hour = 1, dist = [1, 1]
            // 特判类似当hour = 2.01, dist = [1, 1, 100000]时的情况
            int maxDist = *max_element(dist.begin(), dist.end());
            if (hour > n - 1 && hour <= n) {
                int m = decimal_safe_ceil(dist[n - 1] / (hour - n + 1));//这里round或者ceil都不行，需要自己写函数
                // 而且这里可能还存在例子导致decimal_safe_ceil失效，最好还是别用这个方法了，不够严谨
                return max(maxDist, m);
            }

            auto check = [&](int v) ->bool {
                int sum = n - 1;
                for (int i = 0; i < n - 1; ++i) {// 遍历n - 1个元素
                    sum += (dist[i] - 1) / v;
                    if (sum > hour) {
                        return false;
                    }
                }
                double last = sum + (double)dist[n - 1] / v;// 单独判断最后一个元素
                return last > hour ? false : true;
                };

            int left = 0, right = maxDist;// 这里可以排除掉dist[n-1]在0.x小时内到走不完的情况，缩小右端点
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
namespace s1870o1
{   // 答案需满足 sumhour(n - 1) + dist[n-1]/v <= hour，hour最多有两位小数，可以两边同乘100，消除浮点数
    // 注意浮点数如2.01其实是2.009999... 这样的数，存在误差，所以还需要round（四舍五入）一下消除误
    // 因为题目说了只有两位小数，乘100后取round是安全的
    class Solution {
    public:
        int minSpeedOnTime(vector<int>& dist, double hour) {
            int n = dist.size();
            long long h100 = round(hour * 100); // 下面不会用到任何浮点数
            long long delta = h100 - (n - 1) * 100;
            if (delta <= 0) { // 无法到达终点
                return -1;
            }

            int max_dist = *max_element(dist.begin(), dist.end());
            if (h100 <= n * 100) { // 特判
                // 见题解中的公式
                return max(max_dist, (int)((dist.back() * 100 - 1) / delta + 1));
            }

            auto check = [&](int v) -> bool {
                long long t = 0;
                for (int i = 0; i < n - 1; i++) {
                    t += (dist[i] - 1) / v + 1;
                }
                return (t * v + dist.back()) * 100 <= h100 * v;
                };
            
            long long sum_dist = accumulate(dist.begin(), dist.end(), 0LL);// 也可以写reduce(C++17)
            int left = (sum_dist * 100 - 1) / h100; // 也可以初始化成 0（简单写法）
            int h = h100 / (n * 100);
            int right = (max_dist - 1) / h + 1; // 也可以初始化成 max_dist（简单写法）
            while (left + 1 < right) {
                int mid = (left + right) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
// ---------------------
// 【2.2】求最大：跟求最小差不多，注意左右边界的意义与求最小的问题是相反的(3)
/*

275.H 指数 II：给你一个整数数组 citations ，其中 citations[i] 表示研究者的第 i 篇论文被引用的次数，
    citations 已经按照 升序排列 。计算并返回该研究者的 h 指数。
    h 指数的定义：h 代表“高引用次数”（high citations），
    一名科研人员的 h 指数是指他（她）的 （n 篇论文中）至少 有 h 篇论文分别被引用了至少 h 次。
    请你设计并实现对数时间复杂度的算法解决此问题。

2226.每个小孩最多能分到多少糖果：给你一个 下标从 0 开始 的整数数组 candies 。数组中的每个元素表示大小为 candies[i] 的一堆糖果。
    你可以将每堆糖果分成任意数量的 子堆 ，但 无法 再将两堆合并到一起。
    另给你一个整数 k 。你需要将这些糖果分配给 k 个小孩，使每个小孩分到 相同 数量的糖果。
    每个小孩可以拿走 至多一堆 糖果，有些糖果可能会不被分配。
    返回每个小孩可以拿走的 最大糖果数目 。

1802.有界数组中指定下标处的最大值：给你三个正整数 n、index 和 maxSum 。
    你需要构造一个同时满足下述所有条件的数组 nums（下标 从 0 开始 计数）：
    nums.length == n
    nums[i] 是 正整数 ，其中 0 <= i < n
    abs(nums[i] - nums[i+1]) <= 1 ，其中 0 <= i < n-1
    nums 中所有元素之和不超过 maxSum
    nums[index] 的值被 最大化
    返回你所构造的数组中的 nums[index] 。
    注意：abs(x) 等于 x 的前提是 x >= 0 ；否则，abs(x) 等于 -x 。

*/
// ---------------------
// 模板题2：跟求最小大差不差
namespace s275o1
{   // 这个H指针越大越不容易满足条件，越小越容易满足，所以是求最大问题
    class Solution {
    public:
        int hIndex(vector<int>& citations) {
            int n = citations.size();
            auto check = [&](int h) ->bool {// 这道题check函数其实没必要，因为函数体其实只有citations[n - h] >= h一句话
                return (citations[n - h] >= h) ? true : false;
                };
            int left = 0, right = n + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;// 左边都是满足的，右边是不满足的，最后返回left(always true)
            }
            // 最后退出循环时满足 left + 1 = right，left处是true，right处是false，所以返回left
            return left;
        }
    };
}

// 要敢于用求和来缩小端点，accumulate和reduce函数的熟悉
namespace s2226m1
{
    class Solution {
    public:
        int maximumCandies(vector<int>& candies, long long k) {
            auto check = [&](int ans) ->bool {
                long long temp = k;// 注意按这个check方法必须复制一个temp，用mutable也没用，也要用long long
                for (int c : candies) {
                    temp -= c / ans;
                    if (temp <= 0) {
                        return true;
                    }
                }
                return false;
                };
            int left = 0, right = *max_element(candies.begin(), candies.end()) + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}
namespace s2226m2
{
    class Solution {
    public:
        int maximumCandies(vector<int>& candies, long long k) {
            auto check = [&](int ans) ->bool {
                long long temp = k;
                for (int c : candies) {
                    temp -= c / ans;
                    if (temp <= 0) {
                        return true;
                    }
                }
                return false;
                };
            long long candyMax = *max_element(candies.begin(), candies.end());
            long long candyAvg = accumulate(candies.begin(), candies.end(), 0LL) / k;
            // 求和然后算平均值的做法我第一遍就想到了，但是觉得求和一遍影响性能，麻烦
            // 但是只用一次求和就能大幅影响右端点，节省下来的时间更多！！！
            int left = 0, right = min(candyMax, candyAvg) + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}

// 两种等差数列求和公式，其他还是模板，要注意乘法和除法的顺序问题！
namespace s1802m
{
    class Solution {
    public:
        int maxValue(int n, int index, int maxSum) {
            // 长度为n的nums数组，元素为正整数，相邻整数间变化幅度为0或1
            // 元素之和不超过maxSum
            // nums[index]为最大值处
            // 返回最大值
            auto check = [=](int res) {
                long long ans = res;
                int lenL = index + 1;
                int lenR = n - index;
                long long sumL = 0, sumR = 0;
                // 求和可以写成一个函数来精简代码，我就没弄了
                if (lenL >= ans) {
                    sumL = (ans + 1) * ans / 2 + (lenL - ans);
                } else {
                    sumL = lenL * (2 * ans - lenL + 1) / 2;// /2的操作必须要放在最后，否则可能出现奇数除2然后截断，导致结果错误
                }
                if (lenR >= ans) {
                    sumR = (ans + 1) * ans / 2 + (lenR - ans);
                } else {
                    sumR = lenR * (2 * ans - lenR + 1) / 2;
                }
                int sum = sumL + sumR - ans;// 中间值计算了两遍，需要减掉一个
                return sum <= maxSum;
                };
            int left = 1, right = maxSum - n + 2;
            // int right = (2 * maxSum / n + n - 1) / 2 + 1; // 这种缩小端点的写法更复杂，但缩小的范围更多
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}
// ---------------------
// 【2.3】二分间接值(1)
/*

3143.正方形中的最多点数：给你一个二维数组 points 和一个字符串 s ，
    其中 points[i] 表示第 i 个点的坐标，s[i] 表示第 i 个点的 标签 。
    如果一个正方形的中心在 (0, 0) ，所有边都平行于坐标轴，且正方形内 不 存在标签相同的两个点，
    那么我们称这个正方形是 合法 的。
    请你返回 合法 正方形中可以包含的 最多 点数。
    注意：如果一个点位于正方形的边上或者在边以内，则认为该点位于正方形内。正方形的边长可以为零。

*/
// ---------------------
// 模板题3：不直接对答案进行二分，而是通过二分某一个值，更新答案，另有位运算判断是否一个元素是否在集合内的技巧
namespace s3143m1
{   // 纯笨比做法，哈希表+二分，时间复杂度O(Mlog(n)),M为1e9，n为points元素数量
    class Solution {
    public:
        int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
            // 中心在(0, 0)，边与坐标轴平行的正方形
            // 所有点的标签都不相同才算合法
            // 如果一个点位于正方形的边上或者在边以内，则认为该点位于正方形内。
            // 正方形的边长可以为零。
            // points 中的点坐标互不相同， s中只包含小写字母（即答案最多为26），1e5个元素，值为1e9
            int res = 0;
            auto check = [&](int halfedge) {
                unordered_map<char, int> map;
                for (int i = 0; i < points.size(); ++i) {
                    int edgex = abs(points[i][0]);
                    int edgey = abs(points[i][1]);
                    if (edgex <= halfedge && edgey <= halfedge) {
                        if (map.find(s[i]) != map.end()) {
                            return false;
                        }
                        map[s[i]] = 1;
                    }
                }
                res = max(res, (int)map.size());
                // 这里也可以直接写res = map.size()，因为更新res时一定是往更大的情况更新
                // 不可能往已经确定是蓝色的区间上查找
                return true;
                };
            int left = -1, right = 1e9 + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return res;
        }
    };
}
namespace s3143o1
{   // 用位运算替代哈希表查找，相较于m1大幅提速
    class Solution {
    public:
        //也可以手动实现popcnt，但每次都要遍历32遍，比库函数O(1)慢多了
        /*
        int popcount(unsigned int x) {
            int cnt = 0;
            while (x) {
            cnt += x & 1;
            x >>= 1;
            }
            return cnt;
        }
        */
        int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
            int res = 0;
            auto check = [&](int halfedge) {
                int tagSet = 0;// 32位，用低26位来存储26个小写字母
                for (int i = 0; i < points.size(); ++i) {
                    int edgex = abs(points[i][0]);
                    int edgey = abs(points[i][1]);
                    if (edgex <= halfedge && edgey <= halfedge) {
                        int c = s[i] - 'a';
                        if ((tagSet >> c) & 1) {// 先将tagSet向右移动c位，然后跟1进行与运算，如果答案是1，代表c已经在集合中
                            return false;       // tagSet >> c会生成一个临时的右移结果值对象，这里相当于(tagSet >> c) & 1
                        }
                        tagSet |= 1 << c;// 先将1向左移动c位，然后跟tagSet进行或运算，并将答案更新至tagSet
                    }
                }
                res = __popcnt(tagSet);// intrin.h，windows特有指令，计算bit位中有几位是1，复杂度O(1) population count
                // 如果是c++ 20可以用std::popcount，头文件为<bit>
                // GCC / Clang中对应的函数是__builtin_popcount()
                return true;
                };
            int left = -1, right = 1e9 + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return res;
        }
    };

}
namespace s3143o2
{   // 逆天思路，时间复杂度直接干到O(n)
    // min1存放所有标签下的最小的离原点的距离，min2为次小距离（min2只需要存一个全局最小的即可）
    // 遍历所有points，更新完min1，代表所有标签下的最小正方形
    // 对每个标签（小写字母）来说，对应的min1必须是小于其对应的min2（次小距离），否则就会出现重复
    // 那么想要计算所有标签的最大不重复的点数，所有满足要求的标签的对应min1都要小于全局最小的min2，满足的数量即为点数数量
    class Solution {
    public:
        int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
            int min2 = INT_MAX;
            vector<int> min1(26, INT_MAX);
            for (int i = 0; i < points.size(); ++i) {
                int d = max(abs(points[i][0]), abs(points[i][1]));
                int c = s[i] - 'a';
                if (d < min1[c]) {// d是目前最小的，那么min1[c]是次小的
                    min2 = min(min2, min1[c]);
                    min1[c] = d;
                }
                else {// d 可能是次小的
                    min2 = min(min2, d);// 细节，如果出现两个点都在同一个最小正方形内，比如(1,1),(1,-1)，此时d = min1[c]
                }    // 因此这里会将次小值min2更新成min1的值，那么在下面的遍历时就会把两个重复点都排除掉了
            }
            int ans = 0;
            for (int d : min1) {
                ans += d < min2;// 比如，在这里排除(1,1),(1,-1)
            }
            return ans;
        }
    };
}
// ---------------------
// 【2.4】最小化最大值：本质是二分答案求最小(4)
// 开始与贪心等其他算法紧密联系，难度提高，关键是将问题进行转化
/*

410.分割数组的最大值：给定一个非负整数数组 nums 和一个整数 k ，你需要将这个数组分成 k 个非空的连续子数组，
    使得这 k 个子数组各自和的最大值 最小。返回分割后最小的和的最大值。子数组 是数组中连续的部份。

2064.分配给商店的最多商品的最小值：给你一个整数 n ，表示有 n 间零售商店。总共有 m 种产品，
每种产品的数目用一个下标从 0 开始的整数数组 quantities 表示，其中 quantities[i] 表示第 i 种商品的数目。
你需要将 所有商品 分配到零售商店，并遵守这些规则：
一间商店 至多 只能有 一种商品 ，但一间商店拥有的商品数目可以为 任意 件。
分配后，每间商店都会被分配一定数目的商品（可能为 0 件）。
用 x 表示所有商店中分配商品数目的最大值，你希望 x 越小越好。
也就是说，你想 最小化 分配给任意商店商品数目的 最大值 。
请你返回最小的可能的 x 。

1760.袋子里最少数目的球：给你一个整数数组 nums ，其中 nums[i] 表示第 i 个袋子里球的数目。
同时给你一个整数 maxOperations 。你可以进行如下操作至多 maxOperations 次：
选择任意一个袋子，并将袋子里的球分到 2 个新的袋子中，每个袋子里都有 正整数 个球。
比方说，一个袋子里有 5 个球，你可以把它们分到两个新袋子里，分别有 1 个和 4 个球，或者分别有 2 个和 3 个球。
你的开销是单个袋子里球数目的 最大值 ，你想要 最小化 开销。
请你返回进行上述操作后的最小开销。

1631.最小体力消耗路径：你准备参加一场远足活动。给你一个二维 rows x columns 的地图 heights ，
    其中 heights[row][col] 表示格子 (row, col) 的高度。一开始你在最左上角的格子 (0, 0) ，
    且你希望去最右下角的格子 (rows-1, columns-1) （注意下标从 0 开始编号）。
    你每次可以往 上，下，左，右 四个方向之一移动，你想要找到耗费 体力 最小的一条路径。
    一条路径耗费的 体力值 是路径上相邻格子之间 高度差绝对值 的 最大值 决定的。
    请你返回从左上角走到右下角的最小 体力消耗值 。

*/
// ---------------------
// 模板题4：贪心 + 二分查找
namespace s410m
{   // 难点在于理解题意，对单调性进行挖掘
    // 属于二分答案的一种，检查值越小，需要的分段数越多，越难满足；
    // 检查值越大，需要的分段数越少，越容易满足，极限条件下k = 1, 检查值为nums所有元素之和
    // 另一点是check函数的理解：为了让（各子数组和的最大值）最小，那么在模拟划分时，
    // 每个分段下需要在满足给定检查值条件尽可能容纳更多的元素，也即贪心，想要实现只需从左到右遍历即可
    class Solution {
    public:
        int splitArray(vector<int>& nums, int k) {
            auto check = [&](int ans) ->bool {
                int cnt = 1;
                int sum = 0;
                for (int i : nums) {
                    sum += i;
                    if (sum > ans) {// 当前分段下塞不下了
                        cnt++;// 新划分一段
                        if (cnt > k) {// 不能继续划分，已经超过分段数上限
                            return false;// 超过上限了还没把所有元素都装完，那么意味着当前检查值无法满足
                        }
                        sum = i;
                    }
                }
                return true;
            };
            int left = *max_element(nums.begin(), nums.end()) - 1;
            int right = accumulate(nums.begin(), nums.end(), 0);
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };

}

// 套模板
namespace s2064m
{  
    class Solution {
    public:
        int minimizedMaximum(int n, vector<int>& quantities) {
            // 允许部分商店没有商品，每间商店一种商品
            auto check = [&](int ans) {
                int cnt = 0;
                for (int i : quantities) {
                    cnt += (i - 1) / ans + 1;
                    if (cnt > n) {
                        return false;
                    }
                }
                return true;
                };
            int left = 0;
            int right = *max_element(quantities.begin(), quantities.end());
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}

// 套模板
namespace s1760m
{
    class Solution {
    public:
        int minimumSize(vector<int>& nums, int maxOperations) {
            auto check = [&](int ans) {
                int cnt = 0;
                for (int i : nums) {
                    //if (i <= ans) {
                    //    continue; // 这段判断可以不要，因为如果i <= ans，那么(i - 1) / ans必定为0
                    //}
                    cnt += (i - 1) / ans;
                    if (cnt > maxOperations) {
                        return false;// 这里也可以不提前退出，将cnt改为long long类型，然后遍历完整个nums后再判断cnt的大小
                    }
                }
                return true;
                };
            int left = 0;
            int right = *max_element(nums.begin(), nums.end());
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}

// 图论（BFS + DFS) + 二分查找，暂时只看了二分的做法，学了其他图论做法以后再复刷！
// 难度分超过1900的题目开始，基本单一算法无法解决了，要进行综合
// o1：BFS层序遍历（队列） + 二分答案，计算矩阵中下标的小技巧
namespace s1631o1
{
    class Solution {
    public:
        int minimumEffortPath(vector<vector<int>>& heights) {
            int dirs[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
            int m = heights.size();
            int n = heights[0].size();
            if (m == 1 && n == 1) return 0;// 必需单独对1个元素的情况进行判断，因为起点即是终点
            // 否则一直返回true，最后返回值为1e6
            auto check = [&](int diff) ->bool {
                vector<bool> visited(m * n);
                queue<int> q;
                q.push(0);
                while (!q.empty()) {
                    int cur = q.front();
                    q.pop();
                    if (visited[cur]) continue;
                    visited[cur] = true;
                    int row = cur / n, col = cur % n;// 这个学习了，计算矩阵中下标的做法
                    for (int i = 0; i < 4; ++i) {
                        int nrow = row + dirs[i][0];
                        int ncol = col + dirs[i][1];
                        int index = nrow * n + ncol;
                        if (nrow < 0 || nrow >= m || ncol < 0 || ncol >= n || visited[index]
                            || abs(heights[row][col] - heights[nrow][ncol]) > diff) {
                            continue;
                        }
                        if (index == m * n - 1) return true;
                        q.push(index);
                    }
                }
                return false;
             };

            int left = -1, right = 1e6;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
// ---------------------
// 【2.5】最大化最小值：同上(3)
/*

3281.范围内整数的最大得分：给你一个整数数组 start 和一个整数 d，代表 n 个区间 [start[i], start[i] + d]。
    你需要选择 n 个整数，其中第 i 个整数必须属于第 i 个区间。所选整数的 得分 定义为所选整数两两之间的 最小 绝对差。
    返回所选整数的 最大可能得分 。

2517.礼盒的最大甜蜜度：给你一个正整数数组 price ，其中 price[i] 表示第 i 类糖果的价格，另给你一个正整数 k 。
    商店组合 k 类 不同 糖果打包成礼盒出售。礼盒的 甜蜜度 是礼盒中任意两种糖果 价格 绝对差的最小值。
    返回礼盒的 最大 甜蜜度。


1552.两球之间的磁力：在代号为 C-137 的地球上，Rick 发现如果他将两个球放在他新发明的篮子里，它们之间会形成特殊形式的磁力。
    Rick 有 n 个空的篮子，第 i 个篮子的位置在 position[i] ，Morty 想把 m 个球放到这些篮子里，使得任意两球间 最小磁力 最大。
    已知两个球如果分别位于 x 和 y ，那么它们之间的磁力为 |x - y| 。
    给你一个整数数组 position 和一个整数 m ，请你返回最大化的最小磁力。

*/
// ---------------------
// 模板题5：贪心+二分答案，题目转化->给定score，能否从每个区间各选一个数，使得任意两数之差的最小值[至少]为score
namespace s3281o1
{   // 因为通过贪心将问题进行了转化，所以此时left和right端点的选择就不是按照原题目来了
    // 如果按原来题目的定义，左端点0可能为答案之一，也即score连0可能都满足不了，比如{1,10,20},d = 1，此时也无法进行二分查找
    // 问题转化后，则一定可以选出 n 个数，使其两两之差都大于等于 0，这是一定能确定的，所以开区间左端点取0
    class Solution {
    public:
        int maxPossibleScore(vector<int>& start, int d) {
            sort(start.begin(), start.end());
            int n = start.size();
            auto check = [&](int ans) ->bool {
                int left = start[0];
                for (int i = 1; i < n; ++i) {
                    if (start[i] + d - left - ans < 0) {
                        return false;
                    }
                    left = max(start[i], left + ans);
                }
                return true;
                };
            int left = 0;
            // start.back() + d >= start[0] + (n - 1) * score
            int right = (start[n - 1] + d - start[0]) / (n - 1) + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}

// 类似s3281，基于贪心思想，将题目的要求进行转化
namespace s2517m1
{
    class Solution {
    public:
        int maximumTastiness(vector<int>& price, int k) {
            sort(price.begin(), price.end());
            int n = price.size();
            auto check = [&](int diff) {
                int count = 1;  // 已选 1 个（price[0]）
                int prev = price[0];
                for (int j = 1; j < n; ++j) {
                    if (price[j] - prev >= diff) {
                        prev = price[j];
                        if (++count >= k) return true;
                    }
                }
                return false;
                };

            int left = 0;
            int right = (price[n - 1] - price[0]) / (k - 1) + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}

// 同s2517，就当作是复习和换个角度理解题意
namespace s1552m1
{
    class Solution {
    public:
        int maxDistance(vector<int>& position, int m) {
            sort(position.begin(), position.end());
            int n = position.size();
            auto check = [&](int diff) -> bool {
                int pre = position[0];
                int cnt = 1;
                for (int i = 1; i < n; ++i) {
                    if (position[i] >= diff + pre) {
                        pre = position[i];
                        if (++cnt >= m) {
                            return true;
                        }
                    }
                }
                return false;
                };
            int left = 1;
            int right = (position[n - 1] - position[0]) / (m - 1) + 1;
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? left : right) = mid;
            }
            return left;
        }
    };
}
// ---------------------
// 【2.6】第K小/大(3)
/*
668.乘法表中第k小的数：几乎每一个人都用 乘法表。但是你能在乘法表中快速找到第 k 小的数字吗？
乘法表是大小为 m x n 的一个整数矩阵，其中 mat[i][j] == i * j（下标从 1 开始）。
给你三个整数 m、n 和 k，请你在大小为 m x n 的乘法表中，找出并返回第 k 小的数字。

378.有序矩阵中第 K 小的元素：给你一个 n x n 矩阵 matrix ，其中每行和每列元素均按升序排序，找到矩阵中第 k 小的元素。
请注意，它是 排序后 的第 k 小元素，而不是第 k 个 不同 的元素。你必须找到一个内存复杂度优于 O(n2) 的解决方案。

719.找出第 K 小的数对距离：数对 (a,b) 由整数 a 和 b 组成，其数对距离定义为 a 和 b 的绝对差值。
给你一个整数数组 nums 和一个整数 k ，数对由 nums[i] 和 nums[j] 组成且满足 0 <= i < j < nums.length 。
返回 所有数对距离中 第 k 小的数对距离。
*/
// ---------------------
// 模板题6：求第K小/大的模板
namespace s668o1
{   /*
    第 k 小/大问题的通用转化方法：
    第 k 小等价于：求最小的 x，满足 ≤x 的数至少有 k 个。（注意是至少不是恰好）
    第 k 大等价于：求最大的 x，满足 ≥x 的数至少有 k 个。
    */
    // 时间复杂度：O(mlog(mn))。也可以在 m>n 时交换 m 和 n
    class Solution {
    public:
        int findKthNumber(int m, int n, int k) {
            // 问题转化为[1, mn]中 <= x的至少有k个
            // 给定整数 x，统计乘法表中 ≤x 的元素个数 cnt，判断是否满足 cnt≥k
            // 对于基本类型，如int, bool, double，用值捕获安全性更高，且几乎无拷贝导致的性能损失
            // 本题中外部变量只有m和n两个int常量，推荐使用值捕获[=]
            auto check = [=](int x) {
                int cnt = 0;
                for (int i = 1; i <= m; ++i) {
                    // 逐行扫描乘法表，每行中满足<=x的个数为x / i个，但个数不能超过列数
                    // 第i行的数都是i的倍数，x / i之后相当于去掉了倍数，且是向下去整的，因为每行去掉倍数后都是从1开始
                    // 所以x / i正好因为向下取整能代表该行中 <= x的数的个数，如果去掉倍数后还大于n，那就说明整行都比x小
                    cnt += min(x / i, n);
                    if (cnt >= k) return true;
                }
                return false;
                };

            int left = 0;//一定check为false，开区间
            int right = m * n;//一定check为true
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
            /*
            问：为什么二分结束后，答案 ans 一定在乘法表中？
            答：反证法。假设 ans 不在乘法表中，这意味着乘法表中第 k 小的数比 ans 小，
            或者说 ≤ans - 1。换句话说，≤ans - 1 的数有 k 个，即 check(ans - 1)=true。
            但根据循环不变量，二分结束后 ans = right = left + 1, check(ans - 1) = check(left) = false，矛盾。
            故原命题成立。      
            */
        }
    };
}

namespace s378m1
{   // check函数里逐行使用upper_bound，时间复杂度n * logn * logU，U是二分答案初始左右端点间距
    // 这种解法慢了，check()的时间复杂度是O(nlogn)，o1可以实现O(n)
    class Solution {
    public:
        int kthSmallest(vector<vector<int>>& matrix, int k) {
            // <= x 有k 个
            int n = matrix.size();
            auto check = [&](int x) {
                int cnt = 0;
                for (int i = 0; i < n; ++i) {
                    vector<int>& vec = matrix[i];
                    auto it = upper_bound(vec.begin(), vec.end(), x);
                    cnt += it - vec.begin();
                }
                return cnt >= k;
                };
            int left = matrix[0][0] - 1;
            int right = matrix[n - 1][n - 1];
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
namespace s378o1
{   // check函数的双指针优化，将总时间复杂度降为 n * logU，空间复杂度为O(1)
    // Q：搜索出的答案为什么一定在矩阵里？
    // A：反证法，假如ans不在矩阵内：
    // 现在有：对x = ans，矩阵有k个 <= ans的数
    // 又因为ans不在矩阵内，所以对ans - 1，可得矩阵有k个<= ans - 1的数
    // 但是ans - 1等价于left, ans 等价于right，根据循环不变量，可知对ans - 1，check结果为false
    // 矛盾，所以ans一定在矩阵内
    class Solution {
    public:
        int kthSmallest(vector<vector<int>>& matrix, int k) {
            // 求第K小：问题等价于 求最小的x，使满足 <= x 的数至少有 k 个
            // 求第K大：问题等价于 求最大的x，使满足 >= x 的数至少有 k 个
            // x的取值具有二分性
            // 以求第K小为例，x越大越容易满足找到k个数，x越小越不容易找到k个数
            int n = matrix.size();

            auto check = [&](int x) {
                int cnt = 0;
                int i = 0, j = n - 1;
                while (i < n && j >= 0) {
                    if (matrix[i][j] <= x) {
                        cnt += j + 1;// 这一行全都满足
                        ++i;
                    }
                    else {
                        --j;// 这一列全都不满足
                    }
                    if (cnt >= k) return true;
                }
                return false;
                };

            // 开区间二分答案
            int left = matrix[0][0] - 1;        // 不可能满足
            int right = matrix[n - 1][n - 1];   // 一定满足
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}

namespace s240o1
{   // s378o1中双指针优化写法的由来
    class Solution {
    public:
        bool searchMatrix(vector<vector<int>>& matrix, int target) {
            int m = matrix.size();
            int n = matrix[0].size();

            int i = 0, j = n - 1;// 从右上角开始
            while (i < m && j >= 0) {// 还有剩余元素
                if (matrix[i][j] == target) {
                    return true;
                }
                if (matrix[i][j] > target) {
                    --j;// 这一列剩余元素全部大于 target，排除
                }
                else {
                    ++i;// 这一行剩余元素全部小于 target，排除
                }
            }
            return false;
        }
    };
}

namespace s719m1
{   // 当时可能觉得反着来更容易，算的是 > dist的个数，总数对数为n * (n - 1) / 2，那么新的target就是总数-k
    class Solution {
    public:
        int smallestDistancePair(vector<int>& nums, int k) {
            // <= dist的至少有k个
            // > dist的至多有target个
            sort(nums.begin(), nums.end());
            int n = nums.size();
            int target = n * (n - 1) / 2 - k;
            auto check = [&](int dist) {
                int cnt = 0, j = 0;
                for (int i = 0; i < n; i++) {
                    while (j < n && nums[j] - nums[i] <= dist)
                        j++;
                    cnt += n - j; // [j, n-1] 都 > dist
                }
                return cnt <= target;
                };
            int left = -1;
            int right = nums.back() - nums[0];
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };
}
namespace s719o1 
{   // o1和m1思路基本一致，都是二分答案+双指针check，随便哪个都行
    class Solution {
    public:
        int smallestDistancePair(vector<int>& nums, int k) {
            // <= dist的至少有k个
            sort(nums.begin(), nums.end());
            int n = nums.size();
            auto check = [&](int dist) {// check函数为双指针
                int cnt = 0, i = 0;
                for (int j = 0; j < n; j++) {// 遍历右端点，直接计算<= dist的个数
                    while (nums[j] - nums[i] > dist)
                        ++i;
                    cnt += j - i;
                }
                return cnt >= k;
                };
            int left = -1;
            int right = nums.back() - nums[0];
            while (left + 1 < right) {
                int mid = left + (right - left) / 2;
                (check(mid) ? right : left) = mid;
            }
            return right;
        }
    };

}
// ---------------------