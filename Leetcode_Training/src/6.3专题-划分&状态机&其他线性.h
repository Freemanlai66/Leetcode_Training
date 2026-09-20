#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <functional>
#include <algorithm>

using namespace std;

// 问题待定：
/*
1.其他线性DP完全跳过了,s221最大正方形没有做
*/

/*
模板题：
1.划分型DP例题:139
2.买卖股票不限交易次数：122
3.买卖股票增加冷冻期：309
4.买卖股票至多交易k次：188
*/

// 动态规划：划分型DP + 状态机DP + 其他线性DP

// 五、划分型DP(1)：时间有限，目前没有进行系统性学习，只选了一道最高频的s139刷
// 【5.1】判定能否划分 (1)
// 一般定义 f[i] 表示长为 i 的前缀 a[:i] 能否划分，
// 枚举最后一个子数组的左端点 L，从 f[L] 转移到 f[i]，并考虑 a[L:i] 是否满足要求。
/*
139.单词划分：给你一个字符串 s 和一个字符串列表 wordDict 作为字典。
如果可以利用字典中出现的一个或多个单词拼接出 s 则返回 true。
注意：不要求字典中出现的单词全部都使用，并且字典中的单词可以重复使用。
*/
// ---------------------
// 模板题1：划分型DP例题，只有递归和记忆化搜索，无空间优化
namespace s139o1    
{	// 例如 s = leetcode，枚举最后一段的长度：
	// 长为 1，即子串 e，如果它在 wordDict 中，那么问题变成：能否把 leetcod 划分成若干段，使得每段都在 wordDict 中？
	// 长为 2，即子串 de，如果它在 wordDict 中，那么问题变成：能否把 leetco 划分成若干段，使得每段都在 wordDict 中？
	// 注：字典里有1000种字符，但字符长度最多为20，所以应该枚举字符的长度，总共20种，先遍历一遍得到maxLen
    
	// 定义dfs(i)，表示能否把前缀 s[:i]（表示 s[0] 到 s[i - 1] 这段子串）划分成若干段，使得每段都在 wordDict 中
    // 计算dfs(i)时，从[0, 1, 2, i - maxLen]枚举j，如果[j, i - 1]在wordDict中，那么继续向下递归判断dfs(j)
	// 设 wordDict 中字符串的最长长度为 maxLen，枚举的上限不超过 maxLen，因为更长的子串必然不在 wordDict 中
    // 例子：s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"], return: false
    // 递归边界：dfs(0) = true，递归到空串，说明s成功划分完毕
    // 递归入口：dfs(n)，也就是答案

    // 下面是记忆化搜索的版本
    class Solution {
    public:
        bool wordBreak(string s, vector<string>& wordDict) {
            // 将字典转换为哈希集合
            unordered_set<string> words(wordDict.begin(), wordDict.end());

            // 获取字典中字符串最大长度
            int maxLen = 0;
            for (const string& word : words) {
                // 注意word.length()的返回值类型是size_t，需要显式类型转换为int
                maxLen = max(maxLen, (int)word.length());
            }

            // 缓存数组
            int n = s.length();
            vector<int> memo(n + 1, -1); // -1 表示没有计算过

            // dfs(i)代表的是[0, i)段的字符能否划分成若干段，区间左闭右开
            auto dfs = [&](auto&& dfs, int i)->bool {
                // 递归边界：dfs(0) = true，递归到空串，说明划分完毕
                if (i == 0) {
                    return true;
                }
                int& res = memo[i];
                if (res != -1) {
                    // 注意lambda函数类型自动转换不太灵敏，最好进行显式类型转换，否则可能报错
                    return static_cast<bool>(res);
                }

                // 枚举最后一段的长度，区间是左闭右开[0, i)，区间总长度为： i - 1 - 0 + 1 = i
                // 枚举的内层循环 j 从 i - 1（也即下标）开始，代表枚举子串的起点下标
                // 字典中最长的字符串长度为maxLen，起点下标不可能比i - maxLen小，更小的起点下标无需判断， 一定不在字典内
                // i - maxLen则代表最长的有可能在字典里的字符，同时因为下标最小为0，需要与0取max
                for (int j = i - 1; j >= max(i - maxLen, 0); --j) {
                    // substr(j, i - j)，也即[j, i - 1]段子串
                    // 因为是左闭右开，从下标j开始，所以长度为i - j
                    // 满足条件：在字典里 && 子问题返回值为true
                    if (words.count(s.substr(j, i - j)) && dfs(dfs, j)) {
                        return res = true;
                    }
                }

                return res = false;
                };
            // 递归入口：dfs(n)
            return dfs(dfs, n);
        }
    };
}
namespace s139o2
{   // 递推
    class Solution {
    public:
        bool wordBreak(string s, vector<string>& wordDict) {
            unordered_set<string> wordSet(wordDict.begin(), wordDict.end());

            int maxLen = 0;
            for (const string& word : wordSet) {
                maxLen = max(maxLen, (int)word.size());
            }

            int n = s.size();
            vector<bool> f(n + 1);
            f[0] = true;// 翻译自递归边界dfs(0) = true

            for (int i = 1; i <= n; ++i) {
                int bottom = max(0, i - maxLen);
                for (int j = i - 1; j >= bottom; --j) {
                    string sub = s.substr(j, i - j);
                    if (wordSet.count(sub) && f[j]) {
                        f[i] = true;
                        // 对每个i，只要有一个j能使其满足条件，就可以直接结束循环，转去处理i + 1
                        break;
                    }
                }
            }

            return f[n]; // 翻译自递归入口dfs(n)
        }
    };
}
// ------------------------------------------------------------------------------------
// 六、状态机DP(3)
// 【6.1】买卖股票 (4)
/*
122.买卖股票的最佳时机 II：不限交易次数
给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格。
在每一天，你可以决定是否购买和/或出售股票。你在任何时候 最多 只能持有 一股 股票。
然而，你可以在 同一天 多次买卖该股票，但要确保你持有的股票不超过一股。
返回 你能获得的 最大 利润 。

309.买卖股票的最佳时机含冷冻期：不限交易次数 + 冷冻期
给定一个整数数组prices，其中 prices[i] 表示第 i 天的股票价格。
设计一个算法计算出最大利润。在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）:
卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

188.买卖股票的最佳时机 IV：限制交易至多k次
给你一个整数数组 prices 和一个整数 k ，其中 prices[i] 是某支给定的股票在第 i 天的价格。
设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。也就是说，你最多可以买 k 次，卖 k 次。
注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

123.买卖股票的最佳时机 III：限制交易至多2次，s188的特化版，k = 2即可
*/
// ---------------------
// 模板题2：状态机股票问题通用解法（不限交易次数）
namespace s122o1
{   /*
    有点类似“选或不选”，但是额外增加了“状态”（此处为是否持有股票），进而增加了初始状态与状态间的转移

    定义dfs(i, 0)表示到第i天“结束”时，未持有股票的最大利润
    定义dfs(i, 1)表示到第i天“结束”时，持有股票的最大利润
    注：第二参数为表示是否持有股票的布尔值

    由于第i - 1天的“结束”就是第i天的“开始”，dfs(i - 1, 1/0)也表示到第i天“开始”时的最大利润
    总共有四种状态转变：注意从始至终同时只能最多持有一张股票
    1.未持有状态 -> 未持有状态（什么也不做）：dfs(i, 0) = dfs(i - 1, 0)
    2.持有状态 -> 未持有状态（卖出）：dfs(i, 0) = dfs(i - 1, 1) + prices[i]
    3.持有状态 -> 持有状态（什么也不做）：dfs(i, 1) = dfs(i - 1, 1)
    4.未持有状态 -> 持有状态（买入）：dfs(i, 1) = dfs(i - 1, 0) - prices[i]

    可以推出以下式子：
    式1: dfs(i, 0) = max( dfs(i - 1, 0), dfs(i - 1, 1) + prices[i] )
    式2: dfs(i, 1) = max( dfs(i - 1, 1), dfs(i - 1, 0) - prices[i] )

    递归边界：
    dfs(-1, 0) = 0      第0天“开始”时未持有股票，利润为0
    dfs(-1, 1) = -inf   第0天开始时不可能持有股票（这种不合法的情况会被式1和式2自动排除，因为是取max）
    注：在初始化为-inf可能导致溢出时，dfs(-1, 1)也可以初始化为-prices[0]

    递归入口：max( dfs(n - 1, 0), dfs(n - 1, 1) ) = dfs(n - 1, 0)，即最后一天“结束”时未持有股票
    最后一天的股票一定是要卖出去的，因为接下来股票就用不上了，所以递归入口一定是dfs(n - 1, 0)
    */

    // 增加记忆化搜索（该种类型记忆化搜索只是能保证堪堪不超时，最低限度的面试解法是递推，此处只是为方便理解才给出）
    // 时空复杂度皆为O(n)
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int n = prices.size();
            vector<vector<int>> memo(n, { -1, -1 });
            // 此时默认对应下标的持有/未持有的利润为-1时代表没有访问过
            // 此处其实是有瑕疵，因为递归过程中利润可能为负，但无伤大雅，记忆化搜索的基本功能依旧能实现

            function<int(int, bool)> dfs = [&](int i, bool hold) {
                // 递归边界
                if (i < 0) {
                    return hold ? INT_MIN : 0;
                }
                int& res = memo[i][hold];// 用别名的方法简化代码（之前一直不学着灵神用，这次试了下发现很容易模仿）
                if (res != -1) {
                    return res;
                }
                if (hold) {
                    // 赋值的同时进行return，进一步简化代码
                    return res = max(dfs(i - 1, true), dfs(i - 1, false) - prices[i]);
                }
                return res = max(dfs(i - 1, false), dfs(i - 1, true) + prices[i]);
                };

            return dfs(n - 1, false);
        }
    };
}
namespace s122o2
{
    /*
    与o1中的递归分析一一对应：

    f[i][0] = max(f[i - 1][0], f[i - 1][1] + prices[i])
    f[i][1] = max(f[i - 1][1], f[i - 1][0] - prices[i])
    但这样下标i - 1可能为-1，即存在f[-1][0]和f[-1][1]
    将f[i]整体移动并初始化f[0](原来的f[-1])：
    f[i + 1][0] = max(f[i][0], f[i][1] + prices[i])
    f[i + 1][1] = max(f[i][1], f[i][0] - prices[i])

    递归边界翻译成：f[0][0] = 0；   f[0][1] = -inf
    递归入口翻译成：f[n][0]
    */

    // 1:1翻译成递推
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int n = prices.size();
            vector<vector<int>> f(n + 1, { 0, 0 });
            f[0][1] = INT_MIN;

            for (int i = 0; i < n; ++i) {
                f[i + 1][0] = max(f[i][0], f[i][1] + prices[i]);
                f[i + 1][1] = max(f[i][1], f[i][0] - prices[i]);
            }
            return f[n][0];
        }
    };
}
namespace s122o3
{   // 空间优化
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            // 可以只用两个变量记录持有和未持有下的最大利润
            int f0 = 0;// 未持有状态的初始化边界
            int f1 = -prices[0];// 持有状态的初始化边界

            for (int p : prices) {
                int newf0 = max(f0, f1 + p);// 需要用临时变量储存下f0
                f1 = max(f1, f0 - p);
                f0 = newf0;
            }
            return f0;
        }
    };
}

// 在s122基础上加了交易费，稍微修改一下状态方程即可
namespace s714m1
{
    class Solution {
    public:
        int maxProfit(vector<int>& prices, int fee) {
            // dfs(i, 0) = max(dfs(i - 1, 0), dfs(i - 1, 1) + p - fee)
            // dfs(i, 1) = max(dfs(i - 1, 1), dfs(i - 1, 0) - p)
            // dfs(-1, 0) = 0
            // dfs(-1, 1) = INT_MIN / 2

            // f[i + 1][0] = max(f[i][0], f[i][1] + p - fee)
            // f[i + 1][1] = max(f[i][1], f[i][0] - p)
            // f[0][0] = 0
            // f[0][1] = INT_MIN / 2

            // 这里f1的初始化不能设为-prices[0] + fee，而是最大只能为-prices[0]
            // dfs(0, 0)当然不影响，结果是max(0, 0)，但是dfs(0, 1)是错的
            // dfs(0, 1)的结果原本应该是-prices[0]，而因为dfs(-1, 1)有额外的一个fee所以变成了-prices[0] + fee
            // 这个错误结果在之后的递推中永远不会被覆盖
            // dfs(0, 0) = max(dfs(-1, 0), dfs(-1, 1) + p - fee)
            // dfs(0, 1) = max(dfs(-1, 1), dfs(-1, 0) - p)  
            // 在设置股票交易初始化值的时候要两个状态式都观察而不是只看一个
            int f0 = 0, f1 = -prices[0];
            for (int p : prices) {
                int f0_new = max(f0, f1 + p - fee);
                f1 = max(f1, f0 - p);
                f0 = f0_new;
            }
            return f0;
        }
    };
}

// 模板题3：状态机股票问题通用解法（不限交易次数 + 包含冷冻期），冷冻期指买入后不能隔天立刻卖出
// 针对买入或卖出的i - 1改成i - 2，递推需要右移，空间优化需要增加一个变量
namespace s309o1
{   // 有点类似打家劫舍，相间隔的天数不能选
    // 注意：只要求卖出股票的第二天不能买入股票，所以只影响dfs(i, 1)
    // 可以复用s122代码，只需改动一个地方

    // 注意 dfs(i-2,0) 并不意味着第 i-2 天一定卖了股票，而是在没有股票下的最优状态
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int n = prices.size();
            vector<vector<int>> memo(n, { -1, -1 });

            function<int(int, bool)> dfs = [&](int i, bool hold) {
                if (i < 0) {
                    return hold ? INT_MIN : 0;
                }
                int& res = memo[i][hold];
                if (res != -1) {
                    return res;
                }
                if (hold) {
                    // 只影响第i天“结束”时持有股票的递归路径
                    // 此时要求前一天不能存在股票的卖出操作
                    // 也即，如果前一天未持有股票，就可能是卖出导致的，需要再前移一天(i - 2)
                    return res = max(dfs(i - 1, true), dfs(i - 2, false) - prices[i]);
                }
                return res = max(dfs(i - 1, false), dfs(i - 1, true) + prices[i]);
                };
            return dfs(n - 1, false);
        }
    };
}
namespace s309o2
{   // 1:1翻译成递推，基本和s122一致
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int n = prices.size();
            // 因为存在i - 2，也即下标最小为-2，让f[i]整体右移两位
            vector<vector<int>> f(n + 2);
            f[1][1] = -prices[0];// 相应的递推边界也右移一位

            for (int i = 0; i < n; ++i) {
                f[i + 2][0] = max(f[i + 1][0], f[i + 1][1] + prices[i]);
                f[i + 2][1] = max(f[i + 1][1], f[i][0] - prices[i]);
            }
            return f[n + 1][0];// 递推入口同理
        }
    };
}
namespace s309o3
{   // 空间优化
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            // i - 2导致的一个数变成第三个变量，代表着f0的上一个值(也即pre）
            int pre0 = 0, f0 = 0;
            int f1 = -prices[0];

            for (int p : prices) {
                int newf0 = max(f0, f1 + p);
                f1 = max(f1, pre0 - p);
                pre0 = f0;// 注意变量更新顺序
                f0 = newf0;
            }
            return f0;
        }
    };
}

// 模板题4：状态机股票问题通用解法（限制“至多”交易k次），延伸至“恰好”/“至少”交易k次，三维DP
// o2中对于递推矩阵初始化的讨论可以有助于理解DP的本质：只需要关注前驱，而非初始状态本身
// 增加一个维度j，记录交易次数，并针对买入或卖出，将j改为j - 1，递推需要设置为3维矩阵，空间优化需要逆向遍历2维矩阵
namespace s188o1
{   
    /*  在递归过程中记录交易次数
        定义dfs(i, j, 0)表示到第i天结束时完成至多j笔交易，未持有股票的最大利润
        定义dfs(i, j, 1)表示到第i天结束时完成至多j笔交易，持有股票的最大利润
        
        状态方程相较于s122不限交易次数的变化之处在于：
        在由未持有->持有（买入）的路径中，将j - 1

        总共有四种状态转变：注意从始至终同时只能最多持有一张股票
        1.未持有状态 -> 未持有状态（什么也不做）：dfs(i, j, 0) = dfs(i - 1, j, 0)
        2.持有状态 -> 未持有状态（卖出）：dfs(i, j, 0) = dfs(i - 1, j, 1) + prices[i]
        3.持有状态 -> 持有状态（什么也不做）：dfs(i, j, 1) = dfs(i - 1, j, 1)
        4.未持有状态 -> 持有状态（买入）：dfs(i, j, 1) = dfs(i - 1, j - 1, 0) - prices[i](此处进行j - 1)
        注：在状态变化2卖出中，写j - 1，在状态变化4买入中写j也是可以的，只在一端中减小j
            限制买k次或是限制卖k次都可以，为了和冷冻期那题对应，就在买入转移过程修改

        可以推出以下式子：
        式1: dfs(i, j, 0) = max( dfs(i - 1, j, 0), dfs(i - 1, j, 1) + prices[i] )
        式2: dfs(i, j, 1) = max( dfs(i - 1, j, 1), dfs(i - 1, j - 1, 0) - prices[i] )

        递归边界：
        dfs(..., -1, ...) = -inf，任何情况下j都不能为负
        dfs(-1, j, 0) = 0，第一天开始时未持有股票，利润为0
        dfs(-1, j, 1) = -inf，第一天开始时不可能持有股票

        递归入口：
        max( dfs(n - 1, k, 0), dfs(n - 1, k, 1) ) = dfs(n - 1, k, 0)，即最后一天“结束”时未持有股票
    */

    // 记忆化搜索写法，大致与s122相同，但是要注意递归边界的写法，防止溢出
    class Solution {
    public:
        int maxProfit(int k, vector<int>& prices) {
            int n = prices.size();
            // 注意是k + 1，因为递归入口是k，记忆缓存数组要提供k的下标
            // k = 0时并不是没有意义的，只是不能交易而已，可以空过或是卖出（对应下面的写法，只有买入时消耗交易次数）
            vector<vector<vector<int>>> memo(n, vector<vector<int>>(k + 1, { -1, -1 }));

            function<int(int, int, bool)> dfs = [&](int i, int j, bool hold) {
                if (j < 0) {
                    return INT_MIN / 2;// 防止溢出1
                    // 这里和i < 0的地方都写-prices[0]可能导致答案错误
                    // 因为如果用-prices[0]时，当i = 0, j = 0时有：
                    // dfs(0, 0, true) = max(dfs(0 - 1, 0, true), dfs(0 - 1, 0 - 1, false) - prices[0]);
                    //                  = max(dfs(-1, 0, true), dfs(-1, -1, false) - prices[0]);
                    //                  = max(-prices[0], -prices[0] - prices[0]) = - prices[0]
                    // 在s188中，此处虽然也是-prices[0]，但确是有意义，代表第0天买入了股票
                    // 在s122中，这个地方虽然同样得到了-prices[0]的结果，但这个数并无意义
                    // 也即原本只是想用来凑数避过max选择的无意义数值-prices[0]，可能真的被选中并影响到后续结果
                    // 所以两个地方都要用INT_MIN / 2，并在上一层的递归中被排除掉
                }
                if (i < 0) {
                    return hold ? INT_MIN / 2 : 0;// 防止溢出2
                }
                int& res = memo[i][j][hold];
                if (res != -1) {
                    return res;
                }
                if (hold) {
                    // 这里可能INT_MIN会溢出，因为还会再减一个prices[i]
                    return res = max(dfs(i - 1, j, true), dfs(i - 1, j - 1, false) - prices[i]);
                }
                return res = max(dfs(i - 1, j, false), dfs(i - 1, j, true) + prices[i]);
                };
            return dfs(n - 1, k, false);
        }
    };
}
namespace s188o2
{   // 1:1翻译成递推
    class Solution {
    public:
        int maxProfit(int k, vector<int>& prices) {
            int n = prices.size();
            // 注意k + 2，因为递推式中出现了j - 1，所以j也要像i一样右移 
            // 初始化方法1：直接按照1:1翻译设置边界条件
            vector<vector<vector<int>>> f(n + 1, vector<vector<int>>(k + 2, { 0, 0 }));
            for (int i = 0; i < n + 1; ++i) {
                // 边界条件1
                // f[.][0][.] = INT_MIN / 2; 
                f[i][0] = { INT_MIN / 2, INT_MIN / 2 };
            }
            for (int j = 1; j < k + 2; ++j) {
                // 边界条件2
                // f[0][j][1] = INT_MIN / 2; // j >= 1
                f[0][j][1] = INT_MIN / 2;
            }
            // 初始化方法2：反过来思考，只确保f的边界正确，内部就算全是INT_MIN / 2也无所谓
            vector<vector<vector<int>>> f(n + 1, vector<vector<int>>(k + 2, { INT_MIN / 2, INT_MIN / 2 }));
            for (int j = 1; j <= k + 1; j++) {
                f[0][j][0] = 0;
            }
            // 初始化方法2更加简洁，但一开始不好理解，两者初始化生成的f并不相同，但是边界是相同的：
            // 当j = 0时，全部为INT_MIN / 2
            // 当j >= 1, i = 0时的两种情况也是正确设置的
            // 下方递推式想要“正确启动”，只依赖上述两种情况（也即边界）的正确设置
            // 换句话说，哪怕f内部的数字全是不同的随机数，也不影响得到正确答案
            // 两种初始化本质是通过一致的边界条件（i=0 和 j=0）为DP提供正确的起点，而内部区域的值无论初始化为何种值，
            // 都在首次计算时被覆盖。这体现了动态规划的核心特性：状态仅依赖前驱，而非初始状态本身。

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < k + 1; ++j) {
                    f[i + 1][j + 1][1] = max(f[i][j + 1][1], f[i][j][0] - prices[i]);
                    f[i + 1][j + 1][0] = max(f[i][j + 1][0], f[i][j + 1][1] + prices[i]);
                }
                /* 写成下面这样也可以（灵神写法），但我还是感觉自己的写法比较好理解，把j当作i一样来+1处理
                for (int j = 1; j < k + 2; ++j) {
                    f[i + 1][j][1] = max(f[i][j][1], f[i][j - 1][0] - prices[i]);
                    f[i + 1][j][0] = max(f[i][j][0], f[i][j][1] + prices[i]);
                }
                */
            }
            return f[n][k + 1][0];
        }
    };
}
namespace s188o3
{   // 空间优化：需要逆序遍历2维矩阵，类似“背包”
    // 虽然时间复杂度相较于o2没有改进，但是因为内存更小，cache miss更少，时间常数更小，耗时也就更短
    class Solution {
    public:
        int maxProfit(int k, vector<int>& prices) {
            int n = prices.size();
            // 可以直接去掉一个维度
            // 因为f[i]只从f[i - 1]转移过来，可以直接覆盖f[i - 1]，直接去掉i的维度，类似最简单的无限次股票交易
            vector<vector<int>> f(k + 2, { INT_MIN / 2, INT_MIN / 2 });
            for (int j = 1; j < k + 2; ++j) {
                f[j][0] = 0;
            }

            // f[i][j][0] = max(f[i - 1][j][0], f[i - 1][j][1] + prices[i]);
            // f[i][j][1] = max(f[i - 1][j][1], f[i - 1][j - 1][0] - prices[i]);
            //  ---->
            // f[j][0] = max(f[j][0], f[j][1] + prices[i]);
            // f[j][1] = max(f[j][1], f[j - 1][0] - prices[i]); // 需要逆序遍历，正序会被覆盖成f[i][j - 1]
            // 因为需要读取的是上一行的数据（第i - 1行），但正序遍历时会覆盖成第i行，所以要逆序遍历

            for (int p : prices) {
                for (int j = k; j >= 0; --j) {// 逆序遍历
                    // 注意先后顺序，f[j + 1][0]的计算要用到f[j + 1][1]，而f[j + 1][1]不需要
                    f[j + 1][0] = max(f[j + 1][0], f[j + 1][1] + p);
                    f[j + 1][1] = max(f[j + 1][1], f[j][0] - p);
                }
            }
            return f[k + 1][0];
        }
    };
}
namespace s188o4
{   // 以下为进阶内容，不会也没关系，仅做了解
    // 对于恰好/至少交易k次，相较于至多交易k次，区别主要在于边界条件
    // -------------------------------------------------------------
    /*  对于恰好交易k次：
    相当于第1天开始（第0天结束）时，由于没有任何交易，相当于恰好交易了0次
    又由于表示交易次数的j是整体右移了一位，所以边界条件变成
    记忆化搜索：只有i = 0，且j = 0时，未持有股票时才为0，其他全是-inf
    递推式：f[0][1][0] = 0才表示恰好交易0次，其他全是-inf

    o1记忆化搜索：
    function<int(int, int, bool)> dfs = [&](int i, int j, bool hold) {
        if (j < 0) {
            return INT_MIN / 2;

        }
        if (i < 0) {
            return (hold || j) ? INT_MIN / 2 : 0;
        }
        int& res = memo[i][j][hold];
        if (res != -1) {
            return res;
        }
        if (hold) {
            return res = max(dfs(i - 1, j, true), dfs(i - 1, j - 1, false) - prices[i]);
        }
        return res = max(dfs(i - 1, j, false), dfs(i - 1, j, true) + prices[i]);
        };
    o2递推：
        只需将初始化边界条件设置为：f[0][1][0] = 0即可
    */


    // -------------------------------------------------------------
    /* 对于至少交易k次
    至少交易0次和至少交易-1次含义相同，所以每个f[i]前面不需要插入状态
    “至少0次”等价于可以“无限次交易”
    所以f[i][0][.]就是无限次交易下的最大利润，转移方程也一样

    o1记忆化搜索：
    function<int(int, int, bool)> dfs = [&](int i, int j, bool hold) {
        if (i < 0) {
            return (hold || j > 0) ? INT_MIN / 2 : 0;
        }
        int& res = memo[i][j][hold];
        if (res != -1) {
            return res;
        }
        if (hold) {
            return res = max(dfs(i - 1, j, true), dfs(i - 1, j - 1, false) - prices[i]);
        }
        return res = max(dfs(i - 1, j, false), dfs(i - 1, j, true) + prices[i]);
        };
    o2递推：只需将初始化边界条件设置为：f[0][0][0] = 0即可
        int n = prices.size();
        // 创建一个3D数组f[n+1][k+1][2]，初始值为INT_MIN
        vector<vector<vector<int>>> f(n + 1, vector<vector<int>>(k + 1, vector<int>(2, INT_MIN)));    
        // 初始状态：第0天，0次交易，不持有股票
        f[0][0][0] = 0;
        for (int i = 0; i < n; ++i) {
            int p = prices[i];
            // 处理j=0的情况
            f[i + 1][0][0] = max(f[i][0][0], f[i][0][1] + p);
            f[i + 1][0][1] = max(f[i][0][1], f[i][0][0] - p);
            
            // 处理j=1到k的情况
            for (int j = 1; j <= k; ++j) {
                f[i + 1][j][0] = max(f[i][j][0], f[i][j][1] + p);
                f[i + 1][j][1] = max(f[i][j][1], f[i][j - 1][0] - p);
            }
        }
        
        return f[n][k][0];
    */
}

namespace s123m1
{   // 套s188模板即可， 把k改成2
    class Solution {
    public:
        int maxProfit(vector<int>& prices) {
            int n = prices.size();
            int k = 2;
            vector<vector<int>> f(k + 2, { INT_MIN / 2, INT_MIN / 2 });
            for (int j = 1; j < k + 2; ++j) {
                f[j][0] = 0;
            }

            for (int p : prices) {
                for (int j = k; j >= 0; --j) {
                    f[j + 1][0] = max(f[j + 1][0], f[j + 1][1] + p);
                    f[j + 1][1] = max(f[j + 1][1], f[j][0] - p);
                }
            }
            return f[k + 1][0];
        }
    };
}
// ------------------------------------------------------------------------------------
// 七、其他线性DP(1)：时间有限，直接跳过了
/*
221.最大正方形
*/
namespace s221o1
{

}