#pragma once
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
using namespace std;

// 问题待定：
/*
1.
*/

/*
模板题：
1.0-1背包思想见o1，n个物品的取舍，取模滚动数组空间优化，倒序遍历一个数组优化（及翻译成递推的边界条件的启发）：494
2.完全背包模板，时刻记住背包题有两个属性：占用的体积和价值，对比题干然后将问题进行转化：322
3.本质还是完全背包的模板，但是这道题可以作为避免递推中间态溢出的启发：518
4.LCS问题，当前问题可以变成三个子问题，根据s[i]与s[j]进一步分类讨论，比背包复杂一点的DP问题：1143
5.LCS进阶，难点在于子问题的分析，递归和记忆化搜索努力点还是能写出来，但递推和空间优化开始又变得抽象：115
6.LIS问题，有记忆化搜索 / 递推，转换成LCS问题，二分 + 贪心三种思路：300
*/

// 动态规划：背包 + 经典线性DP

// 三、背包()
// 【3.1】0-1背包 (2)
// 每个物品只能选一次，即要么选，要么不选，选或不选思想的代表
/*
494.目标和：给你一个非负整数数组 nums 和一个整数 target 。
向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数，可以构造一个 表达式 ：
例如，nums = [2, 1] ，可以在 2 之前添加 '+' ，在 1 之前添加 '-' ，然后串联起来得到表达式 "+2-1" 。
返回可以通过上述方法构造的、运算结果等于 target 的不同 表达式 的数目。

416.分割等和子集：给你一个 只包含正整数 的 非空 数组 nums 。
请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。
*/
// ---------------------
// 模板题1：0-1背包思想见o1，n个物品的取舍，取模滚动数组空间优化，倒序遍历一个数组优化（及翻译成递推的边界条件的启发）
// 如果是至多装capacity或至少装capacity，只需要改动f[0]（边界条件）和数组中填充的初始元素，具体看视频精讲18的末尾部分
// 至多装和至少装都是进阶题，要么冷门，要么难度分超过2000了，可以暂时战略性放弃，时间紧可以只掌握恰好装一种
// 如果是至多装求方案数，那么递归边界条件变成i < 0时直接return 1，f[]全部填充成1，只要能到i < 0都能视为一种方案（能到i < 0，c都是>=0的，背包还有容量，算做可行方案）
// 如果是至少装求方案数，那么递归边界条件变成i < 0时return c <= 0，同时只能不选的分支删除，同时改变遍历的范围（这个稍微难点）
namespace s494m1
{   // 最简单能想到的dfs做法，但是时间复杂度超了，O(2^n)
    class Solution {
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int n = nums.size();
            int ans = 0;

            function<void(int, int)> dfs = [&](int i, int sum) {
                if (i == n) {
                    if (sum == target) {
                        ++ans;
                    }
                    return;
                }

                dfs(i + 1, sum - nums[i]);
                dfs(i + 1, sum + nums[i]);
                };
            dfs(0, 0);

            return ans;
        }
    };
}
namespace s494o1
{   /* 将问题转换为0 - 1背包：
     正数之和为p，负数之和为q（绝对值），原始数组元素之和为S
     那么有p + q = S，依照题意p - q = target
     可得：p = (S + target) / 2
           q = (S - target) / 2
     所以问题转换为，从nums数组中有多少种选择元素的方案，使其和为p或q（也即背包的容量）
     原始问题得加上正负符号，转换之后直接用原值即可，还能通过正数的性质进行剪枝

     背包容量越小越容易完成计算，所以选p和q之中更小的那个数作为背包容量，也即(S - |target|) / 2
     此时需要考虑两件事，特判后加快速度：
        1.因为nums中元素都是非负数，所以背包容量也应该是非负的，所以如果S - |target| < 0，可以直接返回0，方案数为0；
          注意，等于0是可以的，因为有值为0的元素
        2.如果S - |target|是奇数，因为nums中元素都是整数，无法得到0.5的计算结果，所以也可以直接返回0，方案数为0。
     剩下的就是标准的0-1背包中的一种变形了：恰好装capacity，求方案数/最小/最大价值和（其他两种是至多装capacity和至少装）
     dfs(i, j) = dfs(i - 1, j) + dfs(i - 1, capacity - nums[i]) 
     dfs(i, j)表示到nums[i]为止的方案数
    */
    // 下面是一个没加记忆化搜索的版本
    class Solution {
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int n = nums.size();
            int sum = accumulate(nums.begin(), nums.end(), 0) - abs(target);
            if (sum < 0 || sum % 2 == 1) {
                return 0;
            }
            int capacity = sum / 2;

            // 这里不推荐function写法，还是改成lambda或单独的成员函数
            auto dfs = [&](auto&& dfs, int i, int capacity) {
                if (i < 0) {
                    return int(capacity == 0);// 所有元素检查完之后，看看capacity是否恰好为0，容量全部用完
                }
                /*
                这里不能提前根据capacity == 0返回，比如：
                if (capacity == 0) return 1;
                if (i < 0) return 0;
                这样会忽略掉剩下的元素中还有0的情况，因为选不选0都不影响capacity，是可行的方案
                举一个极端的例子：nums[1, 0], target = 1
                按照代码，初始capacity = 0，此时dfs(1, 0) = dfs(0, 0) + dfs(0, 0 - 0)
                第一个dfs(0,0)代表不选0，第二个dfs(0, 0)代表选0
                分别对应{},{0}这两种情况（空集也算方案！！！），如果提前根据容量为0返回，那么会漏掉情况
                */
                if (nums[i] > capacity) {
                    return dfs(dfs, i - 1, capacity);// 当前背包容量装不下当前元素，只能不选
                }
                return dfs(dfs, i - 1, capacity) + dfs(dfs, i - 1, capacity - nums[i]);
                };

            return dfs(dfs, n - 1, capacity);
        }
    };
}
namespace s494o2
{   // 增加记忆化搜索
    class Solution {
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int sum = accumulate(nums.begin(), nums.end(), 0) - abs(target);
            if (sum < 0 || sum % 2 == 1) {
                return 0;
            }
            int capacity = sum / 2;
            int n = nums.size();
            vector<vector<int>> cache(n, vector<int>(capacity + 1, -1));

            function<int(int, int)> dfs = [&](int i, int c) {
                if (i < 0) {
                    return int(c == 0);
                }

                if (cache[i][c] != -1) {
                    return cache[i][c];
                }

                if (nums[i] > c) {
                    cache[i][c] = dfs(i - 1, c);
                }
                else {
                    cache[i][c] = dfs(i - 1, c) + dfs(i - 1, c - nums[i]);
                }
                return cache[i][c];
                };

            return dfs(n - 1, capacity);
        }
    };
}
namespace s494o3
{   // 改成递推
    // f[i + 1][c] = f[i][c] + f[i][c - nums[i]]
    class Solution {
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int sum = accumulate(nums.begin(), nums.end(), 0) - abs(target);
            if (sum < 0 || sum % 2 == 1) {
                return 0;
            }
            int m = sum / 2; // m为背包容量capacity
            int n = nums.size();

            vector<vector<int>> f(n + 1, vector<int>(m + 1));
            // if (i < 0) return int(c == 0);
            // 之前的边界条件是上式，因为i整体加了1
            // 那么此时边界变成i == 0 && c == 0
            // 也即f[0][0] = 1
            // 这道题的递推数组的初始化没那么容易直接想，可以换个角度
            // 从记忆化搜索的边界条件直接翻译过来
            f[0][0] = 1;

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= m; ++c) {
                    if (nums[i] > c) {
                        f[i + 1][c] = f[i][c];
                    }
                    else {
                        f[i + 1][c] = f[i][c] + f[i][c - nums[i]];
                    }
                }
            }
            return f[n][m];
        }
    };
}
namespace s494o4
{   // 空间优化：两个数组（滚动数组）
    class Solution {
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int sum = accumulate(nums.begin(), nums.end(), 0) - abs(target);
            if (sum < 0 || sum % 2 == 1) {
                return 0;
            }
            int m = sum / 2; // m为背包容量capacity
            int n = nums.size();

            vector<vector<int>> f(2, vector<int>(m + 1));
            f[0][0] = 1;

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= m; ++c) {
                    if (nums[i] > c) {
                        // 通过取模，让f[0]和f[1]交替被覆盖
                        f[(i + 1) % 2][c] = f[i % 2][c];
                    }
                    else {
                        f[(i + 1) % 2][c] = f[i % 2][c] + f[i % 2][c - nums[i]];
                    }
                }
            }
            // 最后一次覆盖是i = n - 1时，求得是f[n % 2][c]，所以返回f[n % 2][m]
            return f[n % 2][m];
        }
    };
}
namespace s494o5
{   // 空间优化：一个数组（倒序遍历，防止覆盖，和s120m3自己写的那次类似）
    // f[i + 1][c] = f[i][c] + f[i][c - nums[i]]
    // 简化成这样：f[c] = f[c] + f[c - nums[i]]   （f[i + 1]和f[i]保存在同一个数组中，所以去掉的参数是i）
    class Solution { 
    public:
        int findTargetSumWays(vector<int>& nums, int target) {
            int sum = accumulate(nums.begin(), nums.end(), 0) - abs(target);
            if (sum < 0 || sum % 2 == 1) {
                return 0;
            }
            int m = sum / 2; // m为背包容量capacity

            vector<int> f(m + 1);
            f[0] = 1;

            for (int x : nums) {
                // 因为用不到i来标记数组了，所以可以直接用ranged for loop
                // 同时可以直接将nums[i]与c的条件判断简化到for循环条件里
                // 这里很直观，因为用到了f[c - x]，所以c的循环条件就是c >= x，防止下标越界
                for (int c = m; c >= x; --c) {
                    f[c] += f[c - x];
                }
            }
            return f[m];
        }
    };
}

// 和s494几乎一样，算是套模板吧，但m5一个数组的优化灵神写的还是比我好多了，避免了不少可以提前返回的无用循环
namespace s416m1
{   // 老规矩，先上一个超时的递归写法看看实力
    class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 2 == 1) {
                return false;
            }

            int m = sum / 2;
            int n = nums.size();
            function<bool(int, int)> dfs = [&](int i, int s) {
                if (i < 0) {
                    return s == m;
                }
                if (s > m) {
                    return false;
                }
                return dfs(i - 1, s) || dfs(i - 1, s + nums[i]);
                };
            return dfs(n - 1, 0);
        }
    };
}
namespace s416m2
{   // 相当于背包容量 = sum / 2
    // 增加记忆化搜索后的背包递归写法   
    // 但现在这题力扣记忆化搜索也超时了
    class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 2 == 1) {
                return false;
            }

            int m = sum / 2;
            int n = nums.size();
            vector<vector<int>> cache(n, vector<int>(m + 1, -1));

            auto dfs = [&](auto&& dfs, int i, int c)->bool {
                if (c == 0) return true;    // 这里提前返回是没问题的
                if (i < 0) return false;
                int& res = cache[i][c];
                if (res != -1) {
                    return res;
                }
                if (c < nums[i]) {
                    return res = dfs(dfs, i - 1, c);
                }
                return dfs(dfs, i - 1, c) || dfs(dfs, i - 1, c - nums[i]);
                };

            return dfs(dfs, n - 1, m);
        }
    };
}
namespace s416m3
{   // 改成递推
    class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 2 == 1) {
                return false;
            }

            int m = sum / 2;
            int n = nums.size();
            vector<vector<bool>> f(n + 1, vector<bool>(m + 1));
            f[0][0] = true;

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= m; ++c) {
                    if (c < nums[i]) {
                        f[i + 1][c] = f[i][c];
                    }
                    else {
                        f[i + 1][c] = f[i][c] || f[i][c - nums[i]];
                    }
                }
            }
            return f[n][m];
        }
    };
}
namespace s416m4
{   // 空间优化滚动数组
    class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 2 == 1) {
                return false;
            }

            int m = sum / 2;
            int n = nums.size();
            vector<vector<bool>> f(2, vector<bool>(m + 1));
            f[0][0] = true;

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= m; ++c) {
                    if (c < nums[i]) {
                        f[(i + 1) % 2][c] = f[i % 2][c];
                    }
                    else {
                        f[(i + 1) % 2][c] = f[i % 2][c] || f[i % 2][c - nums[i]];
                    }
                }
            }
            return f[n % 2][m];
        }
    };
}
namespace s416m5
{   // 空间优化一个数组
    class Solution {
    public:
        bool canPartition(vector<int>& nums) {
            int sum = accumulate(nums.begin(), nums.end(), 0);
            if (sum % 2 == 1) {
                return false;
            }
            int m = sum / 2;

            vector<bool> f(m + 1);
            // vector<int8_t> f(m + 1); // better
            f[0] = true;

            for (int x : nums) {
                for (int c = m; c >= x; --c) {
                    f[c] = f[c] || f[c - x];
                }
                if (f[m]) {
                    return true;// 如果提前找到了方案可以提前返回，一旦f[m]变成了true，后面都是true了
                }
            }

            return false;
            /*  还有一种剪枝优化，内层循环的初始值不需要一直保持为m
            设ranged for loop中的int x的累加和为s2，如果s2比m都要小，那么在(s2, m]范围内的f[c]一定是false的
            不可能在当前遍历到的int x中找到一种方案，恰好占据c的背包空间，不需要计算
            所以可以将内层的起点设为s2 与 m之间较小的那个

            int s2 = 0;
            for (int x : nums) {
                s2 = min(s2 + x, m);
                for (int c = s2; c >= x; --c) {
                    f[c] = f[c] || f[c - x];
                }
                if (f[m]) {
                    return true;
                }
            }
            */
            // return false;
        }
    };
}
// ---------------------
// 【3.2】完全背包(3)
// n个物品变成n种物品，每种物品可以无限重复选，有点像回溯s39，代码模板和0-1背包类似
/*
322.零钱兑换：给你一个整数数组 coins ，表示不同面额的硬币；以及一个整数 amount ，表示总金额。
计算并返回可以凑成总金额所需的 最少的硬币个数 。如果没有任何一种硬币组合能组成总金额，返回 -1 。
你可以认为每种硬币的数量是无限的。

518.零钱兑换 II：给你一个整数数组 coins 表示不同面额的硬币，另给一个整数 amount 表示总金额。
请你计算并返回可以凑成总金额的硬币组合数。如果任何硬币组合都无法凑出总金额，返回 0 。
假设每一种面额的硬币有无限个。 题目数据保证结果符合 32 位带符号整数。

279.完全平方数：给你一个整数 n ，返回 和为 n 的完全平方数的最少数量 。
完全平方数 是一个整数，其值等于另一个整数的平方；换句话说，其值等于一个整数自乘的积。
例如，1、4、9 和 16 都是完全平方数，而 3 和 11 不是。
*/
// ---------------------
// 模板题2：时刻记住背包题有两个属性：占用的体积和价值，对比题干然后将问题进行转化
// 如果是至多装capacity或至少装capacity，只需要改动f[0]（边界条件）和数组中填充的初始元素或是遍历的范围
namespace s322o1
{   // 背包的容量可以看成总金额，那每种硬币的面值就是占用的体积，价值可以设为1
    // 问题转化为在给定背包容量下，恰好装capacity时的求最小价值和    
    class Solution {
    public:
        int coinChange(vector<int>& coins, int amount) {
            // dfs(i, c)代表从前i个物品，在剩余容量c下，能获得的最小价值和
            // 转化成了求最小价值和，边界条件则是当遍历完数组后，正好装满了背包，此时剩余能得到的最小价值和为0
            // 不过由于没有记忆化搜索，当前代码是超时的
            int n = coins.size();

            auto dfs = [&](auto&& dfs, int i, int c) {
                if (i < 0) {
                    return c == 0 ? 0 : (amount + 1);// amount + 1等价于INT_MAX，但是后面可能+1越界，所以用amount + 1
                    // 假如全选数值为1的硬币，最多也才要用amount枚硬币，所以amount + 1枚硬币一定会被下面min排除
                }
                /*
                也可以提前返回，避免很多不必要的递归：
                if (c == 0) return 0;
                if (i < 0) return amount + 1;
                */
                if (c < coins[i]) {
                    return dfs(dfs, i - 1, c);
                }
                return min(dfs(dfs, i - 1, c), dfs(dfs, i, c - coins[i]) + 1);
                };
            int ans = dfs(dfs, n - 1, amount);
            return ans > amount ? -1 : ans;
        }
    };
}
namespace s322o2
{   // 记忆化搜索
    class Solution {
    public:
        int coinChange(vector<int>& coins, int amount) {
            int n = coins.size();
            vector<vector<int>> cache(n, vector<int>(amount + 1, -1));

            function<int(int, int)> dfs = [&](int i, int c) {
                if (i < 0) {
                    return c == 0 ? 0 : (amount + 1);
                }
                if (cache[i][c] != -1) {
                    return cache[i][c];
                }
                if (c < coins[i]) {
                    cache[i][c] = dfs(i - 1, c);// 只能不选
                }
                else {                            // 选 vs 不选
                    cache[i][c] = min(dfs(i - 1, c), dfs(i, c - coins[i]) + 1);
                }
                return cache[i][c];
                };

            int ans = dfs(n - 1, amount);
            return ans > amount ? -1 : ans;
        }
    };
}
namespace s322o3
{
    class Solution {
    public:
        // f[i][c] = min(f[i - 1][c], f[i][c - coins[i]] + 1)
        int coinChange(vector<int>& coins, int amount) {
            int n = coins.size();
            int upper = amount + 1;
            vector<vector<int>> f(n + 1, vector<int>(amount + 1, upper));
            f[0][0] = 0;

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= amount; ++c) {
                    if (c < coins[i]) {
                        f[i + 1][c] = f[i][c];
                    }
                    else {
                        f[i + 1][c] = min(f[i][c], f[i + 1][c - coins[i]] + 1);
                    }
                }
            }
            return f[n][amount] > amount ? -1 : f[n][amount];
        }
    };
}
namespace s322o4
{   // 一个数组空间优化（两个数组滚动的就不写了）
    class Solution {
    public:
        // f[i][c] = min(f[i - 1][c], f[i][c - coins[i]] + 1)
        int coinChange(vector<int>& coins, int amount) {
            vector<int> f(amount + 1, amount + 1);
            f[0] = 0;

            for (int x : coins) {
                // 要的就是覆盖当层的f[c - coins[i]]，所以不能逆序遍历
                /*
                f[i][c] = min(f[i - 1][c], f[i - 1][c - coins[i]] + 1); 0-1背包
                f[i][c] = min(f[i - 1][c], f[i][c - coins[i]] + 1); 完全背包
                对0-1背包：更新f[i][c]用到的是前一行f[i -1]的数据，但正序遍历时f[i][c - coins[i]]会在这一行先于f[i][c]被覆盖，
                所以等到计算f[i][c]时，用到的并不是f[i - 1][c - coins[i]]，而是相同行的f[i][c - coins[i]]，所以需要逆序遍历。
                对完全背包：因为元素可以重复选，恰好要求的就是当前行，也即f[i][c - coins[i]]，
                也就是说需要f[i][c - coins[i]]先于f[i][c]被更新（覆盖）。如果逆序遍历，反而会得到错误的答案，必须正序遍历。
                */
                for (int c = x; c <= amount; ++c) {
                    f[c] = min(f[c], f[c - x] + 1);
                }
            }
            return f[amount] > amount ? -1 : f[amount];
        }
    };
}

// 模板题3：本质上和s322是一道题，都是恰好装capacity，但是变成了求方案数，题目本身没什么，但重要是的下面：
// 记忆化搜索只会递归到能访问到的状态，而递推要把所有状态都算一遍，这其中就包含会溢出的状态
// 所以递推时中间数据可能会溢出，所以int -> unsigned int, unsigned int -> long long
// 在 C++ 中，有符号整数的溢出是未定义行为，而无符号整数的溢出是有定义的(比如UINT_MAX + 1 = 0，取模回绕)
namespace s518m1
{   // 记忆化搜索 + 递归
    class Solution {
    public:
        int change(int amount, vector<int>& coins) {
            int n = coins.size();
            vector<vector<int>> cache(n, vector<int>(amount + 1, -1));

            function<int(int, int)> dfs = [&](int i, int c) {
                if (i < 0) {
                    return int(c == 0);
                }
                if (cache[i][c] != -1) {
                    return cache[i][c];
                }
                if (c < coins[i]) {
                    cache[i][c] = dfs(i - 1, c);
                }
                else {
                    cache[i][c] = dfs(i - 1, c) + dfs(i, c - coins[i]);
                }
                return cache[i][c];
                };

            return dfs(n - 1, amount);
        }
    };
}
namespace s518m2
{   // 递推
    class Solution {
    public:
        int change(int amount, vector<int>& coins) {
            int n = coins.size();
            // 将数据类型改成了unsigned int
            vector<vector<unsigned int>> f(n + 1, vector<unsigned int>(amount + 1));
            f[0][0] = 1;// 边界

            for (int i = 0; i < n; ++i) {
                for (int c = 0; c <= amount; ++c) {
                    if (c < coins[i]) {
                        f[i + 1][c] = f[i][c];
                    }
                    else {
                        // 如果继续用int，这里可能会溢出
                        f[i + 1][c] = f[i][c] + f[i + 1][c - coins[i]];
                    }
                }
            }
            return f[n][amount];
        }
    };
}
namespace s518m3
{   // 空间优化
    /*
    如果硬币集合中不包含面额为 1 的硬币，那么组合数函数 f[c] 可能不是单调递增的。也就是说，
    对于某些 c < amount，f[c] 可能大于 f[amount]。这是因为 amount 可能难以用给定的硬币凑出
    （例如，如果硬币面额较大或没有公因数），而较小的金额 c 可能更容易凑出，因此组合数更多。
    题目只保证最终结果 f[amount] 在 int 范围内，但不保证中间值 f[c] 也在范围内。
    因此，在循环 f[c] += f[c - x] 时，如果 f[c - x] 很大，累加可能导致 f[c] 溢出。
    
    假设硬币面额为 [2, 3]，总金额 amount = 7：
    最终结果 f[7] = 1（组合方式：2+2+3），在 int 范围内。
    但中间值 f[6] = 2（组合方式：2+2+2 或 3+3），大于 f[7]。
    如果 amount 更大，比如 amount = 1000，但硬币仍为 [2, 3]，则 f[1000] 可能较小（约 167），
    但某些 f[c] 对于 c < 1000 可能更大（例如，f[998] 也可能约 167）。
    虽然这个例子中值不大，但如果硬币面额更小、更多样化，中间值可能非常大。

    再比如 amount 是奇数但 coins[i] 都是偶数，这种情况下答案是 0，但是我们计算那些 dp[偶数] 的状态时，会算出溢出。
    */

    /*
    为什么用unsigned int就可以防止溢出？
    因为首先unsigned int的范围是[0, 4.1 x 10^9]，本身范围就比int大，更不容易溢出
    其次当f[c] = UINT_MAX时，如果继续累加，也会取模回绕，比如UINT_MAX + 1 = 0
    虽然这样取模后f数组里装的元素就不严格符合f的定义了，但这样并不会影响答案。
    比如一个数已经超过本来就已经要超过UINT_MAX了，那只要无论是这个值再累加，或是其他元素要用到这个值，
    都说明其并非是答案。又因为题干中说了答案一定不超过UINT_MAX，所以这个值无论变成什么样都不影响答案的递推过程
    */
    class Solution {
    public:
        int change(int amount, vector<int>& coins) {
            int n = coins.size();
            vector<unsigned int> f(amount + 1);
            f[0] = 1;

            for (int x : coins) {
                for (int c = x; c <= amount; ++c) {
                    f[c] += f[c - x];
                }
            }
            return f[amount];
        }
    };
}

// 相当于s322零钱兑换，但是要自己生成零钱coins数组
namespace s279m1
{
    class Solution {
    public:
        int numSquares(int n) {
            // 1, 4, 9, 16等完全平方数是物品，可以无限次使用
            // 恰好满足capacity为n的最少价值和
            // 物品肯定重量不能等于0，相当于零钱兑换

            vector<int> nums;
            for (int i = 1; i * i <= n; ++i) {
                nums.push_back(i * i);
            }

            // f[i][c] = min(f[i - 1][c], f[i][c - nums[i]] + 1)
            // f[c] = min(f[c], f[c - nums[i]] + 1)

            // f数组全部初始化为n + 1，为边界值
            vector<int> f(n + 1, n + 1);
            f[0] = 0;

            for (int x : nums) {
                for (int c = x; c <= n; ++c) {
                    f[c] = min(f[c], f[c - x] + 1);
                }
            }

            // 一定有答案
            return f[n];
        }
    };
}
// ---------------------
// 【3.3】多重背包(选做)()
/*

*/
// ---------------------
// 暂时不做
// ---------------------
// 【3.4】分组背包()
/*

*/
// ---------------------
// 暂时不做
// ---------------------
// 【3.5】树形背包(选做)
/*

*/
// ---------------------
// 暂时不做
// ------------------------------------------------------------------------------------
// 四、经典线性 DP
// 【4.1】最长公共子序列（LCS）(3)
// 子序列本质是选或不选（删或不删），一般定义 f[i][j] 表示对 (s[:i],t[:j]) 的求解结果
/*
1143.最长公共子序列：给定两个字符串 text1 和 text2，返回这两个字符串的最长 公共子序列 的长度。
如果不存在 公共子序列 ，返回 0 。一个字符串的 子序列 是指这样一个新的字符串：
它是由原字符串在不改变字符的相对顺序的情况下删除某些字符（也可以不删除任何字符）后组成的新字符串。
例如，"ace" 是 "abcde" 的子序列，但 "aec" 不是 "abcde" 的子序列。
两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列。

72.编辑距离：给你两个单词 word1 和 word2， 请返回将 word1 转换成 word2 所使用的最少操作数  。
你可以对一个单词进行如下三种操作：插入一个字符/删除一个字符/替换一个字符

115.不同的子序列：给你两个字符串 s 和 t ，统计并返回在 s 的 子序列 中 t 出现的个数。
测试用例保证结果在 32 位有符号整数范围内。
*/
// ---------------------
// 模板题4：LCS问题，当前问题可以变成三个子问题，根据s[i]与s[j]进一步分类讨论，比背包复杂一点的DP问题
namespace s1143o1
{
    // dfs(i, j) = max({dfs(i - 1, j), dfs(i, j - 1), dfs(i - 1, j - 1) + (s[i] == s[j])})
    // 问题1：在s[i] = t[j]时，除了dfs(i - 1, j - 1)外，还需要进行dfs(i - 1, j)或是dfs(i, j - 1)吗？
    // 答：不需要，因为只有dfs(i - 1, j)比dfs(i - 1, j - 1)还大才有递归的必要，但此时dfs(i - 1, j)是<=dfs(i - 1, j - 1)的
    // 问题2：在s[i] != t[j]时，还需要计算dfs(i - 1, j - 1)吗？
    // 答：不需要，因为dfs(i - 1, j - 1)的结果已经在dfs(i - 1, j)里包含了，dfs(i, j)的值是一直递减的，毕竟一直都是求max
    // 所以式子简化为：dfs(i, j) = dfs(i - 1, j - 1) + 1                    (s[i] == t[j])
    //                           = max(dfs(i - 1, j), dfs(i, j - 1))        (s[i] != t[j])

    // 下面是最简单的代码框架，没有记忆化搜索
    class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            int n = text1.size(), m = text2.size();

            function<int(int, int)> dfs = [&](int i, int j) {
                if (i < 0 || j < 0) {
                    return 0; // 其中一个字符串为空
                }
                if (text1[i] == text2[j]) {
                    return dfs(i - 1, j - 1) + 1;
                }
                return max(dfs(i - 1, j), dfs(i, j - 1));
                };

            return dfs(n - 1, m - 1);
        }
    };
}
namespace s1143o2
{   // 记忆化搜索
    class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            int n = text1.size(), m = text2.size();
            vector<vector<int>> cache(n, vector<int>(m, -1));

            function<int(int, int)> dfs = [&](int i, int j) {
                if (i < 0 || j < 0) {
                    return 0;
                }
                if (cache[i][j] != -1) {
                    return cache[i][j];
                }
                if (text1[i] == text2[j]) {
                    cache[i][j] = dfs(i - 1, j - 1) + 1;
                }
                else {
                    cache[i][j] = max(dfs(i - 1, j), dfs(i, j - 1));
                }
                return cache[i][j];
                };

            return dfs(n - 1, m - 1);
        }
    };
}
namespace s1143o3
{   // 递推
    class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            int n = text1.size(), m = text2.size();
            vector<vector<int>> f(n + 1, vector<int>(m + 1));

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (text1[i] == text2[j]) {
                        f[i + 1][j + 1] = f[i][j] + 1;
                    }
                    else {
                        f[i + 1][j + 1] = max(f[i + 1][j], f[i][j + 1]);
                    }
                }
            }

            return f[n][m];
        }
    };
}
namespace s1143o4
{   // 空间优化：两个滚动数组，LCS问题需要用左边，上边，左上三个部分的原始数据更新
    // 所以用一个数组表示还需要一点改变技法，不如用滚动数组无脑点
    class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            int n = text1.size(), m = text2.size();
            vector<vector<int>> f(2, vector<int>(m + 1));

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (text1[i] == text2[j]) {
                        f[(i + 1) % 2][j + 1] = f[i % 2][j] + 1;
                    }
                    else {
                        f[(i + 1) % 2][j + 1] = max(f[(i + 1) % 2][j], f[i % 2][j + 1]);
                    }
                }
            }

            return f[n % 2][m];
        }
    };
}
namespace s1143o5
{   // 不能像之前背包和入门DP那样直接写，要用到左上，左边，上边的数据，为保障左边的数据更新正确，内层循环要用正序遍历
    // 但左上的数据又会被提早覆盖，所以需要每次遍历时用临时变量pre先保存一下左上的数据f[i][j]
    // 综合来说，还是滚动数组无脑点
    class Solution {
    public:
        int longestCommonSubsequence(string text1, string text2) {
            int n = text1.size(), m = text2.size();
            vector<int> f(m + 1);

            for (int i = 0; i < n; ++i) {
                int pre = f[0];// 左上值的初始值
                for (int j = 0; j < m; ++j) {
                    int temp = f[j + 1];// 保存左上值
                    if (text1[i] == text2[j]) {
                        // f[i + 1][j + 1] = f[i][j] + 1;
                        f[j + 1] = pre + 1;
                    }
                    else {
                        f[j + 1] = max(f[j], f[j + 1]);
                    }
                    pre = temp; // 更新左上值临时变量
                }
            }

            return f[m];
        }
    };
}

// 跟s1143类似，关键还是在于子问题的转换与边界条件的确定，最难的点是一个数组的空间优化，需要想s1143一样提前保存左上角值，且更复杂
namespace s72o1
{
    // 所以式子简化为：dfs(i, j) = dfs(i - 1, j - 1)                                                 (s[i] == t[j])
    //                           = min({dfs(i - 1, j), dfs(i, j - 1), dfs(i - 1, j - 1)}) + 1        (s[i] != t[j])
    // 其中s[i] != t[j]时：dfs(i - 1, j)为删除，dfs(i, j - 1)为插入，dfs(i - 1, j - 1)为替换
    // 边界条件是当i或j小于0时，就将另一个字符串全部插入/删除，操作数为另一个字符串的长度

    // 带记忆化搜索的递归版本
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            int n = word1.size(), m = word2.size();
            vector<vector<int>> memo(n, vector<int>(m, -1));

            function<int(int, int)> dfs = [&](int i, int j) {
                if (i < 0) {
                    return j + 1;
                }
                if (j < 0) {
                    return i + 1;
                }
                if (memo[i][j] != -1) {
                    return memo[i][j];
                }
                if (word1[i] == word2[j]) {
                    memo[i][j] = dfs(i - 1, j - 1);
                }
                else {
                    memo[i][j] = min({ dfs(i - 1, j - 1), dfs(i - 1, j), dfs(i, j - 1) }) + 1;
                }
                return memo[i][j];
                };
            return dfs(n - 1, m - 1);
        }
    };

}
namespace s72o2
{   // 递推
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            int n = word1.size(), m = word2.size();
            vector<vector<int>> f(n + 1, vector<int>(m + 1));
            // 1:1翻译：需要将第0行与第0列全部初始化
            for (int j = 0; j < m; ++j) {
                f[0][j + 1] = j + 1;// 先初始化第0行
            }
            /*  与下面初始化方式等价
            for (int j = 0; j <= m; ++j) {
                f[0][j] = j;
            }
            */
            
            for (int i = 0; i < n; ++i) {
                f[i + 1][0] = i + 1;// 初始化第0列，把这个过程单列出来也对，但整合进递推过程更好
                for (int j = 0; j < m; ++j) {
                    if (word1[i] == word2[j]) {
                        f[i + 1][j + 1] = f[i][j];
                    }
                    else {
                        f[i + 1][j + 1] = min({ f[i][j], f[i][j + 1], f[i + 1][j] }) + 1;
                    }
                }
            }

            return f[n][m];
        }
    };
}
namespace s72o3
{   // 无脑滚动数组空间优化
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            int n = word1.size(), m = word2.size();
            vector<vector<int>> f(2, vector<int>(m + 1));
            for (int j = 0; j < m; ++j) {
                f[0][j + 1] = j + 1;// 先初始化第0行
            }

            for (int i = 0; i < n; ++i) {
                f[(i + 1) % 2][0] = i + 1;// 初始化第0列
                for (int j = 0; j < m; ++j) {
                    if (word1[i] == word2[j]) {
                        f[(i + 1) % 2][j + 1] = f[i % 2][j];
                    }
                    else {
                        f[(i + 1) % 2][j + 1] = min({ f[i % 2][j], f[i % 2][j + 1], f[(i + 1) % 2][j] }) + 1;
                    }
                }
            }

            return f[n % 2][m];
        }
    };
}
namespace s72o4
{   // 一个数组的空间优化，和s1143类似，需提前保存左上的数据
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            int n = word1.size(), m = word2.size();
            vector<int> f(m + 1);
            for (int j = 0; j < m; ++j) {
                f[j + 1] = j + 1;// 先初始化第0行
            }

            for (int i = 0; i < n; ++i) {
                int pre = f[0];// 左上角的初始值，要在下行代码之前（保存的是f[i][0]），保存旧的f[0]
                f[0] = i + 1;// 初始化第0列（这个原本是f[i + 1][0]）,初始化新的f[0]

                for (int j = 0; j < m; ++j) {
                    int temp = f[j + 1];    
                    // 保存的是f[i][j + 1]，因为在j自增后，这里的j + 1就相当于j
                    // 也即temp为pre即将要变成的值，要提前保存

                    if (word1[i] == word2[j]) {
                        f[j + 1] = pre;
                    }
                    else {
                        f[j + 1] = min({ f[j], f[j + 1], pre }) + 1;
                    }

                    pre = temp;
                }
            }

            return f[m];
        }
    };
}

// 模板题5：难点在于子问题的分析，递归和记忆化搜索努力点还是能写出来，但递推和空间优化开始又变得抽象
namespace s115o1
{   /*
    以统计 s = babgbag 的所有子序列中，t = bag 出现的个数为例，考虑怎么通过删除字母得到子序列：
    1.如果删除 s 最右边的字母 g，那么需要解决的子问题为：统计 babgba 的所有子序列中，bag 出现的个数。
    2.如果不删除 s 最右边的字母 g，那么需要解决的子问题为：统计 babgba 的所有子序列中，ba 出现的个数。
      这些等于 ba 的子序列，在末尾加上（没有删除的）字母 g，就是子序列 t = bag 了
    */
    // 选或不选，删或不删
    // 定义状态为 dfs(i,j)，表示 s[..i] 的所有子序列中，t[..j] 出现的个数。这里记号 s[..i] 表示 s[0] 到 s[i]
    // 1.删除 s[i]，那么接下来要解决的问题是：统计 s[..i - 1] 的所有子序列中，t[..j] 出现的个数，即 dfs(i - 1, j)。
    // 2.不删除 s[i]（前提是 s[i] = t[j]），那么接下来要解决的问题是：
    //   统计 s[..i - 1] 的所有子序列中，t[..j - 1] 出现的个数，即 dfs(i - 1, j - 1)。
    // 递归边界：
    // 1.dfs(i, -1) = 1，此时t是空串，根据定义，只有一种方法可以从s中得到空串，也即删除s中全部字母
    // 2.如果i < j，那么dfs(i, j) = 0，因为无法从得到一个比s[...i]还长的子序列
    // 递归入口：dfs(n - 1, m - 1), 其中n为s长度，m为t长度

    // 递归 + 记忆化搜索
    class Solution {
    public:
        int numDistinct(string s, string t) {
            int n = s.size(), m = t.size();
            if (n < m) return 0;// 特判，提前返回，可能性剪枝

            vector<vector<int>> memo(n, vector<int>(m, -1));

            function<int(int, int)> dfs = [&](int i, int j) {
                if (j < 0) {
                    return 1;
                }
                if (i < j) {
                    return 0;
                }
                if (memo[i][j] != -1) {
                    return memo[i][j];
                }
                if (s[i] == t[j]) {
                    memo[i][j] = dfs(i - 1, j) + dfs(i - 1, j - 1);
                }
                else {
                    memo[i][j] = dfs(i - 1, j);
                }
                return memo[i][j];
                };

            return dfs(n - 1, m - 1);
        }
    };
    
}
namespace s115o2
{   // 递推会计算很多不必要的中间态，所以对于答案值可能较大的题目，写递推时要时刻考虑是否会越界，用unsigned int解决
    class Solution {
    public:
        int numDistinct(string s, string t) {
            int n = s.size(), m = t.size();
            if (n < m) return 0;// 特判，提前返回

            vector<vector<unsigned>> f(n + 1, vector<unsigned>(m + 1));
            for (int i = 0; i < n + 1; ++i) {
                f[i][0] = 1;// 初始化边界条件，所有j = 0的情况都赋值1
                // 包括f[0][0]也要赋值，这个初始化过程可以拆分为：
                // 单独的f[0][0] = 1 + 下面内层循环前的f[i + 1][0] = 1
            }

            for (int i = 0; i < n; ++i) {
                // j <= i才需要计算 -> j < i + 1，跟m一起取最小值，缩小遍历范围
                // j的遍历范围还可以进一步缩小：
                // 注意o1的递归中，i的减小速度是一定大于等于j的减小速度的
                // 那么有j的减小量：m - j
                //       i的减小量：n - i
                // m - j <= n - i       ->      j >= m - n + i
                // 所以内层循环j的起始点为m - n + i，还要与0取一个max，因为可能为负
                // 但这个优化我觉得超出了1:1翻译递推的范畴，所以就没有直接写出来，面试的时候不加也没关系
                for (int j = 0; j < min(i + 1, m); ++j) {
                    if (s[i] == t[j]) {
                        f[i + 1][j + 1] = f[i][j + 1] + f[i][j];
                    }
                    else {
                        f[i + 1][j + 1] = f[i][j + 1];
                    }
                }
            }
            return f[n][m];
        }
    };
}
namespace s115o3
{   // 滚动数组空间优化
    class Solution {
    public:
        int numDistinct(string s, string t) {
            int n = s.size(), m = t.size();
            if (n < m) return 0;

            vector<vector<unsigned>> f(2, vector<unsigned>(m + 1));
            f[0][0] = f[1][0] = 1; // 边界条件初始化

            for (int i = 0; i < n; ++i) {
                for (int j = max(m - n + i, 0); j < min(i + 1, m); ++j) {
                    if (s[i] == t[j]) {
                        f[(i + 1) % 2][j + 1] = f[i % 2][j + 1] + f[i % 2][j];
                    }
                    else {
                        f[(i + 1) % 2][j + 1] = f[i % 2][j + 1];
                    }
                }
            }

            return f[n % 2][m];
        }
    };
}
namespace s115o4
{
    class Solution {
    public:
        int numDistinct(string s, string t) {
            int n = s.size(), m = t.size();
            if (n < m) return 0;
                
            // 记住依旧用unsigned，空间优化并不能解决中间态数据溢出的问题
            vector<unsigned int> f(m + 1);
            f[0] = 1; // 边界条件初始化

            for (int i = 0; i < n; ++i) {
                for (int j = min(i, m - 1); j >= max(m - n + i, 0); --j) {
                    if (s[i] == t[j]) {
                        f[j + 1] += f[j];
                    }
                }
            }
            return f[m];
        }
    };
}
// ---------------------
// 【4.2】最长递增子序列（LIS）(1)
// 做法比较多，采用枚举选那个思路（子序列要求相邻相关，所以不能用选或不选思路，强行用会比较麻烦）
// 回溯/递推的DP做法的时间复杂度为O(n^2)，而二分搜索+贪心(基于DP优化，同样也要用到DP)时间复杂度为O(nlogn)
/*
300.最长递增子序列：给你一个整数数组 nums ，找到其中最长严格递增子序列的长度。
子序列 是由数组派生而来的序列，删除（或不删除）数组中的元素而不改变其余元素的顺序。
例如，[3,6,2,7] 是数组 [0,3,1,6,2,2,7] 的子序列。
*/
// ---------------------
// 模板题6：可以用子集型回溯思路先去思考（子序列是原数组的一个子集），dfs(i)并不直接返回答案，而是参与最后的答案更新
// 有记忆化搜索/递推，转换成LCS问题，二分+贪心三种思路（这个较难，暂时只能记忆，做不到举一反三与推广应用）
namespace s300o1
{   // 记忆化搜索 + 递归（枚举选哪个）
    // dfs(i)代表以nums[i]结尾的子序列的最大长度
    // 时间复杂度O(n^2)
    class Solution {
    public:
        int lengthOfLIS(vector<int>& nums) {
            int n = nums.size();
            vector<int> memo(n);

            auto dfs = [&](auto&& dfs, int i)->int {
                int& res = memo[i];
                if (res > 0) {
                    return res;
                }

                for (int j = 0; j < i; ++j) {
                    // 枚举所有比当前元素更小的元素的dfs(j)
                    if (nums[j] < nums[i]) {
                        res = max(res, dfs(dfs, j));
                    }
                }
                ++res;
                return res;
                };

            // 逐个遍历所有nums[i]中元素
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                ans = max(ans, dfs(dfs, i));
            }
            return ans;
        }
    };
}
namespace s300o2
{   // 递推，虽然时间复杂度与o1相同，也是O(n * n)，但因为缓存命中率高，效率还是高了不少
    // 这里的递推写法和之前的都有所不同，需要细细体会
    // 这道题的递推写法是可以直接按照遍历过程理解的，推导思维和之前逆序 + 翻译的模式不同

    class Solution {
    public:
        int lengthOfLIS(vector<int>& nums) {
            int n = nums.size();
            vector<int> f(n);
               
            // 因为是一维的DP，只有一行数据，不断更新即可
            // 而且外层i是逐渐增大的，不会出现重复++f[i]的情况
            for (int i = 0; i < n; ++i) {
                // 其实就是把o1中的dfs过程换个地方
                f[i] = 0;// 相当于之前的mx
                for (int j = 0; j < i; ++j) {
                    if (nums[j] < nums[i]) {
                        f[i] = max(f[i], f[j]);// 逐层覆盖
                    }
                }
                ++f[i];// 把当前元素的长度1加上，相当于之前的mx + 1
            }
            /* 也可以按照下面这样写，更好
            vector<int> f(n, 1);
            int ans = 0;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < i; ++j) {
                    if (nums[j] < nums[i]) {
                        f[i] = max(f[i], f[j]);// 逐层覆盖
                    }
                }
                ans = max(ans, f[i]);
            }
            return ans;
            */

            return *max_element(f.begin(), f.end());
        }
    };
}
namespace s300o3
{   // 如果把nums排序并去重后得到nums2，那么nums和nums2的最长公共子序列的长度就是这一题的答案，实现代码复用
    class Solution {
    private:
        // s1143代码复用
        int LCS(const vector<int>& v1, const vector<int>& v2) {
            int n = v1.size(), m = v2.size();
            vector<int> f(m + 1);

            for (int i = 0; i < n; ++i) {
                int pre = f[0]; // 左上值的初始值
                for (int j = 0; j < m; ++j) {
                    int temp = f[j + 1]; // 保存左上值
                    if (v1[i] == v2[j]) {
                        // f[i + 1][j + 1] = f[i][j] + 1;
                        f[j + 1] = pre + 1;
                    }
                    else {
                        f[j + 1] = max(f[j], f[j + 1]);
                    }
                    pre = temp; // 更新左上值临时变量
                }
            }

            return f[m];
        }

    public:
        int lengthOfLIS(vector<int>& nums) {
            vector<int> nums2 = nums;// 拷贝
            sort(nums2.begin(), nums2.end());// 排序
            nums2.erase(unique(nums2.begin(), nums2.end()), nums2.end());// 去重

            return LCS(nums, nums2);
        }
    };
}
namespace s300o4
{   // 基于DP思路转化的的二分 + 贪心优化
    // 对DP时间复杂度的进阶技巧：交换状态与状态值
    // 二分 + 贪心做法：由于没有状态的重叠和子问题，所以这种做法不能算成是DP
    /* 分析思路：
    
    假设 f[3] 和 f[4] 的值都等于 2，且 nums[3]=99999，nums[4]=9。
    我们现在要计算 f[5]。想一想，谁更容易满足上面代码中的 if (nums[j] < nums[i])？这里 i=5，j = 3 或者 4。
    nums[4] 更小，更容易满足 if (nums[j] < nums[i])。更大的 nums[3] 虽然也可能满足条件，
    但由于 f[3]=f[4]，在更新 f[5] 时，二者的效果是一样的，所以 nums[3] 是个无用数据。
    换句话说，对于相同的 f 值，我们只需保留更小的 nums[i]。
    
    因此，定义 g[i] 表示 f[j] = i + 1 时，nums[j] 的最小值。也就是长为 i+1 的上升子序列的末尾元素的最小值。
    这里 +1 是因为 f[j] 至少是 1，g[0] 对应的不是 f[j]=0，而是 f[j]=1。

    把最小的数贪心的放在g的左边，逐个放入，单点更新
    同时由于g定义下g一定是一个单调递增的数组，所以可以使用二分搜索加速更新的过程

    如果上面这段文字还是看不懂，可以去看视频精讲20
    */

    // 如果子序列允许重复，那么方法中的二分就改成upper_bound

    // 这种做法比较超模了，暂时只能记忆，做不到举一反三与推广应用，时间复杂度O(nlogn)
    class Solution {
    public:
        int lengthOfLIS(vector<int>& nums) {
            vector<int> g;
            for (int x : nums) {
                // 从左往右遍历，g中元素只要能放进去，就一定是原数组的一个子序列
                auto it = lower_bound(g.begin(), g.end(), x);
                if (it == g.end()) {
                    g.push_back(x); // g中不存在 >= x的元素
                }
                else {
                    *it = x;
                }
            }
            return g.size();
        }
    };
}
namespace s300o4extension {
    // 在o4的基础上，返回具体的lis序列的做法
    // 序列是指保持在原本数组中的相对顺序的序列
    class Solution {
    public:
        vector<int> lengthOfLIS(vector<int>& nums) {
            int n = nums.size();
            // 不仅记录值，还记录值对应的下标
            vector<int> g;
            vector<int> indexs;
            vector<int> last(n, -1);

            for (int i = 0; i < n; ++i) {
                int x = nums[i];
                auto it = lower_bound(g.begin(), g.end(), x);
                int idx = it - g.begin();
                if (idx > 0) {
                    // 记录 nums[i] 添加到了哪个数的末尾
                    last[i] = indexs[idx - 1];
                }
                if (it == g.end()) {
                    g.push_back(x);
                    indexs.push_back(i);
                }
                else {
                    g[idx] = x;
                    indexs[idx] = i;
                }
            }
              
            vector<int> lis;
            // LIS 的最后一个数是 nums[g.back().second]，顺着 last 倒着找上一个数
            // 注意：for循环的更新条件是i = last[i]，判断条件是i >= 0，不怎么常规
            for (int i = indexs.back(); i >= 0; i = last[i]) {
                lis.push_back(nums[i]);
            }
            return lis;
        }
    };
}