#pragma once
#include <vector>
#include <string>

using namespace std;

// 问题待定：
/*
1.状压DP跳过了没有看，只做了区间DP的一道例题
*/

/*
模板题：
1.最长回文子序列：516
2.区间DP：1039（暂时不做）

*/

// 动态规划：区间DP + 状压DP
// 八、区间DP(2)
// 
/*
516.最长回文子序列：给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度。
子序列定义为：不改变剩余字符顺序的情况下，删除某些字符或者不删除任何字符形成的一个序列。

5.最长回文子串：给你一个字符串 s，找到 s 中最长的 回文 子串。
*/
// ---------------------
// 模板题1：最长回文子序列
// 之前背包、线性DP等问题是从数组的前缀或后缀进行问题分解，而区间DP会将问题规模缩小到数组中间的区间上
// 本质上还是选或不选，从数组的左右两端不断缩短，求解关于某段下标区间的最优值，问题分解思路和最大公共子序列类似
// 可以加深递推的本质（边界条件的翻译），这道题的递推和之前做的题目都不一样
namespace s516o1
{	// 思路1：将s反转后得到s`，与s求最长公共子序列，也即LCS问题
    class Solution {
    public:
        int longestPalindromeSubseq(string s) {
            // 直接套用s1143的解法
            string rs = s;
            reverse(rs.begin(), rs.end());
            int n = s.size();
            vector<int> f(n + 1);

            for (int i = 0; i < n; ++i) {
                int pre = f[0]; // 左上值的初始值
                for (int j = 0; j < n; ++j) {
                    int temp = f[j + 1]; // 保存左上值
                    if (s[i] == rs[j]) {
                        // f[i + 1][j + 1] = f[i][j] + 1;
                        f[j + 1] = pre + 1;
                    }
                    else {
                        f[j + 1] = max(f[j], f[j + 1]);
                    }
                    pre = temp; // 更新左上值临时变量
                }
            }
            return f[n];
        }
    };
}
namespace s516o2
{   // 思路2：直接DP做，问题分解思路类似最长公共子序列问题
    // 记忆化搜索
    // dfs(i, j)表示从[i, j]的最长回文子序列长度
    // 递归入口dfs(0, n - 1)
    // dfs(i, j) = dfs(i + 1, j - 1) + 2                (s[i] == s[j])
    //           = max(dfs(i + 1, j), dfs(i, j - 1)     (s[i] != s[j])
    // 递归边界：dfs(i, i) = 1(只有一个字母)  dfs(i + 1, i) = 0(没有字母)

    class Solution {
    public:
        int longestPalindromeSubseq(string s) {
            int n = s.size();
            vector<vector<int>> cache(n, vector<int>(n, -1));

            auto dfs = [&](auto&& dfs, int i, int j)->int {
                if (i > j) return 0;
                if (i == j) return 1;

                int& res = cache[i][j];
                if (res != -1) return res;

                if (s[i] == s[j]) {
                    return res = dfs(dfs, i + 1, j - 1) + 2;
                }
                return res = max(dfs(dfs, i + 1, j), dfs(dfs, i, j - 1));
                };

            return dfs(dfs, 0, n - 1);
        }
    };
}
namespace s516o3
{   // 递推
    class Solution {
    public:
        int longestPalindromeSubseq(string s) {
            int n = s.size();
            vector<vector<int>> f(n, vector<int>(n, 0));

            // 下面的初始化以及i > j时f[i][j] = 0的初始化都可以并入递推的过程
            /*for (int i = 0; i < n; ++i) {
                f[i][i] = 1;
            }*/

            // 注意j 一定要从i + 1开始遍历，否则计算多余的递推，进而导致整条多米诺链条的错乱
            // 比如计算f[3][3]，原本就该初始化为1，但是如果j可以取到i，那么会将f[3][3]重新改写成错误的2
            for (int i = n - 1; i >= 0; --i) {
                f[i][i] = 1;
                for (int j = i + 1; j < n; ++j) {
                    if (s[i] == s[j]) {
                        f[i][j] = f[i + 1][j - 1] + 2;
                    }
                    else {
                        f[i][j] = max(f[i + 1][j], f[i][j - 1]);
                    }
                }
            }
            return f[0][n - 1];
        }
    };
}
namespace s516o4
{   // 空间优化（摆烂了，直接上滚动数组）
    class Solution {
    public:
        int longestPalindromeSubseq(string s) {
            int n = s.size();
            vector<vector<int>> f(2, vector<int>(n));

            for (int i = n - 1; i >= 0; --i) {
                f[i % 2][i] = 1;
                for (int j = i + 1; j < n; ++j) {
                    if (s[i] == s[j]) {
                        f[i % 2][j] = f[(i + 1) % 2][j - 1] + 2;
                    }
                    else {
                        f[i % 2][j] = max(f[(i + 1) % 2][j], f[i % 2][j - 1]);
                    }
                }
            }
            return f[0][n - 1];
        }
    };
}
namespace s516o5
{   // 正经的空间优化
    class Solution {
    public:
        int longestPalindromeSubseq(string s) {
            int n = s.size();
            vector<int> f(n);

            // 相当于还是从左、左上，上向当前元素递推，需要提前保存左上元素
            for (int i = n - 1; i >= 0; --i) {
                f[i] = 1;
                int pre = 0;
                // 每轮递推时如果要用到pre，其初始值为f[i + 1][j - 1], j = i + 1
                // 也即f[i + 1][i] = 0

                /*
                也可以换个角度理解，用到的初始值为f[i + 1][i]，直接保存f[i]也是一样的，无需关心其具体数值
                int pre = f[i];
                f[i] = 1;
                */

                for (int j = i + 1; j < n; ++j) {
                    int temp = f[j];// 当轮覆盖前的f[j]，也就是下轮的f[j - 1]
                    if (s[i] == s[j]) {
                        f[j] = pre + 2;
                    }
                    else {
                        f[j] = max(f[j], f[j - 1]);
                    }
                    pre = temp;
                }
            }
            return f[n - 1];
        }
    };
}

// 模板题2：最长回文子串，用不到DP，有两种做法，时间O(n^2)空间O(1)的中心拓展法，以及时间O(n)空间O(n)的Manacher算法
// 看o1了解中心拓展法思路，用o4;         Manacher算法看o6
namespace s5o1
{   // 核心思想是从回文串的中心开始向两边拓展，可以用O(1)的时间判断是否为回文串，将暴力时间O(n^3)的做法降至O(n^2)
    class Solution {
    public:
        string longestPalindrome(string s) {
            int n = s.size();
            int ans_left = 0, ans_right = 0;

            // 奇回文串
            for (int i = 0; i < n; i++) {
                int l = i, r = i;
                while (l >= 0 && r < n && s[l] == s[r]) {
                    l--;
                    r++;
                }
                // 循环结束后，s[l+1] 到 s[r-1] 是回文串
                if (r - l - 1 > ans_right - ans_left) {
                    ans_left = l + 1;
                    ans_right = r; // 左闭右开区间
                }
            }

            // 偶回文串
            for (int i = 0; i < n - 1; i++) {
                int l = i, r = i + 1;
                while (l >= 0 && r < n && s[l] == s[r]) {
                    l--;
                    r++;
                }
                if (r - l - 1 > ans_right - ans_left) {
                    ans_left = l + 1;
                    ans_right = r; // 左闭右开区间
                }
            }

            return s.substr(ans_left, ans_right - ans_left);
        }
    };
}
namespace s5o2
{   // 可以将奇偶回文串的判断统一，精简代码，但实际效率没有提升，o2只是提供一种精简的思路，实际刷题面试用o1即可
    // 或者有更好的o3写法，更简洁优雅
    class Solution {
    public:
        string longestPalindrome(string s) {
            int n = s.size();
            int ans_left = 0, ans_right = 0;

            // 当回文串为奇数时，共有n个中心
            // 当回文串为偶数时，共有n - 1个中心
            // 共有2 * n - 1个回文子串中心，o2写法就是将这些中心放在一个循环里精简代码，但无效率提升
            for (int i = 0; i < 2 * n - 1; i++) {
                int l = i / 2, r = (i + 1) / 2;
                while (l >= 0 && r < n && s[l] == s[r]) {
                    l--;
                    r++;
                }
                // 循环结束后，s[l+1] 到 s[r-1] 是回文串
                if (r - l - 1 > ans_right - ans_left) {
                    ans_left = l + 1;
                    ans_right = r; // 左闭右开区间
                }
            }

            return s.substr(ans_left, ans_right - ans_left);
        }
    };
}
namespace s5o3
{   // 在o1o2基础上，增加剪枝
    class Solution {
    public:
        string longestPalindrome(string s) {
            int n = s.size();
            int start = 0;
            int len = 0;// 放弃同时更新左右端点，直接更新长度即可

            // 用lambda函数精简代码
            auto update = [&](int l, int r) {
                while (l >= 0 && r < n && s[l] == s[r]) {
                    --l;
                    ++r;
                }
                if (r - l - 1 > len) {
                    len = r - l - 1;
                    start = l + 1;
                }
                };

            for (int i = 0; i < n; ++i) {
                // 下面这个剪枝是可选的，不一定要带
                // 最优性剪枝：提前终止不可能产生更长回文的遍历
                // 如果后半部分扩展最长也不可能超过当前最大值就没必要往下遍历了
                // 如果是奇回文串，下标i的最长扩展长度为 (n - 1 - (i + 1) + 1) * 2 + 1 = (n - i) * 2 - 1
                // 如果是偶回文串，下标i的最长拓展长度为 (n - 1 - (i + 2) + 1) * 2 + 2 = (n - i) * 2 - 2
                // 所以下标i时的最长拓展长度为(n - i) * 2 - 1，如果这个都比当前len小，那后续也没有遍历的必要了
                // （i越大，最大可拓展长度越小），因此不是continue，而是break
                if ((n - i) * 2 - 1 < len) break;

                update(i, i);
                update(i, i + 1);
            }

            return s.substr(start, len);
        }
    };
}
namespace s5o4
{   // 在o3基础上进一步优化剪枝
    // 重复字符其实可以直接跳过，因为只要是重复的字符则一定是回文串，比如aa, a, aaa，可以把其统一当作一个字符处理
    // 因此问题从奇偶遍历变成了只处理奇数子串的遍历，所有偶数子串的中心都当成一个字符处理
    // 题解地址：https://leetcode.cn/problems/longest-palindromic-substring/solutions/3927149/zui-kuai-de-on2suan-fa-jian-zhi-pythonc-0qshg/
    class Solution {
    public:
        string longestPalindrome(string s) {
            int n = s.size();
            int len = 0, start = 0;

            for (int i = 0; i < n;) {
                // 剪枝1：同o3，如果后半部分扩展最长也不可能超过当前最大值就没必要往下遍历了
                if ((n - i) * 2 - 1 < len) break; 

                // 剪枝2：如果是重复字符直接跳过，下次遍历从重复字符后
                int left = i, right = i;
                // 向右扩展，跳过所有与 s[i] 相同的连续字符
                while (right + 1 < n && s[right] == s[right + 1]) {
                    ++right;
                }
                // 此时 [left, right] 为一段完全相同的字符序列，作为回文的中心块
                // 将 i 直接移动到中心块的下一个位置，避免重复以该组内其他字符为中心扩展
                i = right + 1;  // 为下一轮循环更新i，不是常规的在for循环中使用++i

                // 开始中心拓展
                --left;                     // 中心块左外侧
                ++right;                    // 中心块右外侧
                while (left >= 0 && right < n && s[left] == s[right]) {
                    --left;
                    ++right;
                }
                
                // [left + 1, right - 1]为回文子串
                int curLen = right - left - 1;
                if (curLen > len) {
                    len = curLen;
                    start = left + 1;
                }
            }
            return s.substr(start, len);
        }
    };
}
namespace s5o5
{   // Manacher算法，时间复杂度O(n)，最好也掌握，毕竟是在TOP100题单里，要给予重视
    // 算法思维可以看B站ITI学院的马拉车算法视频，根据ITI视频写出来的代码见o6
    // 灵神写的o5写法就算废弃了吧，直接看o6
    class Solution {
    public:
        string longestPalindrome(string s) {
            // Manacher 模板
            // 将 s 改造为 t，这样就不需要讨论 s.length() 的奇偶性，因为新串 t 的每个回文子串都是奇回文串（都有回文中心）
            // s 和 t 的下标转换关系：
            // (si+1)*2 = ti
            // ti/2-1 = si
            // ti 为偶数，对应奇回文串（从 2 开始）
            // ti 为奇数，对应偶回文串（从 3 开始）
            string t = "^";
            for (char c : s) {
                t += '#';
                t += c;
            }
            t += "#$";

            // 定义一个奇回文串的回文半径=(长度+1)/2，即保留回文中心，去掉一侧后的剩余字符串的长度
            // half_len[i] 表示在 t 上的以 t[i] 为回文中心的最长回文子串的回文半径
            // 即 [i-half_len[i]+1,i+half_len[i]-1] 是 t 上的一个回文子串
            vector<int> half_len(t.length() - 2);
            half_len[1] = 1;
            // box_r 表示当前右边界下标最大的回文子串的右边界下标+1
            // box_m 为该回文子串的中心位置，二者的关系为 r=mid+half_len[mid]
            int box_m = 0, box_r = 0, max_i = 0;
            for (int i = 2; i < half_len.size(); i++) {
                int hl = 1;
                if (i < box_r) {
                    // 记 i 关于 box_m 的对称位置 i'=box_m*2-i
                    // 若以 i' 为中心的最长回文子串范围超出了以 box_m 为中心的回文串的范围（即 i+half_len[i'] >= box_r）
                    // 则 half_len[i] 应先初始化为已知的回文半径 box_r-i，然后再继续暴力匹配
                    // 否则 half_len[i] 与 half_len[i'] 相等
                    hl = min(half_len[box_m * 2 - i], box_r - i);
                }

                // 暴力扩展
                // 算法的复杂度取决于这部分执行的次数
                // 由于扩展之后 box_r 必然会更新（右移），且扩展的的次数就是 box_r 右移的次数
                // 因此算法的复杂度 = O(t.length()) = O(n)
                while (t[i - hl] == t[i + hl]) {
                    hl++;
                    box_m = i;
                    box_r = i + hl;
                }

                half_len[i] = hl;
                if (hl > half_len[max_i]) {
                    max_i = i;
                }
            }

            int hl = half_len[max_i];
            // 注意 t 上的最长回文子串的最左边和最右边都是 '#'
            // 所以要对应到 s，最长回文子串的下标是从 max_i-hl+2 到 max_i+hl-2
            // 结合上文的下标转换关系，得到其在 s 上的下标范围是从 (max_i-hl)/2 到 (max_i+hl)/2-2
            return s.substr((max_i - hl) / 2, hl - 1);
        }
    };
}
namespace s5o6
{
    class Solution {
    public:
        string longestPalindrome(string s) {
            string sn = "^";
            for (char c : s) {
                sn += '#';
                sn += c;
            }
            sn += "#$";

            vector<int> p(sn.length(), 0); // 回文半径数组，初始化为 0
            int c = 0, r = 0;              // 当前最右回文子串的中心和右边界
            int maxC = 0, maxLen = 0;

            for (int i = 1; i < sn.length() - 1; ++i) {
                // 首尾字符已做特殊处理，一般不去遍历
                if (i <= r) {
                    // 蘑菇右侧为r, 当前下标为i, 那么从i拓展到r的拓展次数为r - i
                    // 2 * c - i是i关于c的镜像点, 因为c - mirror = i - c
                    // 整个字符串关于中心 c 的回文区间 [c - p[c], c + p[c]] 是对称的。
                    // 在这个大回文区间内，左边所有位置的回文半径和右边对应位置的回文半径有相同或受限制的关系。
                    // 也就是说，在中心 c 的“保护范围”内，如果 mirror 的回文没有超出左边界，
                    // 那么 i 就能完全继承 mirror 的回文半径；如果超出了，就只能继承到大回文的右边界为止
                    p[i] = min(r - i, p[2 * c - i]); // 利用对称性加速
                }
                // 向两边扩展
                while (sn[i - p[i] - 1] == sn[i + p[i] + 1]) {
                    p[i]++;
                }
                // 更新最右边界
                if (p[i] + i > r) {
                    r = p[i] + i;
                    c = i;
                }
                if (p[i] > maxLen) {
                    maxC = c;
                    maxLen = p[i];
                }
            }

            // 不管中心是#还是字符，拓展的边界一定是#，对应回原始下标只需除2，会自动向下去整
            // 自动对应上边界#的右边的那个字符
            int start = (maxC - maxLen) / 2;
            return s.substr(start, maxLen);
        }
    };
}
// ------------------------------------------------------------------------------------
// 九、状压DP()：时间有限，暂时跳过不做
/*

*/
// ---------------------



