#pragma once
#include <vector>
#include <unordered_map>
#include <limits> // numeric_limits<double>::infinity()

using namespace std;

// 【13.1】简单题()
// 未分类的数学类型简单题
/*
66.加一：给定一个表示 大整数 的整数数组 digits，其中 digits[i] 是整数的第 i 位数字。
这些数字按从左到右，从最高位到最低位排列。这个大整数不包含任何前导 0。
将大整数加 1，并返回结果的数字数组。
*/
// ---------------------
namespace s66o1
{
    class Solution {
    public:
        vector<int> plusOne(vector<int>& digits) {
            int n = digits.size();

            for (int i = n - 1; i >= 0; --i) {
                if (digits[i] < 9) {
                    ++digits[i];
                    return digits;
                }
                digits[i] = 0;
            }
            // 不需要整体移动，当进位进到n = 0时，只要一种可能：
            // 所有位数都是9，所以只需全部置零并将n = 0处置1，再在尾部加上0即可
            digits.push_back(0);
            digits[0] = 1;
            return digits;
        }
    };
}


// ---------------------
// 【13.2】数论()
// 判断质数 + 筛选质数 + 质因数分解 + 阶乘分解 + 因子 + 最大公约数(GCD) + 最小公倍数(LCM) + 互质 + ...
/*
172.阶乘后的零：给定一个整数 n ，返回 n! 结果中尾随零的数量。
提示 n! = n * (n - 1) * (n - 2) * ... * 3 * 2 * 1
*/
// ---------------------
// 阶乘分解
namespace s172o1
{   /*
    10是由质因子2 * 5组成的，所以只需计算阶乘乘数中2 * 5的组合数
        1.阶乘中2的个数永远比5多
          2每两个数出现一个，5每五个数出现一个
          所以2 * 5的组合数只取决于5的个数
        2.每5个数就有一个5的倍数，贡献一个5，也即n / 5个
          但像25，125这种数是特例，还要再补上n / 25, n / 125
          所以答案 = n/5 + n/25 + n/125 + n/625 + …（一直到除到 0 为止）
     */
    // 时间复杂度O(logn), 空间复杂度O(1)
    class Solution {
    public:
        int trailingZeroes(int n) {
            int ans = 0;
            while (n) {
                n /= 5; // floor(n / 5^k)
                ans += n;
            }
            return ans;
        }
    };
}
namespace s172extension1 {
    // 如果是要求计算第一个非0的数字，也就是0左边的第一个数，如120中的2，38400中的4
    class Solution {
    public:
        int lastNonZeroDigit(int n) {
            int prod = 1;      // 所有数抠掉 2、5 之后的乘积（只看个位），最大只可能到81，用int即可
            int cnt2 = 0, cnt5 = 0;  // 分别数一数抠掉了多少个 2 和 5

            /*
            n! / (10^cnt5) = n! / (2^cnt5 * 5^cnt5) = M * 2^cnt2 * 5^cnt5 / (2^cnt5 * 5^cnt5) 
                           = M * 2^(cnt2 - cnt5) = prod
            答案就是prod % 10
            其中M 是所有数把 2 和 5 都抠干净之后剩下部分乘起来的结果（它不被 2、5 整除）
            */
            for (int i = 1; i <= n; i++) {
                int x = i;
                while (x % 2 == 0) { x /= 2; cnt2++; }  // 抠 2
                while (x % 5 == 0) { x /= 5; cnt5++; }  // 抠 5
                // 乘积的个位，只取决于每个因子的个位，所以剩下的只乘个位，
                prod = prod * (x % 10) % 10;            
            }

            int extra = cnt2 - cnt5;    // 配完 10 之后多出来的 2 的个数
            static const int tab[4] = { 6, 2, 4, 8 };// 2^4≡6, 2^1≡2, 2^2≡4, 2^3≡8 (mod 10)
            // 不用真的算 2^extra，结果会非常大溢出
            // 我们只关心个位数，2^k的个位数的循环是2，4，8，6
            int mul = (extra == 0) ? 1 : tab[extra % 4];// 2 的幂个位每 4 个一循环

            return prod * mul % 10;
        }
    };
}

// ---------------------
// 【13.3】杂项()
/*
9.回文数：给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。
回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。例如，121 是回文，而 123 不是。
*/
// ---------------------
// 回文数
namespace s9o1
{   // 空间复杂度O(1)做法，不需要转字符串
    class Solution {
    public:
        bool isPalindrome(int x) {
            // x < 0 不是回文数
            // x > 0 且个位数为0，不是回文数
            if (x < 0 || (x > 0 && x % 10 == 0)) {
                return false;
            }

            int rev = 0;
            while (rev < x / 10) {
                rev = rev * 10 + x % 10;
                x /= 10;
            }

            return rev == x || rev == x / 10;
        }
    };
}

namespace s149o1
{
    class Solution {
    public:
        int maxPoints(vector<vector<int>>& points) {
            // 时间复杂度O(n^2)， 空间复杂度O(n)

            // 本题中指出没有重复点，如果有重复点需要单独计算
            // 如 [[0,0], [0,0], [1,1], [2,2]]，若按照以下代码答案会是3而不是4
            int n = points.size(), ans = 0;
            for (int i = 0; i < n - 1; ++i) {
                unordered_map<double, int> cnt;
                auto& p = points[i];
                // 为什么从j 从 i+1 开始就够？
                // 设那条点数最多的直线为 L，L 上的点里，下标最小的是 points[m]
                // 当外层循环跑到 i = m 的时候，L 上所有其他点的下标都比 m 大，所以它们全部会被内层循环统计到
                // 这条“最优直线”一定会在某一次 i 中被完整统计出来
                // 每一条直线，都在它“下标最小的那个点”被选中时被完整覆盖到了
                // 避免了重复统计（每一对点 (i, j) 只被计算一次）
                for (int j = i + 1; j < n; ++j) {
                    auto& q = points[j];
                    int dx = p[0] - q[0];
                    int dy = p[1] - q[1];
                    // numeric_limits<doulbe>::infinity()属于头文件#include<limits>
                    // 不能把dy / dx改成dx / dy，分类讨论是为了规避分母为0的情况
                    double k = dx ? 1.0 * dy / dx : numeric_limits<double>::infinity();
                    ans = max(ans, ++cnt[k]);
                }
            }
            return ans + 1;
        }
    };
}
namespace s149extension {
    // 如果给出的点可以重复，那么在遍历每个点时，重复点的数量需要加到所有j统计的线条里
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            unordered_map<double, int> cnt;
            int same = 0;                       // 与 points[i] 坐标相同的其它点
            for (int j = 0; j < n; j++) {
                if (j == i) continue;
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];
                if (dx == 0 && dy == 0) {       // 重复点：属于所有方向
                    same++;
                    continue;
                }
                double k = dx ? 1.0 * dy / dx : numeric_limits<double>::infinity();
                cnt[k]++;
            }
            for (auto& [k, v] : cnt)
                ans = max(ans, v + same + 1);   // 每个方向都加上重复点
            ans = max(ans, same + 1);           // 全是重复点的退化情况（这个容易漏）
        }
        return ans;
    }
}