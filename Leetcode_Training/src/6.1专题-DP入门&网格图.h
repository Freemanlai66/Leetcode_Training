#pragma once
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

// 问题待定：
/*
1.网格图DP进阶题s329暂时还没做，面试频率很高
*/

/*
模板题：
1.打家劫舍，DP入门 + 记忆化搜索模板题，与回溯中的选与不选很类似：198
2.环形打家劫舍DP，但不用考虑太多，先选头或尾部的一个元素试着分析下，是否能缩小问题规模或直接改变问题的性质：213
3.DP入门题，爬楼梯，可以启发递归边界的思考：70
4.组合总和类型题目都可以转化为爬楼梯，模板题1，2的强化复杂化题，值得细看：377
5.“值域”上的打家劫舍：740
6.最大子数组和模板，o1为前缀和（更简单），DP做法则是求max(f)，无需记忆化搜索，直接写递推式即可（Kadane 算法）：53
7.网格图DP模板，o2开始是DP入门三部曲（自底向上）：64
*/

// 动态规划：入门DP + 网格图DP

/* 入门DP三步走：
1.用回溯思路思考问题 2.改成记忆化搜索 3.将递归翻译成递推
但记忆化搜索并不是万能的，某些题目只有写成递推，才能结合数据结构等来优化时间复杂度，
多数题目还可以优化空间复杂度。所以尽量在写完记忆化搜索后，把递推的代码也写一下。熟练之后直接写递推也可以。

动态规划有「选或不选」和「枚举选哪个」两种基本思考方式。
子序列相邻无关一般是「选或不选」（比如背包问题），
子序列相邻相关（例如 LIS 问题）一般是「枚举选哪个」。
*/

// 一、入门DP()
// 【1.1】爬楼梯(4)
// 从边界点考虑如何将问题规模缩小进行递归搜索，再加上cache保存计算结果 = 记忆化搜索（防止超时，不记忆则是指数化时间复杂度）
/*
70.爬楼梯：假设你正在爬楼梯。需要 n 阶你才能到达楼顶。
每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶呢？

509.斐波那契数：斐波那契数 （通常用 F(n) 表示）形成的序列称为 斐波那契数列 。
该数列由 0 和 1 开始，后面的每一项数字都是前面两项数字的和。也就是：
F(0) = 0，F(1) = 1
F(n) = F(n - 1) + F(n - 2)，其中 n > 1
给定 n ，请计算 F(n) 。

746.使用最小花费爬楼梯：给你一个整数数组 cost ，其中 cost[i] 是从楼梯第 i 个台阶向上爬需要支付的费用。
一旦你支付此费用，即可选择向上爬一个或者两个台阶。你可以选择从下标为 0 或下标为 1 的台阶开始爬楼梯。
请你计算并返回达到楼梯顶部的最低花费。

377.给你一个由 不同 整数组成的数组 nums ，和一个目标整数 target 。请你从 nums 中找出并返回总和为 target 的元素组合的个数。
题目数据保证答案符合 32 位整数范围。
*/
// ---------------------
// 模板题3：DP入门题，可以启发递归边界的思考
namespace s70m1
{   // 只使用记忆化搜索时最容易想到的写法
    // 这样计算需要从未知的叶子节点一层层向上返回答案
    // 而动态规划需要从已知的base case开始向上推导，具体的思路区别可以看o1
    class Solution {
    public:
        int climbStairs(int n) {
            vector<int> memo(n, -1);

            auto dfs = [&](auto&& dfs, int i)->int {
                if (i == n) return 1;      // 叶子节点：到达终点
                if (i > n) return 0;       // 无效分支：超过终点
                if (memo[i] != -1) {       // 已计算过，直接返回
                    return memo[i];
                }
                memo[i] = dfs(dfs, i + 1) + dfs(dfs, i + 2);
                return memo[i];
                };

            return dfs(dfs, 0);
        }
    };
}
namespace s70o1
{	// 已经确定的base case是当n = 0和n = 1的情况（或者说n = 1和n = 2的情况）
    // 倒着思考，如果最后一步走了1级台阶，那么需要先爬到i - 1的位置
    // 如果最后一步走了2级台阶，那么需要先爬到i - 2的位置
    // 两种情况是互相独立的，也即dfs(i) = dfs(i - 1) + dfs(i - 2)
    // 因此dfs(2) = dfs(0) + dfs(1) = 2，这里dfs(0)没有实际意义，但是为了符合状态方程，需要将其初始化为1
    class Solution {
    public:
        int climbStairs(int n) {
            vector<int> cache(n + 1, -1);
            // 这里注意cache数组的大小要改成n + 1，因为有n + 1个状态

            auto dfs = [&](auto&& self, int i)->int {
                // 进一步启发递归边界的思考
                // dfs(i)代表到i为止的方案数
                if (i <= 1) {
                    // dfs(0), dfs(1)都为1，因为dfs(2) = 2
                    return 1;
                }
                if (cache[i] != -1) {
                    return cache[i];
                } 
                return cache[i] = self(self, i - 1) + self(self, i - 2);
                };
            return dfs(dfs, n);
        }
    };
}
namespace s70o2
{   // 1：1翻译成递推
    class Solution {
    public:
        int climbStairs(int n) {
            vector<int> f(n + 1, 0);
            f[0] = f[1] = 1;

            for (int i = 2; i <= n; ++i) {
                f[i] = f[i - 1] + f[i - 2];
            }
            return f[n];
        }
    };
}
namespace s70o3
{   // 空间优化，o4还能继续精进
    class Solution {
    public:
        int climbStairs(int n) {
            int f0 = 1, f1 = 1;
            for (int i = 2; i <= n; ++i) {
                int fnew = f0 + f1;
                f0 = f1;
                f1 = fnew;
            }
            return f1;
        }
    };
}
namespace s70o4
{   // 当进一步熟悉DP后，o3可以改为以下形式，只要能满足递推过程就能得到正确答案
    class Solution {
    public:
        int climbStairs(int n) {
            int f0 = 0, f1 = 1;

            for (int i = 0; i < n; ++i) {
                int fnew = f0 + f1;
                f0 = f1;
                f1 = fnew;
            }

            return f1;
        }
    };
}

namespace s509m1
{   // 后期才做的这道题，这道题可以直接写空间优化的版本，很简单易懂
    class Solution {
    public:
        int fib(int n) {
            if (n == 0) return 0;
            if (n == 1) return 1;// 这行也可以不写
            int f0 = 0, f1 = 1;
            for (int i = 2; i <= n; ++i) {
                int fnew = f1 + f0;
                f0 = f1;
                f1 = fnew;
            }
            return f1;
        }
    };
}

// 与s70类似的递归边界
namespace s746m1
{   // 记忆化搜索 递归
    class Solution {
    public:
        int minCostClimbingStairs(vector<int>& cost) {
            // 可以从下标为0或为1开始爬
            // 楼梯顶部是i == n的位置
            int n = cost.size();
            vector<int> cache(n + 1, -1);

            // dfs(i)代表爬到i这个位置的所有花费，因此不包含当前i的费用
            auto dfs = [&](auto&& self, int i)->int {
                if (i < 2) {
                    // 因为可以从i = 0或i = 1开始爬，因为是起点，所以爬到这个位置的花费是0
                    return 0;
                }
                if (cache[i] != -1) {
                    return cache[i];
                }

                int p1 = cost[i - 1] + self(self, i - 1);
                int p2 = cost[i - 2] + self(self, i - 2);
                cache[i] = min(p1, p2);
                return cache[i];
                };
            return dfs(dfs, n);
        }
    };
}
namespace s746m2
{   // 递归
    class Solution {
    public:
        int minCostClimbingStairs(vector<int>& cost) {
            int n = cost.size();
            // 正好全部可以全部初始化为0，f[0]和f[1]就不用特地初始化为0了
            vector<int> f(n + 1, 0);

            for (int i = 2; i <= n; ++i) {
                f[i] = min(f[i - 1] + cost[i - 1], f[i - 2] + cost[i - 2]);
            }

            return f[n];
        }
    };
}
namespace s746m3
{   // 空间优化
    class Solution {
    public:
        int minCostClimbingStairs(vector<int>& cost) {
            int n = cost.size();
            int f1 = 0, f0 = 0;

            for (int i = 2; i <= n; ++i) {
                int fnew = min(f1 + cost[i - 1], f0 + cost[i - 2]);
                f0 = f1;
                f1 = fnew;
            }

            return f1;
        }
    };
}

// 模板题4：组合总和类型题目都可以转化为爬楼梯，模板题1，2的强化复杂化题，值得细看
namespace s377m1
{   // 先尝试用常规的回溯思路去写
    // 因为可以重复使用，且不同顺序视为不同组合，所以最好用枚举选哪个模板
    // 但是这样做时间复杂度是O(n^n),指数增长，超时了
    class Solution {
    public:
        int combinationSum4(vector<int>& nums, int target) {
            // nums中元素各不相同，为正整数
            // 单个元素可以重复多次使用
            // 顺序不同的序列视作不同的组合，比如1, 3和3, 1
            int ans = 0;
            int n = nums.size();

            // 连序号都不用带了
            auto dfs = [&](auto&& self, int sum) -> void {
                if (sum == target) {
                    ++ans;
                    return;
                }

                for (int i = 0; i < n; ++i) {
                    if (sum + nums[i] > target) { // 提前终止
                        break;
                    }
                    self(self, sum + nums[i]); // 允许重复选
                }
                };
            dfs(dfs, 0);
            return ans;
        }
    };
}
namespace s377o1
{   // 这道题本质是爬楼梯，s70相当于target = n，nums = [1, 2]
    // dfs(i)依旧代表到i处的方案数，楼梯数则为target
    // dfs(i) = dfs(i - nums[0]) + dfs(i - nums[1] + ... + dfs(i - nums[n - 1])
    // 当i - nums[n - 1] < 0 时，则跳过累加
    // 剩下的就是继续配合记忆化搜索 + 递推 + 空间优化三部曲了

    // 1.记忆化搜索
    // 注意这里递归边界和s70不一样了，只有dfs(0) = 1，因为其他边界都被if (x <= i) {...}排除掉了
    class Solution {
    public:
        int combinationSum4(vector<int>& nums, int target) {
            vector<int> cache(target + 1, -1);

            auto dfs = [&](auto&& self, int i) -> int {
                if (i == 0) {
                    return 1;
                }

                if (cache[i] != -1) {
                    return cache[i];
                }

                cache[i] = 0;
                for (int x : nums) {
                    if (x <= i) {
                        cache[i] += self(self, i - x);
                    }
                }
                return cache[i];
                };
            return dfs(dfs, target);
        }
    };
}
namespace s377o2
{   // dfs(i) = dfs(i - nums[0]) + dfs(i - nums[1] + ... + dfs(i - nums[n - 1])
    // f[i] = f[i - nums[0]] + f[i - nums[1]] + ... + f[i - nums[n - 1]]
    // 递归边界dfs(0) = 1变成f[0] = 1
    // 但做不到O(1)空间优化，因为需要同时用到n个f[]，只是可以省去递归函数的栈空间
    class Solution {
    public:
        int combinationSum4(vector<int>& nums, int target) {
            vector<unsigned> f(target + 1);
            f[0] = 1;
            // 注意i从1开始，同时f用unsigned进一步节省内存
            for (int i = 1; i <= target; ++i) {
                for (int x : nums) {
                    if (x <= i) {
                        f[i] += f[i - x];
                    }
                }
            }
            return f[target];
        }
    };
}
// ---------------------
// 【1.2】打家劫舍(3)
/*
198.打家劫舍：你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，
影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。
给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。

213.打家劫舍 II：你是一个专业的小偷，计划偷窃沿街的房屋，每间房内都藏有一定的现金。这个地方所有的房屋都 围成一圈 ，
这意味着第一个房屋和最后一个房屋是紧挨着的。同时，相邻的房屋装有相互连通的防盗系统，
如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警 。
给定一个代表每个房屋存放金额的非负整数数组，计算你 在不触动警报装置的情况下 ，今晚能够偷窃到的最高金额。

740.删除并获得点数：给你一个整数数组 nums ，你可以对它进行一些操作。
每次操作中，选择任意一个 nums[i] ，删除它并获得 nums[i] 的点数。之后，
你必须删除 所有 等于 nums[i] - 1 和 nums[i] + 1 的元素。
开始你拥有 0 个点数。返回你能通过这些操作获得的最大点数。
*/
// ---------------------
// 模板题1：DP入门 + 记忆化搜索模板题，与回溯中的选与不选很类似
namespace s198o1
{   // 通过储存搜索过的节点，可以让搜索树节点树保持在只有n个节点，时间复杂度降为O(n)
    // 记忆化搜索搜：cache数组占用O(n)空间，这部分可以考虑进行优化，变成O(1)，见o2写法
    class Solution {
    public:
        int rob(vector<int>& nums) {
            int n = nums.size();
            vector<int> cache(n, -1);

            auto dfs = [&](auto&& self, int index)->int {
                if (index < 0) {
                    return 0;
                }
                if (cache[index] != -1) {
                    return cache[index];
                }
                int m1 = self(self, index - 1);
                int m2 = self(self, index - 2) + nums[index];
                cache[index] = max(m1, m2);
                return cache[index];
                };
            return dfs(dfs, n - 1);
        }
    };
}
namespace s198o2
{   // 注意到o1算法中是自底向上的递归，可以考虑直接只用“归”的过程，用循环从叶子节点直接开始“归”
    // 将递归翻译成递推，降低空间复杂度
    // 1.dfs() -> f数组    2.递归 -> 循环      3.递归边界 -> 数组初始值
    // dfs(i) = max(dfs(i - 1), dfs(i - 2) + nums[i])
    // f[i] = max(f[i - 1], f[i - 2] + nums[i]（可能出现负数下标，所以将i改成从2开始）
    // f[i + 2] = max(f[i + 1], f[i] + nums[i])（注意，这里只是让f数组的下标+2避免负数，不会影响nums[i]）
    // 此时完成了递归至递推的初步翻译，但空间复杂度仍为O(n)，最终版本见o3
    class Solution {
    public:
        int rob(vector<int>& nums) {
            int n = nums.size();
            vector<int> f(n + 2, 0);

            for (int i = 0; i < n; ++i) {
                f[i + 2] = max(f[i + 1], f[i] + nums[i]);
            }

            return f[n + 1];
        }
    };
}
namespace s198o3
{   // 再继续看这两个式子：
    // f[i] = max(f[i - 1], f[i - 2] + nums[i]（可能出现负数下标，所以将i改成从2开始）
    // f[i + 2] = max(f[i + 1], f[i] + nums[i])
    // 可以发现，其实每次更新f都只需要知道“上一个状态”以及“上上一个状态”，所以完全不需要用数组，只需要用两个变量保存
    class Solution {
    public:
        int rob(vector<int>& nums) {
            // f1代表上一个状态，f0代表上上一个状态
            int f0 = 0, f1 = 0;

            // 这下连n都不需要了，直接用ranged for loop
            for (int x : nums) {
                // fnew代表当前状态
                int fnew = max(f1, f0 + x);
                // 更新记录状态的两个变量，注意f0和f1更新的顺序别搞错了
                f0 = f1;
                f1 = fnew;
            }

            return f1;
        }
    };
}

// 模板题2：环形DP，但不用考虑太多，先选头或尾部的一个元素试着分析下，是否能缩小问题规模或直接改变问题的性质
namespace s213o1
{   // 1.如果偷 nums[0]，那么 nums[1] 和 nums[n - 1] 不能偷，问题变成从 nums[2] 到 nums[n - 2] 的非环形版本，复用s198代码；
    // 2.如果不偷 nums[0]，那么问题变成从 nums[1] 到 nums[n - 1] 的非环形版本，同样复用s198代码。
    // 环形问题主要出在首尾的处理上，通过1&2分类讨论，将问题转化成了非环形版本，实现代码复用
    class Solution {
    private:
        // s198代码(加上起点终点)
        int rob1(vector<int>& nums, int start, int end) {
            int f0 = 0, f1 = 0;

            for (int i = start; i < end; ++i) {
                int fnew = max(f1, f0 + nums[i]);
                f0 = f1;
                f1 = fnew;
            }
            return f1;
        }

    public:
        int rob(vector<int>& nums) {
            int n = nums.size();
            // [start, end) 左闭右开
            int m1 = nums[0] + rob1(nums, 2, n - 1); // 偷第一家
            int m2 = rob1(nums, 1, n); // 不偷第一家
            return max(m1, m2);
        }
    };

}

// 模板题5：“值域”上的打家劫舍
namespace s740o1
{
    class Solution {
        // 相当于是值域上的打家劫舍
        // 以 nums = [2, 2, 3, 3, 3, 4]为例
        // 如果选了一个等于3的数，那么2和4都不能再选
        // 另外，因为要最大化点数，只要选了一个等于3的数，所有3都要选
        // 等于可以白嫖，所有数值相同的数等于一间屋子
        // 可以将原始数组转换为值域数组，比如例子中的nums转为：
        // [0, 0, 4, 9, 4]，剩下的就和s198一模一样了
    public:
        int deleteAndEarn(vector<int>& nums) {
            int mx = *max_element(nums.begin(), nums.end());
            vector<int> value(mx + 1, 0);

            for (int x : nums) {
                value[x] += x;
            }

            int f1 = 0, f0 = 0;
            for (int x : value) {
                int fnew = max(f1, f0 + x);
                f0 = f1;
                f1 = fnew;
            }
            return f1;
        }
    };
}
// ---------------------
// 【1.3】最大子数组和（最大子段和）(3)
// 这类题目也可以用前缀和来做，转换成买卖股票的最佳时机，转换成递推时不需要考虑记忆化搜索，因为没有别的递归分支
// 重点还是在于分析f(i), f[i]的意义和表达式，能清晰的写出来离解题就不远了
// 对于子数组问题，枚举选哪个或者选或不选都不行，因为无法保证连续，正确的思路是拼接：
// 将dfs(i)看作是以nums[i]结尾的情况下的数值
/*
53.最大子数组和：给你一个整数数组 nums ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
子数组是数组中的一个连续部分。

1749.任意子数组和的绝对值的最大值：给你一个整数数组 nums 。
一个子数组 [numsl, numsl+1, ..., numsr-1, numsr] 的 和的绝对值 为 abs(numsl + numsl+1 + ... + numsr-1 + numsr) 。
请你找出 nums 中 和的绝对值 最大的任意子数组（可能为空），并返回该 最大值 。

152.乘积最大子数组：给你一个整数数组 nums ，请你找出数组中乘积最大的非空连续 子数组（该子数组中至少包含一个数字），
并返回该子数组所对应的乘积。测试用例的答案是一个 32-位 整数。
请注意，一个只包含一个元素的数组的乘积是这个元素的值。
*/
// ---------------------
// 模板题6：o1为前缀和（更简单），见11.1专题，DP做法则是求max(f)，无需记忆化搜索，直接写递推式即可（Kadane 算法）
// 最基础的连续子数组DP题，用枚举选哪个思路，每次枚举以nums[i]结尾的情况
namespace s53o2
{
// 不能用选或不选思路来做，因为无法保证子数组是连续的，可以用来做子序列题
// 注意，此时返回的不是f[n - 1]，而是max(f)
// f[i] 代表以nums[i]结尾的最大子数组和，分类讨论：
//      1.nums[i]单独组成一个子数组，那么fi[i] = nums[i]
//      2.和前面的子数组拼接起来，也就是“以nums[i - 1]结尾的最大子数组”
//        后面添加nums[i]，也即f[i] = f[i - 1] + nums[i]
// 因此f[i] = :
// 1. i = 0: f[i] = nums[i]
// 2. i > 0: max(f[i - 1], 0) + nums[i](如果f[i - 1]为负就不用拼了)

    // 这里还是先用递归做一遍，虽然意义不大（这里没有记忆化搜索的必要，因为没有别的递归分支，只能一路往下）
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int ans = nums[0];// 数组里随便哪个元素都可以
            int n = nums.size();

            auto dfs = [&](auto&& self, int i)->int {
                if (i == 0) {
                    return nums[0];
                }
                int res = max(self(self, i - 1), 0) + nums[i];
                ans = max(ans, res);// 需要在递归中更新答案
                return res;
                };
            dfs(dfs, n - 1);
            // 注意，答案不是dfs(n - 1)，因为其意义是以nums[i - 1]为结尾的子数组最大值，并不是整个数组的子数组最大值
            return ans;
        }
    };
}
namespace s53o3
{   // 因为比较简单，直接改成优化空间后的递推
    class Solution {
    public:
        int maxSubArray(vector<int>& nums) {
            int f = 0;
            int ans = INT_MIN;
            for (int x : nums) {
                f = max(f, 0) + x;
                ans = max(ans, f);
            }
            return ans;
        }
    };
}

// 相当于求子数组最大和以及最小和，套s53模板即可
namespace s1749o2
{   // 问题变成求最大子数组和，以及最小子数组和
    class Solution {
    public:
        int maxAbsoluteSum(vector<int>& nums) {
            int fmax = 0, fmin = 0;
            int ans = 0;
            for (int x : nums) {
                fmax = max(fmax, 0) + x;
                fmin = min(fmin, 0) + x;
                ans = max({ ans, fmax, -fmin });
            }
            return ans;
        }
    };
}

// 思维扩展：这就转不成前缀和了，本题为s53的乘法版本，这里要同时维护最大和最小乘积，根据当前nums[i]的符号分类讨论
namespace s152o1
{
    // 递推
    class Solution {
    public:
        int maxProduct(vector<int>& nums) {
            int n = nums.size();
            vector<int> fmax(n), fmin(n);
            fmax[0] = fmin[0] = nums[0];
            int ans = nums[0];

            for (int i = 1; i < n; ++i) {
                int x = nums[i];
                fmax[i] = max({ x, fmax[i - 1] * x, fmin[i - 1] * x });
                fmin[i] = min({ x, fmax[i - 1] * x, fmin[i - 1] * x });
                ans = max(ans, fmax[i]);
            }

            return ans;
        }
    };
}
namespace s152o2
{   // 因为存在负数，所以需要维护f[i - 1]的最大值和最小值
    // 所以fmax = max({ fmax * x, fmin * x, x }), fmin同理
    // 也因此写不了递归的版本，直接从递推/空间优化起步
    class Solution {
    public:
        int maxProduct(vector<int>& nums) {
            int ans = INT_MIN;
            int fmax = 1;
            int fmin = 1;

            for (int x : nums) {
                int mx = fmax;
                fmax = max({ fmax * x, fmin * x, x });
                fmin = min({ mx * x, fmin * x, x });
                ans = max(ans, fmax);
            }
            return ans;
        }
    };
}
namespace s152o3
{   // 这个是我写的空间优化版，可以对比o2看看差距在哪
    class Solution {
    public:
        int maxProduct(vector<int>& nums) {
            int n = nums.size();
            int fmax = nums[0], fmin = nums[0];// 这个可以换成1
            int ans = nums[0];

            for (int i = 1; i < n; ++i) {
                int x = nums[i];
                int temp = fmax;
                fmax = max({ x, fmax * x, fmin * x });
                fmin = min({ x, temp * x, fmin * x });
                ans = max(ans, fmax);
            }

            return ans;
        }
    };
}
    
// s53的环形数组版本，不能简单地用双倍数组处理
namespace s918o1
{   // 分类讨论：
    // 1.目标子数组在原数组两端范围内：s53
    // 2.目标子数组拆分成了两部分，一端在头，一端在尾，那么夹在中间的子数组越小，目标子数组越大
    //   也即sum - min_s
    class Solution {
    public:
        int maxSubarraySumCircular(vector<int>& nums) {
            int max_f = 0; // 计算最大子数组和的 DP 数组（空间优化成一个变量）
            int max_s = INT_MIN; // 最大子数组和，不能为空
            int min_f = 0; // 计算最小子数组和的 DP 数组（空间优化成一个变量）
            int min_s = 0; // 最小子数组和，可以为空（元素和为 0），这道题的易错点就在这里
            int sum = 0; // nums 的元素和

            for (int x : nums) {
                // 53. 最大子数组和（空间优化写法）
                max_f = max(max_f, 0) + x;
                max_s = max(max_s, max_f);
                min_f = min(min_f, 0) + x;
                min_s = min(min_s, min_f);
                sum += x;
            }
            // 对情况2下的特殊情况（最小子数组是整个数组，也即拆成两半的最大子树组是空的，为0）特判：
            // 1.如果情况1下的最大子数组是正数，可以覆盖上述情况，没问题
            // 2.如果情况1下的最大子数组是负数，那么会返回0，是错误答案，因为题干要求目标子数组非空，直接返回情况1的答案
            return max_s < 0 ? max_s : max(max_s, sum - min_s);
        }
    };
}
// ------------------------------------------------------------------------------------
// 二、网格图DP
// 【2.1】网格图基础(4)
/*
64.最小路径和：给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
说明：每次只能向下或者向右移动一步。

62.不同路径：一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。
机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。
问总共有多少条不同的路径？

63.不同路径II：给定一个 m x n 的整数数组 grid。一个机器人初始位于 左上角（即 grid[0][0]）。
机器人尝试移动到 右下角（即 grid[m - 1][n - 1]）。机器人每次只能向下或者向右移动一步。
网格中的障碍物和空位置分别用 1 和 0 来表示。机器人的移动路径中不能包含 任何 有障碍物的方格。
返回机器人能够到达右下角的不同路径数量。
测试用例保证答案小于等于 2 * 10^9。

120.三角形最小路径和：给定一个三角形 triangle ，找出自顶向下的最小路径和。每一步只能移动到下一行中相邻的结点上。
相邻的结点 在这里指的是 下标 与 上一层结点下标 相同或者等于 上一层结点下标 + 1 的两个结点。
也就是说，如果正位于当前行的下标 i ，那么下一步可以移动到下一行的下标 i 或 i + 1 。
*/
// ---------------------
// 模板题7：o1为常规回溯思路（自顶向下） + 记忆化搜索，o2开始是DP入门三部曲（自底向上）,直接从o3开始看
namespace s64m1
{   // 自己写的代码，对边界的处理还是不行，实在不会处理边界，尤其是转角处的网格值（自己写会导致加两遍转角处）
    // 这版代码意味着进入格子时就已经用了当前格子的值，所以边界处理很简单
    // 增加了记忆化搜索，勉勉强强通过了
    class Solution {
        // 固定从(0, 0)到(n - 1, n - 1)
        // 每次只能向下或者向右移动一步, r,c只能增大
        // 向右：c + 1，注意移动时的边界
        // 向下：r + 1，注意移动时的边界
        // 当r == n - 1 && n - 1时更新ans

        // 记忆化搜索 + 简单剪枝
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size();
            int n = grid[0].size();
            vector<vector<int>> cache(m, vector<int>(n, INT_MAX));
            cache[0][0] = grid[0][0];

            auto dfs = [&](auto&& self, int sum, int r, int c)-> void {
                if (r == m - 1 && c == n - 1) {
                    return;
                }

                if (r + 1 < m) {
                    if (cache[r + 1][c] > sum + grid[r + 1][c]) {
                        cache[r + 1][c] = sum + grid[r + 1][c];
                        self(self, sum + grid[r + 1][c], r + 1, c);
                    }
                }
                if (c + 1 < n) {
                    if (cache[r][c + 1] > sum + grid[r][c + 1]) {
                        cache[r][c + 1] = sum + grid[r][c + 1];
                        self(self, sum + grid[r][c + 1], r, c + 1);
                    }
                }
                };
            dfs(dfs, grid[0][0], 0, 0);

            return cache[m - 1][n - 1];
        }
    };
}
namespace s64o1
{   // 代码比m1好看多了，但是效率是m1的1／3（也可能提交时正好是一台慢机器），也不知道为什么
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            vector<vector<int>> cache(m, vector<int>(n, INT_MAX));

            auto dfs = [&](auto&& dfs, int r, int c, int sum)->void {
                if (r >= m || c >= n) return;

                // 进入当前格子时才加上它的值
                sum += grid[r][c];

                // 已经有更优路径
                if (sum >= cache[r][c]) return;
                cache[r][c] = sum;

                // 到达终点
                if (r == m - 1 && c == n - 1) return;

                // 在递归前，就计算好sum，那么就不怕越界了
                dfs(dfs, r + 1, c, sum);// 向下
                dfs(dfs, r, c + 1, sum);// 向右
                };
            dfs(dfs, 0, 0, 0);
            return cache[m - 1][n - 1];
        }
    };
}
namespace s64o2
{   // 自底向上的DP递归思路，当然这个版本是会超时的，没有记忆化搜索
    // 定义 dfs(i,j) 表示从左上角到第 i 行第 j 列这个格子（记作 (i,j)）的最小价值和
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            auto dfs = [&](auto&& dfs, int i, int j) {
                if (i < 0 || j < 0) {
                    return INT_MAX;
                }
                if (i == 0 && j == 0) {
                    return grid[0][0];
                }
                return min(dfs(dfs, i - 1, j), dfs(dfs, i, j - 1)) + grid[i][j];
                };
            return dfs(dfs, m - 1, n - 1);
        }
    };
}
namespace s64o3
{   // 光换个方向，改成自底向上后，跟o1对比，同样是记忆化搜索，效率已经翻了几十倍，只能说如果不是常规的回溯题，递归一律DP
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();

            vector<vector<int>> cache(m, vector<int>(n, -1));
            cache[0][0] = grid[0][0]; // 将特例提前初始化更快，可以节省掉不少if判断语句

            auto dfs = [&](auto&& dfs, int i, int j) {
                if (i < 0 || j < 0) {
                    return INT_MAX;// 递归边界，使其保证不影响结果，且不溢出
                }
                if (cache[i][j] != -1) {
                    return cache[i][j];
                }
                /*
                if (i == 0 && j == 0) {
                    return grid[0][0];// 唯一一个可能两边都可能是超出边界的点，需要特殊处理
                }
                */
                return cache[i][j] = grid[i][j] + min(dfs(dfs, i - 1, j), dfs(dfs, i, j - 1));
                };
            return dfs(dfs, m - 1, n - 1);
        }
    };
}
namespace s64o4
{   // 改成递推，这个形式是最容易去理解元素更新过程的，在草稿纸上模拟一下运算过程就能很快理解空间优化o5的原理
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            vector<vector<int>> f(m + 1, vector<int>(n + 1, INT_MAX));
            f[1][0] = f[0][1] = 0;// 或者只设一个为0也行，无所谓，只要两个min之后为0即可

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    // 此时唯一可能遭到影响的边界就是f[1][1]了
                    // 只需将f[0][1]和f[1][0]设为0即可
                    // f[i + 1][j + 1]的只依靠左边和上边的元素，所以只有最开是的f[1][1]推成功了
                    // 后面的f[1][2]，f[1][3]等元素也会自然推导成功（有min来确保不会取到INT_MAX）
                    f[i + 1][j + 1] = grid[i][j] + min(f[i][j + 1], f[i + 1][j]);
                }
            }
            return f[m][n];
        }
    };
}
namespace s64o5
{   // 空间优化（不同于一维的DP，普通优化后空间复杂度也只能降为O(n)，需要用一个一维数组保存之前的状态
    // 单独拿出一行来分析，需要储存n + 1个状态，跟列数相关：
    // 在计算 f[1][1] 时，会用到 f[0][1]，但是之后就不再用到了。
    // 那么干脆把 f[1][1] 记到 f[0][1] 中，这样对于 f[1][2] 来说，
    // 它需要的数据就在 f[0][1] 和 f[0][2] 中。f[1][2] 算完后也可以同样记到 f[0][2] 中。
    // 格子左边的数据是被覆盖后的，上方的数据是一维数组里对应位置的旧数据，计算完当前格子后，将当前格子数据覆盖至对应位置
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            vector<int> f(n + 1, INT_MAX);
            // 不能初始化f[0] = 0，会导致第二行开始的列首min判断错误，这里是陷阱
            f[1] = 0; // ok，因为这个0会一开始就被覆盖，完成边界使命

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    f[j + 1] = min(f[j], f[j + 1]) + grid[i][j];
                }
            }
            return f[n];
        }
    };
}
namespace s64o6
{   // 原地修改空间优化，直接空间复杂度降为O(1)，直接用grid[0]当作数组
    // 这样优化我其实感觉没必要，牺牲了代码的可读性，而且污染了原始数据
    // 同时因为grid[0]的大小只有n，所以还需要额外的边界特判代码
    // 用grid[0]来替代f，因为长度只有n，所以按照最原始的式子：
    // f[i][j]=min(f[i][j-1],f[i-1][j])+grid[i][j]
    // i=0 时，上式为 f[i][j]=f[i][j-1]+grid[i][j]
    // 用一个数组时，为 f[j]=f[j-1]+grid[0][j]=f[j-1]+f[j]（grid[0] 就是 f 数组）
    // j=0 时，上式为 f[i][j]=f[i-1][j]+grid[i][j]
    // 用一个数组时，为 f[0]=f[0]+grid[i][0]
    class Solution {
    public:
        int minPathSum(vector<vector<int>>& grid) {
            int m = grid.size(), n = grid[0].size();
            auto& f = grid[0];

            // i = 0的情况，先处理一行
            // 此时f[0]就是grid[0]，不用特别初始化
            for (int j = 1; j < n; j++) {
                f[j] += f[j - 1];
            }

            // 因为i = 0的第一行已经被处理了，所以i从1开始
            for (int i = 1; i < m; i++) {
                f[0] += grid[i][0];// 特殊处理j = 0的点
                for (int j = 1; j < n; j++) {
                    f[j] = min(f[j - 1], f[j]) + grid[i][j];
                }
            }
            return f[n - 1];
        }
    };

}

// 计算从左上走到右下的路径数，比64简单，可以直接写空间优化版本
namespace s62m1
{   // 记忆化搜索
    class Solution {
    public:
        int uniquePaths(int m, int n) {
            vector<vector<int>> cache(m, vector<int>(n, -1));

            function<int(int, int)> dfs = [&](int i, int j) {
                if (i < 0 || j < 0) {
                    return 0;
                }

                if (i == 0 && j == 0) {
                    return 1;
                }

                if (cache[i][j] != -1) {
                    return cache[i][j];
                }
                cache[i][j] = dfs(i - 1, j) + dfs(i, j - 1);
                return cache[i][j];
                };
            return dfs(m - 1, n - 1);
        }
    };
}
namespace s62m2
{   // 递推
    class Solution {
    public:
        int uniquePaths(int m, int n) {
            vector<vector<int>> f(m + 1, vector<int>(n + 1));
            f[1][0] = 1;// f[0][1]设为1也没问题
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    f[i + 1][j + 1] = f[i][j + 1] + f[i + 1][j];
                }
            }
            return f[m][n];
        }
    };
}
namespace s62m3
{   // 空间优化
    class Solution {
    public:
        int uniquePaths(int m, int n) {
            vector<int> f(n + 1);
            f[1] = 1;
            // 注意，空间优化后递推助力要谨慎选择，如果设f[0] = 1，第一轮递推是正确的
            // 但第二轮递推中f[1]又会加一遍f[0]，此时f[0]需要为0，所以这种形式的空间优化下只能初始化f[1] = 1
            // 
            // 第一轮中f[1] = 1, f[0] = 0，答案还是正确的
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    f[j + 1] = f[j] + f[j + 1];
                    // f[j + 1] += f[j]; 也就相当于这个
                }
            }

            return f[n];
        }
    };
}

// 加了障碍物的s62，比较简单，也可以直接写空间优化版本
namespace s63m1
{   // 记忆化搜索
    class Solution {
    public:
        int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
            int m = obstacleGrid.size(), n = obstacleGrid[0].size();
            if (obstacleGrid[0][0] || obstacleGrid[m - 1][n - 1]) {
                return 0;
            }

            vector<vector<int>> cache(m, vector<int>(n, -1));

            function<int(int, int)> dfs = [&](int i, int j) {
                // 有障碍物的情况就相当于越界了，到达障碍物格子的路径为0条
                if (i < 0 || j < 0 || obstacleGrid[i][j] == 1) {
                    return 0;
                }
                if (i == 0 && j == 0) {
                    return 1;// 特判之后，默认起始点一定没有障碍物，路径数量为1
                }
                if (cache[i][j] != -1) {
                    return cache[i][j];
                }
                cache[i][j] = dfs(i - 1, j) + dfs(i, j - 1);
                return cache[i][j];
                };

            return dfs(m - 1, n - 1);
        }
    };
}
namespace s63m2
{   // 递推
    class Solution {
    public:
        int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
            int m = obstacleGrid.size(), n = obstacleGrid[0].size();
            if (obstacleGrid[0][0] || obstacleGrid[m - 1][n - 1]) {
                return 0;
            }

            vector<vector<int>> f(m + 1, vector<int>(n + 1));
            f[0][1] = 1;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    f[i + 1][j + 1] = (obstacleGrid[i][j] == 1) ? 0 : f[i + 1][j] + f[i][j + 1];
                }
            }
            return f[m][n];
        }
    };
}
namespace s63m3
{   // 空间优化至O(n)（原地修改的代码就不写了，感觉有点太过了）
    class Solution {
    public:
        int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
            int m = obstacleGrid.size(), n = obstacleGrid[0].size();
            if (obstacleGrid[0][0] || obstacleGrid[m - 1][n - 1]) {
                return 0;
            }
            vector<int> f(n + 1);
            f[1] = 1;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    // 障碍物处必须置0，与递推逻辑无关
                    f[j + 1] = obstacleGrid[i][j] ? 0 : f[j + 1] + f[j];
                }
            }
            return f[n];
        }
    };
}

// 思路虽然和标准做法有些偏差（计算方向反了），但也能做出来
namespace s120m1
{   // 初步回溯，确定思路，但没有加记忆化搜索，一定超时
    // 每行节点个数一定是上一行数量加1，第一行有1个节点
    class Solution {
    public:
        int minimumTotal(vector<vector<int>>& triangle) {
            int m = triangle.size(), n = triangle.back().size();
            function<int(int, int)> dfs = [&](int i, int j) {
                // j > i是因为三角形结构带来的边界判断
                if (i < 0 || j < 0 || j > i) {
                    return INT_MAX;
                }
                if (i == 0 && j == 0) {
                    return triangle[0][0];
                }
                return min(dfs(i - 1, j), dfs(i - 1, j - 1)) + triangle[i][j];
                };
            int ans = INT_MAX;
            for (int j = 0; j < n; ++j) {
                ans = min(ans, dfs(m - 1, j));
            }
            return ans;
        }
    };
}
namespace s120m2
{   // 加了个记忆化搜索
    class Solution {
    public:
        int minimumTotal(vector<vector<int>>& triangle) {
            // 复制一个三角形矩阵，同时将元素全部置为-1，也可以直接创建一个m * m的方形矩阵，虽然会有很多浪费的空间
            vector<vector<int>> cache = triangle;
            for (auto& row : cache) {
                for (int& x : row) {
                    x = INT_MIN;// triangle[i][j]可能是负数，不能随便设个-1了
                }
            }

            function<int(int, int)> dfs = [&](int i, int j) {
                if (i < 0 || j < 0 || j > i) {
                    return INT_MAX;
                }
                if (i == 0 && j == 0) {
                    return triangle[0][0];
                }
                if (cache[i][j] != -1) {
                    return cache[i][j];
                }
                cache[i][j] = min(dfs(i - 1, j), dfs(i - 1, j - 1)) + triangle[i][j];
                return cache[i][j];
                };

            int m = triangle.size(), n = triangle.back().size();
            int ans = INT_MAX;
            for (int j = 0; j < n; ++j) {
                ans = min(ans, dfs(m - 1, j));
            }
            return ans;
        }
    };
}
namespace s120m3
{   // 直接写成空间优化后的递推
    class Solution {
    public:
        int minimumTotal(vector<vector<int>>& triangle) {
            int m = triangle.size();
            vector<int> f(m + 1, INT_MAX);
            f[1] = 0;

            for (int i = 0; i < m; ++i) {
                // 注意每行逆序遍历，如果正序遍历那么f[j]会被当前行的数据覆盖，用到的就不是上一行的数据了
                for (int j = triangle[i].size() - 1; j >= 0; --j) {
                    // 可以直接写j = i
                    f[j + 1] = min(f[j + 1], f[j]) + triangle[i][j];
                }
            }

            int ans = *(min_element(f.begin(), f.end()));
            return ans;
        }
    };
}
namespace s120o1
{   // 其实这道题从三角形底部往上计算更好
    // 首先是记忆化搜索的DFS
    class Solution {
    public:
        int minimumTotal(vector<vector<int>>& triangle) {
            vector<vector<int>> cache = triangle;
            for (auto& row : cache) {
                for (int& x : row) {
                    x = INT_MIN;
                }
            }

            int n = triangle.size();

            function<int(int, int)> dfs = [&](int i, int j) {
                // 自底向上的一个好处是边界条件简单，只需要判断n - 1即可
                if (i == n - 1) {
                    return triangle[i][j];
                }
                if (cache[i][j] == -1) {
                    return cache[i][j];
                }
                // 每次往下方或斜右下方走
                cache[i][j] = min(dfs(i + 1, j), dfs(i + 1, j + 1)) + triangle[i][j];
                return cache[i][j];
                };
            // 返回值直接写dfs(0, 0)即可，不用再多一次遍历
            return dfs(0, 0);
        }
    };
}
namespace s120o2
{   // 自底向上计算的空间优化递推
    class Solution {
    public:
        int minimumTotal(vector<vector<int>>& triangle) {
            int n = triangle.size();
            // 直接拿三角形最后一行来用，长度天然比倒数第二行大1，自然不会越界
            // 而且连数据都初始化好了
            vector<int> f = triangle[n - 1];

            for (int i = n - 2; i >= 0; --i) {
                for (int j = 0; j <= i; ++j) {
                    f[j] = min(f[j], f[j + 1]) + triangle[i][j];
                }
            }

            // 返回值也很简单
            return f[0];
        }
    };
}
// ---------------------
// 【2.2】网格图进阶(1)
/*
329.
*/
// ---------------------
// 暂时不做，时间有限
namespace s329o1
{

}